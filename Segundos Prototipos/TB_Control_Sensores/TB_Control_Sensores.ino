
/*
   Sistema de telemetría para Taq Balam
   Este programa tiene el objetivo de ser base para trabajar cuando ya se tenga módulo de recepción y transmisión de datos
   Lo que contiene es para trabajar con sensor MPU6065 y módulo de obstáculos para obtener inclinación de carro y velocidad.
   Tambien tendrá un prototipo para mandar datos a una interfaz de matlab y recibir para modificar algunos parametros.
*/
#include <TaqBalam.h>

const float num_pin_rue = 4; // numero de pines de rueda
const float dia_rue = 0.6;// diametro de rueda en metros
const float dia_sen = 0.1; // diametro de circulo de sensor


float vel_ang; // velocidad angular
float vel_rueda;  // velocidad de la rueda
int contador; // contador para calcular cuantas vueltas da la rueda
const char pin = 32;// GPIO32 a este pin estará conectado el sensor
volatile int interruptCounter; // Variables de la interrupcion del timer
int totalInterruptCounter;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() { // interrupcion del timer para obtener vueltas por segundo 
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

}

void IRAM_ATTR ISR() { // interrupcion del sensor, cada vez que se activa
  contador++;
}

void setup() {
  
  pinMode(pin, INPUT); // configurar pin de entrada para sensor
  attachInterrupt(pin, ISR, HIGH);// Se configura la interrupcion para que se active cada vez que se activa el sensor
  
  Serial.begin(115200);

  timer = timerBegin(0, 80, true); // algunas configuraciones para ajustar el timer a 1 seg
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);// interrupcion cada segundo
  timerAlarmEnable(timer);

  inicioMPU();// Iniciamos el módulo MPU6065

}

void loop() {


  mpuupdate(); /// Obtenemos una actualización de los angulos del sensor

  if (interruptCounter > 0) { // Corresponde a interrupcion del timer
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    
    // Interrupt handling code
    vel_ang = 2 * PI * (contador / num_pin_rue);//    Se realizan los calculos para obtener velocidad tangencial de la rueda.
    vel_rueda = vel_ang * dia_rue; // Tenemos m/s
    vel_rueda = vel_rueda * 3.6; //tenemos km/h ///// VARIABLE A USAR 

    contador = 0; // esto si se incluye, para reinciar contador
  }


  /// Prototipo para mandar datos a 



}
