#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>
#define DHT11_PIN D2
#define DHTTYPE DHT22

DHT dht(DHT11_PIN, DHTTYPE);

const char* ssid     = "SOLANi";
const char* password = "password";

const char* SERVER_NAME = "https://automationatsk.000webhostapp.com/sensordata.php";

String PROJECT_API_KEY = "hello solani";

unsigned long lastMillis = 0;
long interval = 5000;

void setup() {
  
  Serial.begin(115200);
  Serial.println("esp32 serial initialize");
  
  dht.begin();
  Serial.println("initialize DHT22");
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
} 

void loop() {
  
 
  if(WiFi.status()== WL_CONNECTED){
    if(millis() - lastMillis > interval) {
       upload_temperature();
       lastMillis = millis();
    }
  }

  else {
    Serial.println("WiFi Disconnected");
  }
  
  delay(30000);  //time period
}

void upload_temperature()
{
  float t = dht.readTemperature();
  
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  String humidity = String(h, 2);
  String temperature = String(t, 2);

  Serial.println("Temperature: "+temperature);
  Serial.println("Humidity: "+humidity);
  
  Serial.println("--------------------------");
 
  String temperature_data;
  temperature_data = "api_key="+PROJECT_API_KEY;
  temperature_data += "&temperature="+temperature;
  temperature_data += "&humidity="+humidity;

  Serial.print("temperature_data: ");
  Serial.println(temperature_data);
  
  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_NAME);
  
  http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //content-type header
 
  int httpResponseCode = http.POST(temperature_data);  //HTTP POST request
  
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
    
  http.end();
  }
