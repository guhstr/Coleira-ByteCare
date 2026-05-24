#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ===== WIFI =====
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// ===== MQTT (HiveMQ Cloud) =====
const char* mqtt_server = "3f3a91da234e4351bfbad9f9e86a145c.s1.eu.hivemq.cloud";
const int   mqtt_port   = 8883;
const char* mqtt_user   = "gustavostr";
const char* mqtt_pass   = "260207Gu";
const char* topic_sub   = "coleira/dias";
const char* topic_status = "coleira/status";

// ===== PINOS =====
#define LED_ROXO    2   // 1 dia  — consulta urgente
#define LED_VERDE   4   // 5 dias
#define LED_AMARELO 5   // 15 dias
#define LED_VERMELHO 18 // mais de 15 dias
#define BUZZER      19

WiFiClientSecure espClient;
PubSubClient client(espClient);

// ===== DESLIGA TODOS OS LEDS =====
void desligarLeds() {
  digitalWrite(LED_ROXO,     LOW);
  digitalWrite(LED_VERDE,    LOW);
  digitalWrite(LED_AMARELO,  LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(BUZZER,       LOW);
}

// ===== ACENDE LED CONFORME OS DIAS =====
void atualizarColeira(int dias) {
  desligarLeds();

  if (dias <= 1) {
    digitalWrite(LED_ROXO, HIGH);
    // Buzzer bipa 3 vezes quando falta 1 dia
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER, HIGH);
      delay(300);
      digitalWrite(BUZZER, LOW);
      delay(200);
    }
    client.publish(topic_status, "URGENTE: consulta amanha!");
  } else if (dias <= 5) {
    digitalWrite(LED_VERDE, HIGH);
    client.publish(topic_status, "OK: consulta em breve");
  } else if (dias <= 15) {
    digitalWrite(LED_AMARELO, HIGH);
    client.publish(topic_status, "OK: consulta nos proximos 15 dias");
  } else {
    digitalWrite(LED_VERMELHO, HIGH);
    client.publish(topic_status, "OK: consulta longe");
  }
}

// ===== CALLBACK MQTT =====
void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (unsigned int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.print("MQTT recebido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);

  int dias = mensagem.toInt();
  atualizarColeira(dias);
}

// ===== WIFI =====
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

// ===== RECONECTAR MQTT =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32_COLEIRA", mqtt_user, mqtt_pass)) {
      Serial.println("Conectado!");
      client.subscribe(topic_sub);
    } else {
      Serial.print("Erro: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  pinMode(LED_ROXO,     OUTPUT);
  pinMode(LED_VERDE,    OUTPUT);
  pinMode(LED_AMARELO,  OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER,       OUTPUT);

  desligarLeds();
  setup_wifi();

  espClient.setInsecure();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ===== LOOP =====
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}