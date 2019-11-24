#include <ESP8266WebServer.h>
//Setear credenciales de la red 
const char *ssid = "Autito";
const char *password = "12345678";
IPAddress addr(10, 10, 10, 10);

//1-instancia de ESP8266WebServer 
//ESP8266WebServerTemplate(IPAddress addr, int port = 80);
ESP8266WebServer server(addr, 80);

 
//4-Funciones encargada de enviar la respuesta al cliente
void handleRoot(){
    server.send(200, "text/html", "<h1>Hola Mundo</h1>");
}
 
//4-Función que se ejecuta en URL desconocida
void handleNotFound() {
   server.send(404, "text/plain", "Not found");
}
 
void setup(){
    delay(1000);
    Serial.begin(115200);
    WiFi.softAP(ssid, password);
 
    //2-Definir la asociación entre la URL de la petición y
    //la acción que se ejecutará.
    server.on("/", handleRoot);
    
    // Ruteo para '/inline'
    server.on("/inline", []() {
        server.send(200, "text/plain", "Esto también funciona");
    });
    
    // Ruteo para URL desconocida
    server.onNotFound(handleNotFound);  
 
    //Iniciar servidor
    server.begin();
    Serial.println("HTTP server started");
}
 
void loop(){
  //3-función que se encarga de recibir las peticiones de los clientes
  //y ejecutar las acciones asociadas al ruteo.
  server.handleClient();
}
