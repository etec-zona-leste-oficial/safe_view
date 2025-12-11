#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

int echoPin = 13;
int trigPin = 12;

// ============= CONFIGURAÇÕES DO ACCESS POINT =============
const char* ap_ssid = "SafeView";
const char* ap_password = "12345678";

// Configuração de IP fixo para o AP
IPAddress ap_local_IP(192, 168, 4, 1);
IPAddress ap_gateway(192, 168, 4, 1);
IPAddress ap_subnet(255, 255, 255, 0);

// ============= CONFIGURAÇÕES DA SUA REDE WiFi =============
const char* sta_ssid = "Ester";        // ⚠️ ALTERE AQUI
const char* sta_password = "25110709ester";   // ⚠️ ALTERE AQUI

// ============= CONFIGURAÇÕES DO SERVIDOR =============
const char* server_url = "192.168.4.2:5000/api/sensor-data";  // ⚠️ ALTERE AQUI
// Exemplo: "http://192.168.1.100:5000/api/sensor-data"

// Cria servidor web na porta 80
WebServer server(80);

// Variável global para armazenar a última distância
int ultimaDistancia = 100;
unsigned long ultimaLeitura = 0;
unsigned long ultimoEnvio = 0;

// Status da conexão
bool sta_connected = false;

void setup() {
  Serial.begin(115200);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  delay(1000);
  
  Serial.println();
  Serial.println("===========================================");
  Serial.println("  ESP32 - Modo AP + STA (SafeView)");
  Serial.println("===========================================");
  
  // ============= CONFIGURAR ACCESS POINT =============
  Serial.println("\n[1] Configurando Access Point...");
  WiFi.mode(WIFI_AP_STA);  // Modo híbrido AP + Station
  
  WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet);
  WiFi.softAP(ap_ssid, ap_password);
  
  Serial.println("✓ Access Point criado!");
  Serial.print("  SSID: ");
  Serial.println(ap_ssid);
  Serial.print("  IP do AP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("  Senha: ");
  Serial.println(ap_password);
  
  // ============= CONECTAR À REDE WiFi =============
  Serial.println("\n[2] Conectando à rede WiFi...");
  Serial.print("  Rede: ");
  Serial.println(sta_ssid);
  
  WiFi.begin(sta_ssid, sta_password);
  
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    sta_connected = true;
    Serial.println("✓ Conectado à Internet!");
    Serial.print("  IP na rede: ");
    Serial.println(WiFi.localIP());
    Serial.print("  Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("  DNS: ");
    Serial.println(WiFi.dnsIP());
  } else {
    sta_connected = false;
    Serial.println("✗ Falha ao conectar à rede WiFi");
    Serial.println("  O ESP32 continuará funcionando apenas como AP");
  }
  
  // ============= CONFIGURAR SERVIDOR WEB =============
  Serial.println("\n[3] Iniciando servidor HTTP...");
  
  // Rota para fornecer dados do sensor
  server.on("/api/sensor-data", HTTP_GET, handleSensorData);
  
  // Rota para status do sistema
  server.on("/api/status", HTTP_GET, handleStatus);
  
  // Rota raiz
  server.on("/", HTTP_GET, handleRoot);
  
  server.begin();
  Serial.println("✓ Servidor HTTP iniciado na porta 80");
  
  Serial.println("\n===========================================");
  Serial.println("  SISTEMA PRONTO!");
  Serial.println("===========================================");
  Serial.println("Acessos disponíveis:");
  Serial.println("  - http://192.168.4.1 (via AP)");
  Serial.println("  - http://192.168.4.1/api/sensor-data");
  if (sta_connected) {
    Serial.print("  - http://");
    Serial.print(WiFi.localIP());
    Serial.println(" (via rede WiFi)");
  }
  Serial.println("===========================================\n");
}

void handleRoot() {
  String html = "<html><head><meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='2'>";
  html += "<style>body{font-family:Arial;background:#182946;color:#fff;padding:20px;}";
  html += ".box{background:#0f1c32;padding:15px;margin:10px 0;border-radius:10px;}";
  html += ".safe{color:#4ade80;} .warning{color:#facc15;} .danger{color:#f87171;}</style></head>";
  html += "<body><h1>SafeView Sensor</h1>";
  
  html += "<div class='box'><h2>Sensor Ultrassônico</h2>";
  html += "<p>Distância: <strong>" + String(ultimaDistancia) + " cm</strong></p>";
  
  String statusClass = "safe";
  String statusText = "SEGURO";
  if (ultimaDistancia < 30) {
    statusClass = "danger";
    statusText = "PERIGO";
  } else if (ultimaDistancia < 60) {
    statusClass = "warning";
    statusText = "ATENÇÃO";
  }
  html += "<p>Status: <span class='" + statusClass + "'>" + statusText + "</span></p></div>";
  
  html += "<div class='box'><h2>Conexões</h2>";
  html += "<p>AP: <strong>" + String(ap_ssid) + "</strong> (" + WiFi.softAPIP().toString() + ")</p>";
  html += "<p>Dispositivos conectados: <strong>" + String(WiFi.softAPgetStationNum()) + "</strong></p>";
  
  if (sta_connected && WiFi.status() == WL_CONNECTED) {
    html += "<p>WiFi: <strong>Conectado</strong> (" + WiFi.localIP().toString() + ")</p>";
  } else {
    html += "<p>WiFi: <strong>Desconectado</strong></p>";
  }
  html += "</div>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleStatus() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  String json = "{";
  json += "\"ap_ssid\":\"" + String(ap_ssid) + "\",";
  json += "\"ap_ip\":\"" + WiFi.softAPIP().toString() + "\",";
  json += "\"ap_clients\":" + String(WiFi.softAPgetStationNum()) + ",";
  json += "\"sta_connected\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
  json += "\"sta_ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"distance\":" + String(ultimaDistancia) + ",";
  json += "\"uptime\":" + String(millis() / 1000);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleSensorData() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
  
  String status;
  if (ultimaDistancia < 30) {
    status = "danger";
  } else if (ultimaDistancia < 60) {
    status = "warning";
  } else {
    status = "safe";
  }
  
  String json = "{";
  json += "\"distance\":" + String(ultimaDistancia) + ",";
  json += "\"status\":\"" + status + "\",";
  json += "\"last_update\":" + String(ultimaLeitura);
  json += "}";
  
  server.send(200, "application/json", json);
  
  Serial.print("Dados enviados via HTTP: ");
  Serial.print(ultimaDistancia);
  Serial.print("cm - Status: ");
  Serial.println(status);
}

int lerDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duracao = pulseIn(echoPin, HIGH, 30000);
  if (duracao == 0) {
    return -1;
  }
  
  int distancia = (duracao / 2) / 29.1;
  return distancia;
}

void enviarDadosParaServidor() {
  if (!sta_connected || WiFi.status() != WL_CONNECTED) {
    return;  // Não enviar se não estiver conectado
  }
  
  HTTPClient http;
  http.begin(server_url);
  http.addHeader("Content-Type", "application/json");
  
  String status;
  if (ultimaDistancia < 30) {
    status = "danger";
  } else if (ultimaDistancia < 60) {
    status = "warning";
  } else {
    status = "safe";
  }
  
  String jsonData = "{";
  jsonData += "\"distance\":" + String(ultimaDistancia) + ",";
  jsonData += "\"status\":\"" + status + "\"";
  jsonData += "}";
  
  int httpResponseCode = http.POST(jsonData);
  
  if (httpResponseCode > 0) {
    Serial.print("✓ Dados enviados ao servidor: ");
    Serial.print(ultimaDistancia);
    Serial.print("cm - HTTP ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("✗ Erro ao enviar: ");
    Serial.println(http.errorToString(httpResponseCode));
  }
  
  http.end();
}

void loop() {
  // Processa requisições HTTP
  server.handleClient();
  
  // Lê o sensor a cada 200ms
  unsigned long tempoAtual = millis();
  if (tempoAtual - ultimaLeitura >= 200) {
    int distancia = lerDistancia();
    
    if (distancia > 0 && distancia < 400) {
      ultimaDistancia = distancia;
      ultimaLeitura = tempoAtual;
      
      Serial.print("📏 Distância: ");
      Serial.print(distancia);
      Serial.println(" cm");
    } else {
      Serial.println("⚠ Leitura inválida do sensor");
    }
  }
  
  // Envia dados ao servidor a cada 1 segundo (se conectado à WiFi)
  if (tempoAtual - ultimoEnvio >= 1000) {
    if (sta_connected && WiFi.status() == WL_CONNECTED) {
      enviarDadosParaServidor();
    }
    ultimoEnvio = tempoAtual;
  }
  
  // Verifica reconexão WiFi a cada 10 segundos
  static unsigned long ultimaVerificacao = 0;
  if (tempoAtual - ultimaVerificacao >= 10000) {
    if (sta_connected && WiFi.status() != WL_CONNECTED) {
      Serial.println("⚠ WiFi desconectado. Tentando reconectar...");
      WiFi.begin(sta_ssid, sta_password);
    } else if (!sta_connected && WiFi.status() == WL_CONNECTED) {
      sta_connected = true;
      Serial.println("✓ WiFi reconectado!");
      Serial.print("  IP: ");
      Serial.println(WiFi.localIP());
    }
    ultimaVerificacao = tempoAtual;
  }
  
  // Mostra status a cada 30 segundos
  static unsigned long ultimoStatus = 0;
  if (tempoAtual - ultimoStatus >= 30000) {
    Serial.println("\n--- STATUS ---");
    Serial.print("AP Clientes: ");
    Serial.println(WiFi.softAPgetStationNum());
    Serial.print("WiFi: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Conectado" : "Desconectado");
    Serial.println("--------------\n");
    ultimoStatus = tempoAtual;
  }
  
  delay(10);
}