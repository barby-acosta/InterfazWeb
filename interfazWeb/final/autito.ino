#include <HC_SR04X2.h>
#include <ControlMotor.h>

/*DEFINICIONES DE CONFIGURACION*/
#define BAUDRATE            9600 //115200

#define HC_SR04FRONT_OUT    8 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04FRONT_IN     9 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04RIGHT_OUT    10 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04RIGHT_IN     11 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04LEFT_OUT     12 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04LEFT_IN      13 // Numero de pin por el cual se recibe informacion de la medioción (echo)

#define HC_SR04BACK_OUT     A0 // Numero de pin por el cual se envia el pulso de START (trig)
#define HC_SR04BACK_IN      A1 // Numero de pin por el cual se recibe informacion de la medioción (echo)

//#define TAM_BUFFER          128
#define CRITIC_DISTANCE     40

/*DEFINICIONES DE TIEMPO DE EJECUCION*/
#define F_Sensed          1
#define F_Movement        2
#define F_Communication   3

/*DECLARACION DE VARIABLES GLOBALES*/
ControlMotor control(2, 3, 7, 4, 5, 6); // MotorDer1,MotorDer2,MotorIzq1,MotorIzq2,PWM_Derecho,PWM_Izquierdo
HC_SR04X2 sensor_distance;

uint32_t frecuency = 1; //variables que nos permite implementar un porgrmacion de tareas
int data; //Initialized variable to store recieved data

//variables de almacenamiento de distancias
float distance_front = 0;
float distance_right = 0;
float distance_left = 0;
float distance_back = 0;

//variables de desplazamiento -> el vehiculo lee estas variables y decide su movimiento
uint8_t direction = 0; // 0=stop, 1=front, 2=back //si va para adelante o para atras
uint8_t speed = 0;   // 0=stop, 1=slow, 2=fast ,3=fasta&furious
uint8_t turn = 0;    // 0=forward, 1=left, 2=right

void setup(void)
{
  Serial.begin(BAUDRATE);
  
  delay(3000);
  pinMode(HC_SR04BACK_OUT, OUTPUT);
  pinMode(HC_SR04BACK_OUT, INPUT);
  sensor_distance.FrontSetup(HC_SR04FRONT_OUT, HC_SR04FRONT_IN);
  sensor_distance.RightSetup(HC_SR04RIGHT_OUT, HC_SR04RIGHT_IN);
  sensor_distance.LeftSetup(HC_SR04LEFT_OUT, HC_SR04LEFT_IN);
  sensor_distance.BackSetup(HC_SR04BACK_OUT, HC_SR04BACK_IN);
}

void loop(void)
{
  // MEDICION DE DISTANCIA Y FRENO DE EMEGENCIA
  /*Si es tiempo de medir la distancia*/
  if (frecuency ==  F_Sensed) {
    //enviar los valores instantaneos de los sensores
    distance_front = sensor_distance.FrontMeasurement();
    distance_right = sensor_distance.RightMeasurement();
    distance_left = sensor_distance.LeftMeasurement();
    distance_back = sensor_distance.BackMeasurement();

    //Si voy para adelante y hay poca distancia
    if (direction == 1) { //REVISAR SI NO MOLESTA PARA DOBLAR
      if (distance_front <= CRITIC_DISTANCE) {
        control.parar();
        direction = 0;
        speed = 0;
      }
      if (distance_right <= CRITIC_DISTANCE) {
        control.parar();
        direction = 0;
        speed = 0;
      }
      if (distance_left <= CRITIC_DISTANCE) {
        control.parar();
        direction = 0;
        speed = 0;
      }
    } else {
      //Si voy para atrás y queda poca distancia
      if (distance_back <= CRITIC_DISTANCE && direction == 2) {
        control.parar();
        direction = 0;
        speed = 0;
      }

    }
  }
  // FIN MEDICION DE DISTANCIA Y FRENO DE EMEGENCIA

  // DEZPLAZAMIENTO
  if (frecuency == F_Movement) {
    //Si debo avanzar
    if ( direction == 1 && speed != 0  ) {
      /*Si hay que simplemente ir derecho*/
      if ( turn == 0) {
        control.avanzar(speed);
      }
      //Si hay que girar a la izquierda
      if ( turn == 1) {
        control.girarIzquierda();
      }
      //Si hay que girar a la derecha
      if ( turn == 2) {
        control.girarDerecha();
      }
    }
    //Si debo ir para atrás
    if ( direction == 2 && speed != 0  ) {
      control.retroceder();
    }

    //Si debo frenar
    if (direction == 0 || speed == 0) {
      control.parar();
    }
  }
  // FIN DEZPLAZAMIENTO

  // COMUNICACION  
  if ((frecuency == F_Communication)&&(Serial.available()>0)) { //SI es momento de procesar la comunicacion y el ESP envió datos
    data = Serial.read(); //Read the serial data and store it
    if (data == 9) { //SI SE RECIBE ORDEN
      //REcibí un mensaje de desplazamiento, almacenamiento de los movimientos que decidió el usuario
      direction = Serial.read();// - 48; //se resta 48, pues el valro que se recibe está en ascci
      speed = Serial.read();// - 48;
      turn = Serial.read();// - 48;
    }
    if (data == 8) { //SI PIDE INFORMACION
      Serial.write((int)distance_front);
      Serial.write((int)distance_back);
      Serial.write((int)distance_left);
      Serial.write((int)distance_right);
    }
  }
  // FIN COMUNICACION

  // CONFIGURACION DE TIEMPOS
  delay(200); //espera entre iteraciones
  frecuency++;//Aumento de la iteracion realizada
  if (frecuency == 4) {
    frecuency = 1;
  }
  // FIN CONFIGURACION DE TIEMPOS
}
