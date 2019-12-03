#include <ESP8266WebServer.h>
/* Set these to your desired credentials. */
const char *ssid = "simple";
ESP8266WebServer server(80);

void handleRoot(){
  server.send(200, "text/html", "<h1>Reply from ESP8266</h1>");
}

void setup(){
  delay(1000);
  WiFi.softAP(ssid);
  server.on("/", handleRoot);
  server.begin();
}

void loop(){
  server.handleClient();
}
