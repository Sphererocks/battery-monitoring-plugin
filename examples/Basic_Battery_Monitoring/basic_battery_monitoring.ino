
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <WebServer.h>

const char* ssid = "SphereMako";
const char* password = "5phere7708";

Adafruit_ADS1115 ads;
const float ACS758_SENSITIVITY = 0.0190;
const float ZERO_CURRENT_VOLTAGE = 2.522;
const float ADC_LSB = 0.1875 / 1000;

WebServer server(80);

float readCurrent() {
  int16_t adcValue = ads.readADC_SingleEnded(0);
  float voltage = adcValue * ADC_LSB;
  return (voltage - ZERO_CURRENT_VOLTAGE) / ACS758_SENSITIVITY;
}

void handleRoot() {
  float current = readCurrent();
  String html = "<html><head><title>Battery Monitor</title></head><body>";
  html += "<h1>Real-Time Battery Current</h1>";
  html += "<p>Current: " + String(current, 2) + " A</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(5, 4);

  if (!ads.begin(0x48)) {
    Serial.println("ADS1115 init failed!");
    while (1);
  }
  ads.setGain(GAIN_TWOTHIRDS);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi connected.");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
