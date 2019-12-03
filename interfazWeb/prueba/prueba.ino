#include <ESP8266WebServer.h>

//Setear credenciales de la red 
const char *ssid = "Autito";
const char *password = "12345678";
//IPAddress addr(10, 10, 10, 10);


//1-instancia de ESP8266WebServer 
//ESP8266WebServerTemplate(IPAddress addr, int port = 80);
//ESP8266WebServer server(addr, 80);
ESP8266WebServer server(80);
 
//4-Funciones encargada de enviar la respuesta al cliente
void handleRoot(){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", "<h1>Hola Mundo</h1>");
}
 
//4-Función que se ejecuta en URL desconocida
void handleNotFound() {
  server.sendHeader("Access-Control-Allow-Origin", "*");
   server.send(404, "text/plain", "Not found");
}
 
void setup(){
    delay(1000);
    
    Serial.begin(9600); //Begin Serial at 115200 Baud 9600
    WiFi.softAP(ssid, password);
   /* WiFi.begin(ssid, password);
    IPAddress ip(10,10,10,10);   
    IPAddress gateway(192,168,1,254);   
    IPAddress subnet(255,255,255,0);
    WiFi.config(ip, gateway, subnet);*/

     /*Serial.print("IP address: antes");
    Serial.println(WiFi.localIP());  //Print the local IP*/
    
    /*while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
    }*/
   
    
    if (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
    }
    
  
    Serial.print("IP address: despues");
    Serial.println(WiFi.localIP());  //Print the local IP
  
    //2-Definir la asociación entre la URL de la petición y
    //la acción que se ejecutará.
    server.on("/", handleRoot);
    
    // Ruteo para '/inline'
    /*server.on("/inline", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Esto también funciona");
    });*/
    //server.on("/", handle_index); //Handle Index page


    server.on("/izquierda", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "IZQUIERDA");
    });

    server.on("/derecha", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "DERECHA");
    });

    server.on("/adelante", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "ADELANTE");
    });

    server.on("/atras", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "ATRAS");
    });

    server.on("/sensores", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "SENSORES");
    });
    
    // Ruteo para URL desconocida
    server.onNotFound(handleNotFound);  
   
  
   //Iniciar servidor
    server.begin(); //Start the server
    Serial.println("HTTP server started");
}
 
void loop(){
  //3-función que se encarga de recibir las peticiones de los clientes
  //y ejecutar las acciones asociadas al ruteo.
  server.handleClient();
}
/*void handle_index() {
  //Print Hello at opening homepage
  server.send(200, "text/plain", "Hello! This is an index page.");
}*/
