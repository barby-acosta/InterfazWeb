#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Setear credenciales de la red
const char *ssid = "Autito";
const char *password = "12345678";

//1-instancia de ESP8266WebServer
ESP8266WebServer server(80);

void sendPlainText(char *plaintext)
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", plaintext);
}

void sendJson(char *json)
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
}

//4-Funciones encargada de enviar la respuesta al cliente
void handleRoot()
{
    sendPlainText("Autito corriendo");
}

//4-Función que se ejecuta en URL desconocida
void handleNotFound()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(404, "text/plain", "Not found");
}

void setup()
{
    delay(1000);

    Serial.begin(9600); //Begin Serial at 115200 Baud 9600
    WiFi.softAP(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    { //Wait for connection
        delay(500);
        Serial.println("Waiting to connect...");
    }

    Serial.print("IP address: despues");
    Serial.println(WiFi.localIP()); //Print the local IP

    //2-Definir la asociación entre la URL de la petición y
    //la acción que se ejecutará.
    server.on("/", handleRoot);

    server.on("/frenar", []() {
        sendPlainText("FRENAR OK");
    });

    server.on("/masmarcha", []() {
        sendPlainText("MAS MARCHA OK");
    });

    server.on("/menosmarcha", []() {
        sendPlainText("MENOS MARCHA OK");
    });

    server.on("/izquierda", []() {
        sendPlainText("IZQUIERDA OK");
    });

    server.on("/derecha", []() {
        sendPlainText("DERECHA OK");
    });

    server.on("/adelante", []() {
        sendPlainText("ADELANTE OK");
    });

    server.on("/atras", []() {
        sendPlainText("ATRAS OK");
    });

    server.on("/sensores", []() {
        sendJson(
          "{\"adelante\":0, \"atras\":1, \"izquierda\":2, \"derecha\":3, \"direccion\":4, \"velocidad\":5, \"doblando\":6}"
        );
    });

    // Ruteo para URL desconocida
    server.onNotFound(handleNotFound);

    //Iniciar servidor
    server.begin(); //Start the server
    Serial.println("HTTP server started");
}

void loop()
{
    //3-función que se encarga de recibir las peticiones de los clientes
    //y ejecutar las acciones asociadas al ruteo.
    server.handleClient();
}
