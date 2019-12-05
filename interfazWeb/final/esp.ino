#include <ESP8266WebServer.h>

//Setear credenciales de la red
const char *ssid = "Autito";
const char *password = "12345678";
const int wifichannel = 13;
const boolean hidden = false;
const int max_connections = 8;


//1-instancia de ESP8266WebServer
ESP8266WebServer server(80);

// mantener una referencia a los valores actuales para modificarlos
// Usuario tiene la última palabra de esta manera si falla la comunicación serial en algún momento
int direction = 0;
int speed = 0;
int turn = 0;

int sensor_front = 0;
int sensor_back = 0;
int sensor_left = 0;
int sensor_right = 0;

char intbuffer[5];
String response;

const String mainpage = "<head><style>button{width:100px;height:50px;}#status{visibility:hidden;}</style></head><body><h1>Autito Proyecto 12</h1><br/><table><tr><td><button onclick=\"send('masmarcha')\">MARCHA +</button></td><td></td><td></td><td><button onclick=\"send('adelante')\">ADELANTE</button></td><td></td></tr><tr><td><button onclick=\"send('menosmarcha')\">MARCHA -</button></td><td></td><td><button onclick=\"send('izquierda')\">IZQUIERDA</button></td><td><button onclick=\"send('frenar')\">FRENAR</button></td><td><button onclick=\"send('derecha')\">DERECHA</button></td></tr><tr><td></td><td></td><td></td><td><button onclick=\"send('atras')\">ATR&Aacute;S</button></td><td></td></tr></table><br/><h2 id=\"status\"></h2><script>let status=document.getElementById('status');setStatus=(text,color)=>{status.innerHTML=text;status.style.color=color;status.style.visibility='visible';window.setTimeout(()=>{status.style.visibility='hidden';},5000);};send=(dir)=>{let xhttp=new XMLHttpRequest();xhttp.onreadystatechange=function(){if(this.readyState==4){if(this.status==200){setStatus(dir+' OK','black');}else{setStatus(dir+' ERROR','red');}}};xhttp.open('GET','http://192.168.4.1/'+dir,true);xhttp.timeout = 5000;xhttp.send();};</script></body>";

void sendPlainText(const char *plaintext)
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", plaintext);
}

void sendJson(const char *json)
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
}

void sendCommand()
{
    Serial.write(9);
    Serial.write(direction);
    Serial.write(speed);
    Serial.write(turn);
}

//4-Funciones encargada de enviar la respuesta al cliente
void handleRoot()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/html", mainpage.c_str());
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
    ESP.eraseConfig();
    WiFi.softAP(ssid, password, wifichannel, hidden, max_connections);
    Serial.begin(9600);

    //2-Definir la asociación entre la URL de la petición y
    // la acción que se ejecutará.
    // Ruteo para URL desconocida
    server.onNotFound(handleNotFound);
    
    server.on("/", handleRoot);

    server.on("/frenar", []() {
        direction = 0;
        speed = 0;
        sendCommand();
        sendPlainText("FRENAR");
    });

    server.on("/masmarcha", []() {
        speed = speed + 1;
        if (speed > 3){
            speed = 3;
        }
        sendCommand();
        sendPlainText("MASMARCHA");
    });

    server.on("/menosmarcha", []() {
        speed = speed - 1;
        if (speed < 0){
            speed = 0;
        }
        sendCommand();
        sendPlainText("MENOSMARCHA");
    });

    server.on("/izquierda", []() {
        if(turn == 2){
            turn = 0;
        }else{
            turn = 1;
        }
        sendCommand();
        sendPlainText("IZQUIERDA");
    });

    server.on("/derecha", []() {
        if(turn == 1){
            turn = 0;
        }else{
            turn = 2;
        }
        sendCommand();
        sendPlainText("DERECHA");
    });

    server.on("/adelante", []() {
        direction = 1;
        sendCommand();
        sendPlainText("ADELANTE");
    });

    server.on("/atras", []() {
        direction = 2;
        sendCommand();
        sendPlainText("ATRAS");
    });

    server.on("/sensores", []() {
      Serial.write(8);
      while(Serial.available()<4);
      sensor_front = Serial.read();
      sensor_back = Serial.read();
      sensor_left = Serial.read();
      sensor_right = Serial.read();
      response = "{\"adelante\":" + String(sensor_front)
        + ", \"atras\":" + String(sensor_back)
        + ", \"izquierda\":" + String(sensor_left)
        + ", \"derecha\":" + String(sensor_right)
        + ", \"direccion\":" + String(direction)
        + ", \"velocidad\":" + String(speed)
        + ", \"doblando\":" + String(turn)
        + "}";
      sendJson(response.c_str());
    });

    //Iniciar servidor
    server.begin(); //Start the server
}

void loop()
{
    //3-función que se encarga de recibir las peticiones de los clientes
    //y ejecutar las acciones asociadas al ruteo.
    server.handleClient();
}
