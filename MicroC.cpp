#include <Arduino.h>
#include "WiFi.h"
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <HardwareSerial.h>

#define BLU 2
#define WIFI_NETWORK "Mannosproz" 
#define WIFI_PASSWORD "annarella" 
#define WIFI_TIMEOUT_MS 20000
#define DEVICE "ESP32"
#define INFLUXDB_URL "http://192.168.43.179:8087"
#define INFLUXDB_TOKEN "gQbaJqp00M7Uu-cnLva175b3QWiTGCXRiYipov-lv1cJp-sZZr5dBq_18nbWtAjNjPh3I1FffWw_-jPpNS8YxA=="
#define INFLUXDB_ORG "a"
#define INFLUXDB_BUCKET "a"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
  
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("CO2MIS");

void connectToWiFi(){ //funzione per accedere alla rete wifi
  Serial.print("Connecting to wifi");
  WiFi.mode(WIFI_STA); //modalità solo client
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD); //connette alle rete wifi con i parametri specificati prima

  while(WiFi.status() != WL_CONNECTED){ //ciclo che tenta la connessione
    Serial.print(".");
    delay(100);
  }
  //stampa l'ip ottenuto, o in caso contrario "Failed"
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Failed");
  }else{
    Serial.print("Connected!\nIp:  ");
    Serial.print(WiFi.localIP()); //funzione .localIP() : contiene l'ip ottenuto
    digitalWrite(BLU, HIGH); //se è connesso alla rete wifi accende il led blu sull'esp32
  }
}

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(BLU, OUTPUT);
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WIFI_NETWORK);
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");
  connectToWiFi(); 
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop() {
  while (Serial2.available()) {
    sensor.clearFields();
    uint32_t z=Serial2.read();
    Serial.println(z);
    sensor.addField("CO2", z);
    z=0;
    Serial.print("Writing: ");
    Serial.println(sensor.toLineProtocol());
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  }
}
