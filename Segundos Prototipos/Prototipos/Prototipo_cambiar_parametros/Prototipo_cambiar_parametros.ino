#include "BluetoothSerial.h"
#include "Stdlib.h"

BluetoothSerial ESPBLU;

char datos_recibidos[10];
float datos_convertidos[10];
bool bandera_mandar_datos = false;
bool bandera_recibir_datos = false;


uint8_t contadornum = 0;
uint8_t contadorindex = 0;

void setup() {

  conf_bluetooth(115200);

}

void loop() {


}

void conf_bluetooth(int BaudRate) {
  ESPBLU.begin("TB Sensor",BaudRate);

  // Limpiamos arreglos
  limpiararreglos();

}

void recibirdatos() {
  char temporal = ESPBLU.read();




  if (temporal == 'M') {
    bandera_mandar_datos = true;
  } else if (temporal == 'O') {
    bandera_mandar_datos = false;
  }


  ///////////// Recibir datos y guardarlos/////////////////////////////////
  if (temporal == 'N') {
    bandera_recibir_datos = true;
    limpiararreglos();
    contadorindex = 0;
    contadornum = 0;
    temporal = ESPBLU.read();

  } else if (temporal == ',') {
    datos_convertidos[contadorindex] = atof(datos_recibidos);//datos_recibidos.toFloat();
    contadorindex++;
    contadornum = 0;
    temporal = ESPBLU.read();

    // limpiamos
    for (int i = 0; i < sizeof(datos_recibidos) + 1; i++) {
      datos_recibidos[i] = 0;
    }

  } else if (temporal == 'B') {
    datos_convertidos[contadorindex] = atof(datos_recibidos);//datos_recibidos.toFloat();
    bandera_recibir_datos = false;
    limpiararreglos();
    contadorindex = 0;
    contadornum = 0;
    Serial.print(datos_convertidos[0]);
    Serial.print(" ");
    Serial.print(datos_convertidos[1]);
    Serial.print(" ");
    Serial.print(datos_convertidos[2]);
    Serial.print(" ");
    Serial.print(datos_convertidos[3]);
    Serial.print(" ");
    Serial.print(datos_convertidos[4]);
    Serial.print(" ");
    Serial.print(datos_convertidos[5]);
    Serial.print(" ");
    Serial.print(datos_convertidos[6]);
    Serial.print(" ");
    Serial.print(datos_convertidos[7]);
    Serial.print(" ");
    Serial.print(datos_convertidos[8]);
    Serial.print(" ");
    Serial.print(datos_convertidos[9]);
    Serial.print('\n');

  }

  if (bandera_recibir_datos == true) {
    datos_recibidos[contadornum] = temporal;
    contadornum++;
  }

  //////////////////////////////////////////////////////////////////////


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

void limpiararreglos(void) {
  for (int i = 0; i < sizeof(datos_recibidos) + 1; i++) {
    datos_recibidos[i] = 0;
  }
  for (int i = 0; i < sizeof(datos_convertidos) + 1; i++) {
    datos_convertidos[i] = 0.0;
  }
}
