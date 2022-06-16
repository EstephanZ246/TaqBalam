#include<EEPROM.h>

// Variables (se podran cambiar luego)
float num_pin_rue = 4; // numero de pines de rueda (direccion 0)
float dia_rue = 0.6;// diametro de rueda en metros (direccion 4)
float dia_sen = 0.1; // diametro de circulo de sensor (direccion 8)
float conversion = 3.6; // (direccion 12)
float frecuencia_de_muestreo = 100;//100Hz
////////////////////////////

void setup() {
  Serial.begin(115200);

  EEPROM.put(0, num_pin_rue);
  EEPROM.put(4, dia_rue);
  EEPROM.put(8, dia_sen);
  EEPROM.put(12, conversion);
  EEPROM.put(16, frecuencia_de_muestreo);

  Serial.println("Valor direccion 0");
  Serial.println( EEPROM.get(0, num_pin_rue) );
  Serial.println("");

  Serial.println("Valor direccion 4: ");
  Serial.println( EEPROM.get(4, dia_rue) );
  Serial.println("");

  Serial.println("Valor direccion 8: ");
  Serial.println( EEPROM.get(8, dia_sen) );
  Serial.println("");

  Serial.println("Valor direccion 12: ");
  Serial.println( EEPROM.get(12, conversion) );
  Serial.println("");

}

void loop() {
  // put your main code here, to run repeatedly:

}
