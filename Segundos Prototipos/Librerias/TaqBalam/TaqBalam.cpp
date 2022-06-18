#include "TaqBalam.h"
#include <Wire.h>// LIBRERIA PARA PODER USAR EL I2C
#include "BluetoothSerial.h"
#include "Stdlib.h"
#include <EEPROM.h>



float gyroXoffset, gyroYoffset, gyroZoffset;  ///variables para los ca
float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;
float angleGyroX, angleGyroY, angleGyroZ,
      angleAccX, angleAccY, angleAccZ;

float angleX, angleY, angleZ;
float roll, pitch, yaw;

unsigned long preintervalo1 = millis();
float intervalo1 = 0;

// BANDERA PARA CARGAR DATOS NUEVOS
bool bandera_cargar = false;

// Bluetooth

BluetoothSerial ESPBLU;

char datos_recibidos[20];// arreglo guardar datos recibidos
float datos_convertidos[10];// Guardar convertidos
float datos_revertidos[10];// Guardar convertidos
bool bandera_mandar_datos = false; // mandar datos
bool bandera_recibir_datos = false;  // recibir datos
char temporal; // temporal para guardar datos
uint8_t contadornum = 0; // contador sirve para datos_recibidos
uint8_t contadorindex = 0;// contador datos_convertidos


void inicioMPU() {
  Wire.begin(); // iniciamos comunicacion I2C
  MPUwrite(0x19, 0x00);//MPU6050_SMPLRT_DIV
  MPUwrite(0x1a, 0x00);//MPU6050_CONFIG
  MPUwrite(0x1b, 0x08);//MPU6050_GYRO_CONFIG
  MPUwrite(0x1c, 0x00);//MPU6050_ACCEL_CONFIG
  MPUwrite(0x6b, 0x01);//MPU6050_PWR_MGMT_1
  //mpuupdate();
  //angleGyroX = 0;
  //angleGyroY = 0;
  preintervalo1 = millis();
  calcoffset(true);

}

void MPUwrite(byte reg, byte datas) {
  Wire.beginTransmission(0x68);//MPU6050_ADDR
  Wire.write(reg);
  Wire.write(datas);
  Wire.endTransmission();
}

byte MPUread(byte reg) {
  Wire.beginTransmission(0x68);//MPU6050_ADDR
  Wire.write(reg);
  Wire.endTransmission(true);
  Wire.requestFrom(0x68, 1);
  byte datas = Wire.read();
  return datas;
}

void calcoffset(bool console) {
  float x = 0, y = 0, z = 0;
  int16_t rx, ry, rz;

  delay(1000);

  if (console) {
   /* Serial.println();
    Serial.println("========================================");
    Serial.println("CALCULANDO OFFSETS");
    Serial.print("NO MOVER MPU6050");*/
  }

  for (int i = 0; i < 3000; i++) {
    if (console && i % 1000 == 0) {
      Serial.print(".");
    }
  }

  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom((int)0x68, 6);
  rx = Wire.read() << 8 | Wire.read();
  ry = Wire.read() << 8 | Wire.read();
  rz = Wire.read() << 8 | Wire.read();

  x += ((float)rx) / 65.5;
  y += ((float)ry) / 65.5;
  z += ((float)rz) / 65.5;

  gyroXoffset = x / 3000;
  gyroYoffset = y / 3000;
  gyroZoffset = z / 3000;

  if (console) {
   /* Serial.println();
    Serial.println("CALCULADOS!");
    Serial.print("X : "); Serial.println(gyroXoffset);
    Serial.print("Y : "); Serial.println(gyroYoffset);
    Serial.print("Z : "); Serial.println(gyroZoffset);
    Serial.println("EL PROGRAMA DE INICIA EN 3 SEGUNDOS");
    Serial.print("========================================");*/
    delay(3000);
  }

}

void mpuupdate() {
  int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
          rawGyroX, rawGyroY, rawGyroZ;

  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom((int)0x68, 14);

  rawAccX = Wire.read() << 8 | Wire.read();
  rawAccY = Wire.read() << 8 | Wire.read();
  rawAccZ = Wire.read() << 8 | Wire.read();
  rawTemp = Wire.read() << 8 | Wire.read();
  rawGyroX = Wire.read() << 8 | Wire.read();
  rawGyroY = Wire.read() << 8 | Wire.read();
  rawGyroZ = Wire.read() << 8 | Wire.read();

  temp = (rawTemp + 12412.0) / 340.0;

  accX = ((float)rawAccX) / 16384;
  accY = ((float)rawAccY) / 16384;
  accZ = ((float)rawAccZ) / 16384;

  angleAccX = atan2(accY, sqrt(accZ * accZ + accX * accX)) * 360 / 2.0 / PI;
  angleAccY = atan2(accX, sqrt(accZ * accZ + accY * accY)) * 360 / -2.0 / PI;

  gyroX = ((float)rawGyroX) / 65.5;
  gyroY = ((float)rawGyroY) / 65.5;
  gyroZ = ((float)rawGyroZ) / 65.5;

  gyroX -= gyroXoffset;
  gyroY -= gyroYoffset;
  gyroZ -= gyroZoffset;

  float accCoef = 0.02f;
  float gyroCoef = 0.98f;

  intervalo1 = (millis() - preintervalo1) * 0.001;

  angleGyroX += gyroX * intervalo1;
  angleGyroY += gyroY * intervalo1;
  angleGyroZ += gyroZ * intervalo1;

  if (angleGyroZ> 360){
    angleGyroZ = 0;
  }else if (angleGyroZ<0){
    angleGyroZ = 360;
  } 

  angleX = (gyroCoef * (angleX + gyroX * intervalo1)) + (accCoef * angleAccX);
  angleY = (gyroCoef * (angleY + gyroY * intervalo1)) + (accCoef * angleAccY);
  angleZ = angleGyroZ;

  preintervalo1 = millis();


  roll = angleX;
  pitch = angleY;
  yaw = angleZ;

}


void sendArray(int *palabra){ // Al final de cada array debe ir '\0'
int tamano = sizeof(palabra);
int contador= 0;
Serial.print('A');
Serial.print('\n');

    for (int i = 0; i<tamano; i++){
        Serial.print(*palabra);
        //Serial.write('\n');
        //Serial.print(",");
        Serial.print('\n');
        palabra++; 
    }

Serial.print('B');
Serial.print('\n');
          
  }

void conf_bluetooth(int BaudRate) {
ESPBLU.begin(BaudRate);
  
  // Limpiamos arreglos
  limpiararreglos();

}

void recibirdatos() {
  // if (ESPBLU.read() > 0) {
  temporal = ESPBLU.read();
  //}

  if (temporal == 'M') {
    bandera_mandar_datos = true;
  } 
  if (temporal == 'O') {
    EEPROM.get(0, datos_revertidos[0]);
    EEPROM.get(4, datos_revertidos[1]);
    EEPROM.get(8, datos_revertidos[2]);
    EEPROM.get(12, datos_revertidos[3]);
    EEPROM.get(16, datos_revertidos[4]);
    EEPROM.get(20, datos_revertidos[5]);
    EEPROM.get(24, datos_revertidos[6]);
    EEPROM.get(28, datos_revertidos[7]);
    EEPROM.get(32, datos_revertidos[8]);
    EEPROM.get(36, datos_revertidos[9]);
    bandera_cargar = true;
  }

  ///////////// Recibir datos y guardarlos/////////////////////////////////
  if (temporal == 'N') {
    bandera_recibir_datos = true;
    //limpiararreglos();
    contadorindex = 0;
    contadornum = 0;
    temporal = ESPBLU.read();

  } else if (temporal == ',') {
    datos_convertidos[contadorindex] = atof(datos_recibidos);//datos_recibidos.toFloat();
    contadorindex++;
    contadornum = 0;
    temporal = ESPBLU.read();

    // limpiamos
    for (int i = 0; i < sizeof(datos_recibidos); i++) {
      datos_recibidos[i] = 0;
    }

  } else if (temporal == 'B') {
    datos_convertidos[contadorindex] = atof(datos_recibidos);//datos_recibidos.toFloat();
    bandera_recibir_datos = false;
    //limpiararreglos();
    contadorindex = 0;
    contadornum = 0;

    EEPROM.put(0, datos_convertidos[0]);
    EEPROM.put(4, datos_convertidos[1]);
    EEPROM.put(8, datos_convertidos[2]);
    EEPROM.put(12, datos_convertidos[3]);
    EEPROM.put(16, datos_convertidos[4]);
    EEPROM.put(20, datos_convertidos[5]);
    EEPROM.put(24, datos_convertidos[6]);
    EEPROM.put(28, datos_convertidos[7]);
    EEPROM.put(32, datos_convertidos[8]);
    EEPROM.put(36, datos_convertidos[9]);
    bandera_cargar = true;

  }

  if (bandera_recibir_datos == true) {
    datos_recibidos[contadornum] = temporal;
    contadornum++;
  }

}
void sendBLUE() {
  if (bandera_mandar_datos == true) {
    Serial.print('A');
    Serial.print(datos_convertidos[0]);
    Serial.print(',');
    Serial.print(datos_convertidos[1]);
    Serial.print(',');
    Serial.print(datos_convertidos[2]);
    Serial.print(',');
    Serial.print(datos_convertidos[3]);
    Serial.print(',');
    Serial.print(datos_convertidos[4]);
    Serial.print(',');
    Serial.print(datos_convertidos[5]);
    Serial.print(',');
    Serial.print(datos_convertidos[6]);
    Serial.print(',');
    Serial.print(datos_convertidos[7]);
    Serial.print(',');
    Serial.print(datos_convertidos[8]);
    Serial.print(',');
    Serial.print(datos_convertidos[9]);
    Serial.println('B');
    bandera_mandar_datos = false;
  }
}

void limpiararreglos(void) {
  for (int i = 0; i < sizeof(datos_recibidos) ; i++) {
    datos_recibidos[i] = 0;
  }
  for (int i = 0; i < sizeof(datos_convertidos) ; i++) {
    datos_convertidos[i] = 0.0;
  }
}

void cargardatos(){
    EEPROM.get(0, datos_revertidos[0]);
    EEPROM.get(4, datos_revertidos[1]);
    EEPROM.get(8, datos_revertidos[2]);
    EEPROM.get(12, datos_revertidos[3]);
    EEPROM.get(16, datos_revertidos[4]);
    EEPROM.get(20, datos_revertidos[5]);
    EEPROM.get(24, datos_revertidos[6]);
    EEPROM.get(28, datos_revertidos[7]);
    EEPROM.get(32, datos_revertidos[8]);
    EEPROM.get(36, datos_revertidos[9]);
  }