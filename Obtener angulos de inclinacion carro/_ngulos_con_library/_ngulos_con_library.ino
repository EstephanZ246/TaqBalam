/*
   Proyecto Dispositivo de telemetria para Taq Balam Universidad del Valle de Guatemala
   Equipo Electrico
   Esta programacion es para obtener lo angulos de inclinacion del carro, asi como angulos de giro con MPU6065

*/

#include "BluetoothSerial.h" // Librería para utilizar bluetooth del ESP32

BluetoothSerial SerialBT;

#include <Wire.h>// LIBRERIA PARA PODER USAR EL I2C

float gyroXoffset, gyroYoffset, gyroZoffset;  ///variables para los ca

float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;

float angleGyroX, angleGyroY, angleGyroZ,
      angleAccX, angleAccY, angleAccZ;
float angleX, angleY, angleZ;

float acelx, acely, acelz = 0; // valores aceleracion para trabajar con velocidades
float gx, gy , gz = 0;
float pitch, roll, yaw = 0;

float vfx, vox, acelax = 0;

float historialacel[100];
int contador = 0;
int promediohist;
int sumahist;
int veltemp; // temporalllll
int temporals1;

unsigned long tiempoahora1 = 0;
int intervalo2 = 1;

unsigned long preintervalo1 = millis();
float intervalo1 = 0;


////////////////////////////////////////// PROTOTIPO DE FUNCIONES /////////////////////////////////////////////////////
void calcoffset();
void mpuupdate();
void inicioMPU();


void setup() {
  Serial.begin(115200);// INICIALIAMOS EL UART CON 115200 BAUDIOS
  Serial2.begin(115200);// INICIAMOS UART MODULO 2 PARA
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Wire.begin();// INICIALIZAMOS COMUNICACION I2C
  inicioMPU();// INICIO DEL SENSOR MPU6050
  calcoffset(true); // CALCULAMOS OFFSET SER ACELEROMETRO

}

void loop() {


  mpuupdate(); // SE ACTUALIZAN LOS VALORES QUE PROVIENEN DEL SENSOR :)

  if (millis() >= intervalo2 + tiempoahora1) {
    tiempoahora1 = millis();

    //////////////////////////////////////// AJUSTES A ACELERACION SIN LA COMPONENTE DE GRAVEDAD////////////////////////////////////////////////////////////////////////////////////////

    acelx = accX * 9.80665F;
    acely = accY * 9.80665F;
    acelz = accZ * 9.80665F;

    roll = angleX; // angulos de inclinacion y giro del carro
    pitch = angleY;
    yaw = angleZ;

    gx = 9.80665F * sin(pitch * 0.01745329252);
    gy = -9.80665F * sin(roll * 0.01745329252);

    acelx = (acelx + gx); // aceleracion sin la componente de gravedad 
    acely = (acely + gy);

    ///////////////////////////////////////////////////////////// CALCULO VELOCIDADES///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    SerialBT.print(" vel acum:");
    SerialBT.print(vfx);
    SerialBT.print(" Acel:");
    SerialBT.print(acelx);
    SerialBT.print(" Vel inst:  ");
    SerialBT.println(veltemp);
    
    Serial.print(" Vel acum:");
    Serial.print(vfx);
    Serial.print("  Acel:");
    Serial.print(acelx);
    Serial.print(" Vel inst: ");
    Serial.println(veltemp);

    /*
      float historialacel[100];
      int contador = 0;
    */
    //////////////////////////////////////////// historial de aceleraciones para compararlas luego
    /*historialacel[contador] = acelx;
    contador++;

    if (contador == 100 - 1) { // contador para poder tener el index del historial
      contador = 0;
    }*/
    /////////////////////////////////////////////
    

/*
    temporals1++;

    if (temporals1 >= 1000) {
      veltemp = acelx * 3.6;
      temporals1 = 0;
    }


    if (acelx >= 0.1) {

      if (vfx < 0) {
        vfx = 0;
      }
      ////////// Mantenemos velocidad ////////////////////////////////////////// Este fue un intento de obtener la velocidad junto con la aplicacion de "filtro"(No es útil) 
      if ((acelx < (historialacel[contador - 1] + 0.5)) and (acelx > (historialacel[contador - 1] - 0.5))) {
        vfx = vox;
        vox = vfx;
      }

      if (acelx < (historialacel[contador - 1] - 0.5)) {
        vfx = vox - ((acelx - historialacel[contador - 1]) * intervalo2 * 0.001 * 3.6);  // *3.6 para km/h
        vox = vfx;
      }
      if (acelx > (historialacel[contador - 1] + 0.5)) {
        vfx = vox + (acelx * intervalo2 * 0.001 * 3.6);  // *3.6 para km/h
        vox = vfx;
      }





    }*/



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




  }



}

void inicioMPU() {
  MPUwrite(0x19, 0x00);//MPU6050_SMPLRT_DIV
  MPUwrite(0x1a, 0x00);//MPU6050_CONFIG
  MPUwrite(0x1b, 0x08);//MPU6050_GYRO_CONFIG
  MPUwrite(0x1c, 0x00);//MPU6050_ACCEL_CONFIG
  MPUwrite(0x6b, 0x01);//MPU6050_PWR_MGMT_1
  //mpuupdate();
  //angleGyroX = 0;
  //angleGyroY = 0;
  preintervalo1 = millis();

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
    Serial.println();
    Serial.println("========================================");
    Serial.println("CALCULANDO OFFSETS");
    Serial.print("NO MOVER MPU6050");
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
    Serial.println();
    Serial.println("CALCULADOS!");
    Serial.print("X : "); Serial.println(gyroXoffset);
    Serial.print("Y : "); Serial.println(gyroYoffset);
    Serial.print("Z : "); Serial.println(gyroZoffset);
    Serial.println("EL PROGRAMA DE INICIA EN 3 SEGUNDOS");
    Serial.print("========================================");
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

  angleX = (gyroCoef * (angleX + gyroX * intervalo1)) + (accCoef * angleAccX);
  angleY = (gyroCoef * (angleY + gyroY * intervalo1)) + (accCoef * angleAccY);
  angleZ = angleGyroZ;

  preintervalo1 = millis();

}
