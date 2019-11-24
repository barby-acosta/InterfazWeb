#include <ESP8266WebServer.h>

//AGREGADO
//#include <HC_SR04X2.h>
//#include <ControlMotor.h>



/*DEFINICIONES DE CONFIGURACION*/
#define BAUDRATE            115200

#define HC_SR04FRONT_OUT    8 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04FRONT_IN     9 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04RIGHT_OUT    10 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04RIGHT_IN     11 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04LEFT_OUT     12 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04LEFT_IN      13 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04BACK_OUT     A0 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04BACK_IN      A1 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define TAM_BUFFER          128
#define CRITIC_DISTANCE     40

/*DEFINICIONES DE TIEMPO DE EJECUCION*/
#define F_Sensed                1
#define F_Communication         3
#define F_Movement              2

/*DECLARACION DE VARIABLES GLOBALES*/
//ControlMotor control(2, 3, 7, 4, 5, 6); // MotorDer1,MotorDer2,MotorIzq1,MotorIzq2,PWM_Derecho,PWM_Izquierdo
//HC_SR04X2 sensor_distance;
//ESP8266 wifi(Serial);

uint8_t buffer[TAM_BUFFER] = {0}; //buffer para enviar y recibir informacion
uint32_t len = 0; //Tamaño del mensaje recibido o el mensaje a enviar
uint32_t frecuency = 1; //variables que nos permite implementar un porgrmacion de tareas
String data_send;


//variables de almacenamiento de distancias
float distance_front = 0;
float distance_right = 0;
float distance_left = 0;
float distance_back = 0;
//variables de desplazamiento -> el vehiculo lee estas variables y decide su movimiento
uint8_t direction = 0; // 0=stop, 1=front, 2=back //si va para adelante o para atras
uint8_t speed = 0;   // 0=stop, 1=slow, 2=fast ,3=fasta&furious
uint8_t turn = 0;    // 0=forward, 1=left, 2=right

// FIN AGREGADO

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
    Serial.begin(BAUDRATE);
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

    //AGREGADO
    pinMode(HC_SR04BACK_OUT, OUTPUT);
    pinMode(HC_SR04BACK_OUT, INPUT);
    //sensor_distance.FrontSetup(HC_SR04FRONT_OUT, HC_SR04FRONT_IN);
    //sensor_distance.RightSetup(HC_SR04RIGHT_OUT, HC_SR04RIGHT_IN);
    //sensor_distance.LeftSetup(HC_SR04LEFT_OUT, HC_SR04LEFT_IN);
    //sensor_distance.BackSetup(HC_SR04BACK_OUT, HC_SR04BACK_IN);
    //FIN AGREGADO
}
 
void loop(){
  //3-función que se encarga de recibir las peticiones de los clientes
  //y ejecutar las acciones asociadas al ruteo.
  server.handleClient();
}
