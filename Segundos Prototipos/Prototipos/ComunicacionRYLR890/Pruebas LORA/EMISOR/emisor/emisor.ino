int contador;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop()
{
  delay(1000);
  contador++;
  if (contador >= 2000) {
    contador = 0;
  }
  Serial.println(contador);

  String contval = String(contador);
  String cmd = "AT+SEND=0," + String(contval.length()) + "," + String(contval) + "\r";

  
  //Serial.println("AT+SEND=0,3,val");
  
  Serial2.println(cmd);

  
  while (Serial2.available()) {
    Serial.write(Serial2.read());
  }
  Serial.println();
  Serial.println(cmd);
  delay(50);
}
