#include "esp_camera.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>

// ✅ USE o modelo AI_THINKER que já está configurado nos seus pinos
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// Configurações da rede WiFi
const char* ssid = "SafeView";
const char* password = "12345678";

IPAddress local_IP(192, 168, 4, 8);    // IP da Lateral
IPAddress gateway(192, 168, 4, 1);     // Gateway
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

bool cameraInitialized = false;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("Iniciando ESP32-CAM...");

  WiFi.config(local_IP, gateway, subnet);

  // Configurar pino do flash
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  
  // Dar tempo para a câmera estabilizar
  delay(1000);

  // Inicializar câmera
  if (!initCamera()) {
    Serial.println("FALHA CRÍTICA: Câmera não inicializou");
    Serial.println("Verifique as conexões físicas da câmera OV2640");
    Serial.println("Reiniciando em 5 segundos...");
    delay(5000);
    ESP.restart();
    return;
  }

  // Conexão WiFi
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("Conectando ao WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha na conexão WiFi, mas servidor funcionará localmente");
  }

  // Rotas do servidor
  server.on("/", HTTP_GET, handleRoot);
  server.on("/stream", HTTP_GET, handleStream);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

bool initCamera() {
  camera_config_t config;
  
  // Configuração dos pinos (já definidos no camera_pins.h)
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  // Configuração OTIMIZADA
  config.xclk_freq_hz = 10000000;  // 10MHz
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_VGA;  // 640x480 - bom equilíbrio
  config.jpeg_quality = 12;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_LATEST;

  // Tentativa de inicialização
  esp_err_t err = esp_camera_init(&config);
  if (err == ESP_OK) {
    cameraInitialized = true;
    Serial.println("✅ Câmera inicializada com sucesso!");
    return true;
  }

  Serial.printf("❌ Falha na inicialização: 0x%x\n", err);
  
  // Tentativa alternativa com configuração mais leve
  config.frame_size = FRAMESIZE_CIF;  // 400x296
  config.jpeg_quality = 15;
  config.xclk_freq_hz = 8000000;      // 8MHz
  
  err = esp_camera_init(&config);
  if (err == ESP_OK) {
    cameraInitialized = true;
    Serial.println("✅ Câmera inicializada com configuração alternativa!");
    return true;
  }

  Serial.printf("❌ Falha também na configuração alternativa: 0x%x\n", err);
  return false;
}

void loop() {
  server.handleClient();
  delay(1);
}

void handleRoot() {
  String html = "<html><head><title>ESP32-CAM Stream</title>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }";
  html += "img { max-width: 100%; height: auto; border: 1px solid #ccc; }";
  html += ".status { padding: 10px; margin: 10px; border-radius: 5px; }";
  html += ".success { background: #d4edda; color: #155724; }";
  html += ".error { background: #f8d7da; color: #721c24; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>ESP32-CAM Video Stream</h1>";
  
  html += "<div class='status ";
  html += cameraInitialized ? "success" : "error";
  html += "'>";
  html += cameraInitialized ? "✅ Câmera Funcionando" : "❌ Câmera com Problemas";
  html += "</div>";
  
  html += "<p>IP: " + WiFi.localIP().toString() + "</p>";
  
  if (cameraInitialized) {
    html += "<img src='/stream' alt='Video Stream'/>";
  } else {
    html += "<p>Recarregue a página ou reinicie o ESP32</p>";
  }
  
  html += "<br><br>";
  html += "<p><small>Se a imagem não carregar, aguarde alguns segundos</small></p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleStream() {
  if (!cameraInitialized) {
    server.send(503, "text/plain", "Câmera não disponível");
    return;
  }

  WiFiClient client = server.client();
  
  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n";
  response += "Access-Control-Allow-Origin: *\r\n";
  response += "\r\n";
  
  client.print(response);

  while (client.connected()) {
    camera_fb_t* fb = esp_camera_fb_get();
    
    if (fb && fb->len > 0) {
      String frameHeader = "--frame\r\n";
      frameHeader += "Content-Type: image/jpeg\r\n";
      frameHeader += "Content-Length: " + String(fb->len) + "\r\n";
      frameHeader += "\r\n";
      
      client.print(frameHeader);
      client.write((char*)fb->buf, fb->len);
      client.print("\r\n");
      
      esp_camera_fb_return(fb);
    } else if (fb) {
      esp_camera_fb_return(fb);
    }
    
    delay(50); // ~20 FPS
  }
  
  Serial.println("Cliente desconectado do stream");
}