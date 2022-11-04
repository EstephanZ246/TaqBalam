#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>


long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;


uint8_t broadcastAddress[] = {0x94, 0xB9, 0x7E, 0xE5, 0x93, 0x7C}; // 94:B9:7E:E5:93:7C   94:B9:7E:E5:93:7C

int enviar;



void setup() {

  //pines I

  WiFi.mode(WIFI_STA);

  //Serial.println();
  //Serial.println(WiFi.macAddress());

  Serial.begin(115200);
  Wire.begin();
  setupMPU();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // register peer
  esp_now_peer_info_t peerInfo;

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }



  //esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &x, sizeof(int));



}

void loop() {


  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));


  recordAccelRegisters();
  recordGyroRegisters();
  printData();
  delay(100);


}

void setupMPU() {
  Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
  Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4)
  Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5)
  Wire.write(0b00000000); //Setting the accel to +/- 2g
  Wire.endTransmission();
}

void recordAccelRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40)
  while (Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processAccelData();
}

void processAccelData() {
  gForceX = accelX / 16384.0;
  gForceY = accelY / 16384.0;
  gForceZ = accelZ / 16384.0;
}

void recordGyroRegisters() {
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Gyro Registers (43 - 48)
  while (Wire.available() < 6);
  gyroX = Wire.read() << 8 | Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read() << 8 | Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read() << 8 | Wire.read(); //Store last two bytes into accelZ
  processGyroData();
}

void processGyroData() {
  rotX = gyroX / 131.0;
  rotY = gyroY / 131.0;
  rotZ = gyroZ / 131.0;
}

void printData() {
 /* Serial.print("Gyro (deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);
  Serial.print(" Accel (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);*/

  /*enviar = 'G';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'X';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));*/
  enviar = 1;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  Serial.println("Se supone que se mando");
  /*enviar = 'Y';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = rotY;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'Z';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = rotZ;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'A';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'X';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = gForceX;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'Y';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = gForceY;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'Z';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = gForceZ;
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));
  enviar = 'H';
  esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(int));*/

}
