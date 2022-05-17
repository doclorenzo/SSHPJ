#include <Wire.h>  
#include <Adafruit_Sensor.h> 
#include <Adafruit_BME280.h> 
#include "WiFi.h" 
#include <InfluxDbClient.h> 
#include <InfluxDbCloud.h> 
#include <time.h>
#include <stdlib.h>

#define BLU 2 //pin blue led esp32
#define WIFI_NETWORK //"wifi-ssid" 
#define WIFI_PASSWORD //"password" 
#define WIFI_TIMEOUT_MS 20000 
#define DEVICE // "device-name" 
#define INFLUXDB_URL // "http://ip:9999" 
#define INFLUXDB_TOKEN // "token" 
#define INFLUXDB_ORG  // "org" 
#define INFLUXDB_BUCKET // "bucket" 
#define TZ_INFO // "time-zone" 
Adafruit_BME280 bme;   
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert); 
Point sensor("dokUniMeasures"); 
void connectToWiFi(){ 
 Serial.print("Connecting to wifi"); 
 WiFi.mode(WIFI_STA);  
 WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);  
 while(WiFi.status() != WL_CONNECTED){  
 Serial.print("."); 
 delay(100); 
 } 
 if(WiFi.status() != WL_CONNECTED){ 
 Serial.print("Failed"); 
 }else{ 
 Serial.print("Connected!\nIp: "); 
 Serial.print(WiFi.localIP());  
 digitalWrite(BLU, HIGH); 
 } 
} 
void setup() { 
 Serial.begin(9600); 
 pinMode(BLU, OUTPUT); 
 connectToWiFi(); 
 Wire.begin();  
 bme.begin(0x76);  
 sensor.addTag("device", DEVICE); 
 if (client.validateConnection()) { 
 Serial.print("\nConnected to InfluxDB: "); 
 Serial.println(client.getServerUrl()); 
 } else { 
 Serial.print("InfluxDB connection failed: "); 
 Serial.println(client.getLastErrorMessage()); 
 } 
} 
void loop() { 
 srand(time(NULL));
 sensor.clearFields(); 

 float hum=bme.readHumidity(); 
 float tem=bme.readTemperature(); 
 int co2 =(rand() % (1700 - 1300 + 1)) + 1300;

 sensor.addField("HUM", hum); 
 sensor.addField("TEM", tem); 
 sensor.addField("CO2", co2);

 Serial.print("Writing: "); 
 Serial.println(sensor.toLineProtocol()); 
 if (!client.writePoint(sensor)) { 
 Serial.print("InfluxDB write failed: "); 
 Serial.println(client.getLastErrorMessage()); 
 } 
 delay(10000); 
} 
