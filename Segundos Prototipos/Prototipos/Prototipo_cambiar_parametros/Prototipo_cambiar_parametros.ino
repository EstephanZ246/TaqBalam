#include "BluetoothSerial.h"
BluetoothSerial ESPBLU;



uint8_t datos_recibidos[10];
float datos_convertidos[10];
bool bandera_mandar_datos = false;
bool bandera_recibir_datos = false;


void setup() {
  // put your setup code here, to run once:


}

void loop() {
  // put your main code here, to run repeatedly:
  conf_bluetooth(115200);


}


void conf_bluetooth(int BaudRate) {
  ESPBLU.begin(BaudRate);

  // Limpiamos arreglos
  for (int i = 0; i < sizeof(datos_recibidos) + 1; i++) {
    datos_recibidos[i] = 0;
  }
  for (int i = 0; i < sizeof(datos_convertidos) + 1; i++) {
    datos_convertidos[i] = 0.0;
  }

}

void recibirdatos() {
  char temporal = ESPBLU.read();

  if (temporal == 'M') {
    bandera_mandar_datos = true;
  } else if (temporal == 'O') {
    bandera_mandar_datos = false;
  }

  
///////////// Recibir datos y guardarlos
  if (temporal == 'N'){
    
  }else if (temporal == ','){
    
  }else if (temporal == 'B'){
    
  }

  if (bandera_recibir_datos == true){
    
  }
////////////////////////////////////////////

  
/////////////////// Guardamos valores en EEPROM

//////////////////////////////////////////////
  
}


void sendBLUE(int *palabra) { // Al final de cada array debe ir '\0'
  if (bandera_mandar_datos == true) {

    int tamano = sizeof(palabra);
    int contador = 0;
    Serial.print('A');
    //Serial.print('\n');

    for (int i = 0; i < tamano; i++) {
      Serial.print(*palabra);
      //Serial.write('\n');
      Serial.print(",");
      // Serial.print('\n');
      palabra++;
    }

    Serial.print('B');
    Serial.print('\n');

  }
}
