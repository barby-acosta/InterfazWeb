#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//============
//#include<SoftwareSerial.h> //Included SoftwareSerial Library
//Started SoftwareSerial at RX and TX pin of ESP8266/NodeMCU
SoftwareSerial s(3,1);
//============

//Setear credenciales de la red 
const char *ssid = "Autito";
const char *password = "12345678";

//IPAddress addr(10, 10, 10, 10);

//1-instancia de ESP8266WebServer 
//ESP8266WebServerTemplate(IPAddress addr, int port = 80);
//ESP8266WebServer server(addr, 80);
ESP8266WebServer server(80);


 void sendJson(char *json)
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
}


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

    //===========
    //Serial S Begin at 9600 Baud
    s.begin(9600);
    //===========

    delay(1000);
    Serial.begin(9600); //Begin Serial at 115200 Baud 9600
    WiFi.softAP(ssid, password); 

    //Esto supuestamente te deja setear la IP del servidor. No funciona.
   /* WiFi.begin(ssid, password);
    IPAddress ip(10,10,10,10);   
    IPAddress gateway(192,168,1,254);   
    IPAddress subnet(255,255,255,0);
    WiFi.config(ip, gateway, subnet);*/
       
    if (WiFi.status() != WL_CONNECTED) {  //Wait for connection
      delay(500);
      Serial.println("Waiting to connect...");
    }
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //Print the local IP
  
    //2-Definir la asociación entre la URL de la petición y
    //la acción que se ejecutará.
    server.on("/", handleRoot);
    
    // Ruteo para URL desconocida
    server.onNotFound(handleNotFound);  

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
        sendJson(
          "{\"adelante\":0, \"atras\":1, \"izquierda\":2, \"derecha\":3, \"direccion\":4, \"velocidad\":5, \"doblando\":6}"
        );
    });
    
    server.on("/frenar", []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "FRENAR");
    });
   
    server.on("/mensaje", []() {
      /*server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", "MENSAJE");*/
      //===========
      //Write '123' to Serial
      s.write('1');
      delay(1000);
      
      //while( Serial.read()==nullptr);
      char data[2];
      data[0] =  Serial.read(); //Read the serial data and store it
      data[1]='\0';
       
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain",  data);
      //===========
    });   
  
   //Iniciar servidor
    server.begin(); //Start the server
    Serial.println("HTTP server started");
}
 
void loop(){
  //3-función que se encarga de recibir las peticiones de los clientes
  //y ejecutar las acciones asociadas al ruteo.
  server.handleClient();

}
