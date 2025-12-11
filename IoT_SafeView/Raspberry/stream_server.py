import cv2
import threading
import time
import requests
from flask import Flask, Response, jsonify, request
from flask_cors import CORS
import numpy as np

app = Flask(__name__)
CORS(app)

# Configuração da câmera única
CAMERA = {
    'id': 'lateral',
    'name': 'Câmera Lateral',
    'ip': '192.168.4.9',
    'url': 'http://192.168.4.9/stream',
    'active': False,
    'current_frame': None,
    'frame_lock': threading.Lock()
}

stream_active = True

# Dados do sensor frontal
sensor_data = {
    'distance': 100,
    'status': 'safe',
    'last_update': time.time()
}

# Configuração do ESP32 com sensor (AP)
SENSOR_ESP32_URL = "http://192.168.4.1/api/sensor-data"

def fetch_sensor_data():
    """Busca dados do sensor ESP32 periodicamente"""
    global sensor_data
    
    print("Iniciando thread de leitura do sensor...")
    retry_count = 0
    
    while stream_active:
        try:
            # Faz requisição GET para o ESP32
            response = requests.get(SENSOR_ESP32_URL, timeout=3)
            
            if response.status_code == 200:
                data = response.json()
                
                # Atualiza dados do sensor
                sensor_data['distance'] = data.get('distance', 100)
                sensor_data['status'] = data.get('status', 'safe')
                sensor_data['last_update'] = time.time()
                
                if retry_count > 0:
                    print(f"✓ Reconectado ao sensor ESP32")
                    retry_count = 0
                
                # Log apenas quando mudar de status
                status_atual = sensor_data['status']
                if not hasattr(fetch_sensor_data, 'last_status') or fetch_sensor_data.last_status != status_atual:
                    print(f"Sensor: {sensor_data['distance']}cm - Status: {status_atual}")
                    fetch_sensor_data.last_status = status_atual
                    
            else:
                print(f"Erro HTTP {response.status_code} do sensor ESP32")
                retry_count += 1
                
        except requests.exceptions.ConnectionError:
            if retry_count == 0:
                print(f"✗ Não foi possível conectar ao ESP32 em {SENSOR_ESP32_URL}")
                print("  Verifique:")
                print("  1. Se o computador está conectado à rede WiFi 'SafeView'")
                print("  2. Se o ESP32 está ligado e funcionando")
                print("  3. Se o IP do ESP32 é realmente 192.168.4.1")
            retry_count += 1
            
        except requests.exceptions.Timeout:
            if retry_count == 0:
                print(f"Timeout ao conectar ao sensor ESP32")
            retry_count += 1
            
        except Exception as e:
            if retry_count == 0:
                print(f"Erro ao buscar dados do sensor: {e}")
            retry_count += 1
        
        # Aguarda antes da próxima leitura
        time.sleep(0.5)  # Lê a cada 500ms
        
        # Marca como offline se não receber dados há muito tempo
        if time.time() - sensor_data['last_update'] > 5:
            if sensor_data['status'] != 'offline':
                sensor_data['status'] = 'offline'
                print("⚠ Sensor offline (sem dados há 5 segundos)")

def fetch_camera_stream(camera):
    """Captura frames do stream da câmera ESP32 em thread separada"""
    print(f"Iniciando captura da {camera['name']} ({camera['ip']})")
    
    stream_url = camera['url']
    retry_count = 0
    max_retries = 3
    
    while stream_active:
        try:
            if retry_count == 0 or retry_count % 5 == 0:
                print(f"Tentando conectar à {camera['name']}... (tentativa {retry_count + 1})")
            
            response = requests.get(
                stream_url, 
                stream=True, 
                timeout=(10, 30),
                headers={'Connection': 'keep-alive'}
            )
            
            if response.status_code == 200:
                print(f"✓ Conectado à {camera['name']}")
                camera['active'] = True
                retry_count = 0
                
                bytes_buffer = bytes()
                last_frame_time = time.time()
                
                for chunk in response.iter_content(chunk_size=2048):
                    if not stream_active:
                        break
                    
                    if not chunk:
                        continue
                        
                    bytes_buffer += chunk
                    
                    while True:
                        a = bytes_buffer.find(b'\xff\xd8')
                        b = bytes_buffer.find(b'\xff\xd9')
                        
                        if a != -1 and b != -1 and b > a:
                            jpg_data = bytes_buffer[a:b+2]
                            bytes_buffer = bytes_buffer[b+2:]
                            
                            try:
                                np_frame = np.frombuffer(jpg_data, dtype=np.uint8)
                                frame = cv2.imdecode(np_frame, cv2.IMREAD_COLOR)
                                
                                if frame is not None:
                                    with camera['frame_lock']:
                                        camera['current_frame'] = frame
                                    last_frame_time = time.time()
                            except Exception as e:
                                print(f"Erro ao decodificar frame: {e}")
                        else:
                            break
                    
                    if time.time() - last_frame_time > 30:
                        print(f"Sem frames há 30s, reconectando {camera['name']}")
                        break
                        
            else:
                print(f"Erro HTTP {response.status_code} na {camera['name']}")
                camera['active'] = False
                retry_count += 1
                
        except Exception as e:
            if retry_count == 0 or retry_count % 5 == 0:
                print(f"Erro na {camera['name']}: {e}")
            camera['active'] = False
            retry_count += 1
        
        if retry_count > 0:
            wait_time = min(5 * (2 ** (retry_count - 1)), 60)
            time.sleep(wait_time)
            
            if retry_count >= max_retries:
                retry_count = 0
                time.sleep(5)

def generate_frames():
    """Gera frames no formato MJPEG"""
    while stream_active:
        with CAMERA['frame_lock']:
            if CAMERA['current_frame'] is not None:
                ret, buffer = cv2.imencode('.jpg', CAMERA['current_frame'], [
                    cv2.IMWRITE_JPEG_QUALITY, 70
                ])
                
                if ret:
                    frame_bytes = buffer.tobytes()
                    yield (b'--frame\r\n'
                           b'Content-Type: image/jpeg\r\n\r\n' + 
                           frame_bytes + b'\r\n')
        
        time.sleep(0.1)

@app.route('/')
def index():
    """Página inicial com status da câmera"""
    status = "✅ Conectada" if CAMERA['active'] else "❌ Desconectada"
    sensor_status = sensor_data['status']
    distance = sensor_data['distance']
    
    # Emojis baseados no status
    status_emoji = {
        'safe': '🟢',
        'warning': '🟡',
        'danger': '🔴',
        'offline': '⚫'
    }
    
    status_html = f"""
    <html>
    <head>
        <meta http-equiv="refresh" content="2">
    </head>
    <body style='background: #182946; color: white; font-family: Arial; padding: 20px;'>
        <h1>Servidor de Câmera - SafeView</h1>
        <div style='margin: 20px 0; padding: 15px; background: #0f1c32; border-radius: 10px;'>
            <h3>{CAMERA['name']} ({CAMERA['ip']})</h3>
            <p>Status: {status}</p>
            <img src='/stream/lateral' width='640' style='border: 2px solid #2a4365; border-radius: 5px;'/>
        </div>
        <div style='margin: 20px 0; padding: 15px; background: #0f1c32; border-radius: 10px;'>
            <h3>Sensor Frontal {status_emoji.get(sensor_status, '⚫')}</h3>
            <p>Distância: <strong>{distance} cm</strong></p>
            <p>Status: <strong>{sensor_status.upper()}</strong></p>
            <p style='font-size: 12px; color: #888;'>Atualizado: {time.time() - sensor_data['last_update']:.1f}s atrás</p>
        </div>
    </body>
    </html>
    """
    return status_html

@app.route('/stream/lateral')
def video_stream():
    """Stream de vídeo da câmera"""
    return Response(
        generate_frames(),
        mimetype='multipart/x-mixed-replace; boundary=frame',
        headers={
            'Access-Control-Allow-Origin': '*',
            'Cache-Control': 'no-cache'
        }
    )

@app.route('/api/cameras')
def get_cameras():
    """API para informações da câmera"""
    return jsonify([{
        'id': CAMERA['id'],
        'name': CAMERA['name'],
        'ip': CAMERA['ip'],
        'active': CAMERA['active'],
        'stream_url': f'/stream/{CAMERA["id"]}'
    }])

@app.route('/api/status')
def status():
    """Status do servidor e câmera"""
    return jsonify({
        'server_status': 'online',
        'camera_active': CAMERA['active'],
        'sensor_status': sensor_data['status'],
        'sensor_distance': sensor_data['distance'],
        'camera': {
            'id': CAMERA['id'],
            'name': CAMERA['name'],
            'ip': CAMERA['ip'],
            'active': CAMERA['active']
        }
    })

@app.route('/api/sensor-data', methods=['GET'])
def sensor_data_endpoint():
    """Retorna dados do sensor (lidos do ESP32)"""
    
    # Verifica se os dados estão desatualizados
    if time.time() - sensor_data['last_update'] > 5:
        current_status = 'offline'
    else:
        current_status = sensor_data['status']
    
    return jsonify({
        'distance': sensor_data['distance'],
        'status': current_status,
        'last_update': sensor_data['last_update'],
        'time_since_update': time.time() - sensor_data['last_update']
    })

if __name__ == '__main__':
    print("=" * 60)
    print("Servidor de Câmera SafeView")
    print("=" * 60)
    
    # Inicia thread de leitura do sensor
    print("\n🔍 Iniciando conexão com sensor ESP32...")
    print(f"   URL: {SENSOR_ESP32_URL}")
    print("   IMPORTANTE: Certifique-se de estar conectado à rede WiFi 'SafeView'")
    
    sensor_thread = threading.Thread(
        target=fetch_sensor_data,
        daemon=True
    )
    sensor_thread.start()
    
    time.sleep(2)
    
    # Inicia thread da câmera
    print(f"\n📹 Iniciando conexão com câmera...")
    camera_thread = threading.Thread(
        target=fetch_camera_stream, 
        args=(CAMERA,), 
        daemon=True
    )
    camera_thread.start()
    
    time.sleep(2)
    
    print("\n✓ Servidor Flask iniciando...")
    print("\nRotas disponíveis:")
    print("  - http://localhost:5000 (Status da câmera e sensor)")
    print("  - http://localhost:5000/stream/lateral (Stream)")
    print("  - http://localhost:5000/api/cameras (API câmera)")
    print("  - http://localhost:5000/api/status (API status)")
    print("  - http://localhost:5000/api/sensor-data (API sensor - GET)")
    print("\n" + "=" * 60)
    
    try:
        app.run(
            host='0.0.0.0',
            port=5000,
            debug=False,
            threaded=True
        )
    except KeyboardInterrupt:
        print("\n\nParando servidor...")
    finally:
        stream_active = False
        print("Servidor finalizado")