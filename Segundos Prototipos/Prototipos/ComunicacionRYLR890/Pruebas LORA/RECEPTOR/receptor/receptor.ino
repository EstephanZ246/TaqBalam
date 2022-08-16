int tiempo = 0;
int horas = 1;
int mins = 10;
int seg = 20;
char buf[32];

int temp1, temp2, temp3;
int Nivel_Bateria;
int Pedal_Derecho;
int Velocidad, RPM;


void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(115200);

  delay(1000);
  Serial.print("page LOGOS");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(2000);
  Serial.print("page DATOS");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(2000);
  Serial.print("t0.txt=\"ON\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);


}

void loop()
{

  tiempo = millis() / 1000;
  horas = tiempo / 3600;
  mins = tiempo / 60;
  seg = tiempo - (horas * 3600 + mins * 60);

  //char ch;
  String inString;
 /* while (Serial2.available())
  {

    if (Serial2.available()) {
      //inString += String(char(Serial2.read()));
      if (Serial2.read() == 'B') {
        inString = Serial2.readStringUntil('Q');
        temp1 = inString.toInt();

      }
      else if (Serial2.read() == 'Q') {
        inString = Serial2.readStringUntil('W');
        temp2 = inString.toInt();

      }
      else if (Serial2.read() == 'W') {
        inString = Serial2.readStringUntil('T');
        temp3 = inString.toInt();

      }
      else if (Serial2.read() == 'T') {
        inString = Serial2.readStringUntil('Y');
        Nivel_Bateria = inString.toInt();

      }
      else if (Serial2.read() == 'Y') {
        inString = Serial2.readStringUntil('\r');
        Pedal_Derecho = inString.toInt();

      }


    }
    Serial.print("Temp1: ");
      Serial.println(temp1);
      Serial.print("Temp2: ");
      Serial.println(temp2);
      Serial.print("Temp3: ");
      Serial.println(temp3);
      Serial.print("Nivel_Bateria: ");
      Serial.println(Nivel_Bateria);
      Serial.print("Pedal_Derecho: ");
      Serial.println(Pedal_Derecho);


  }*/


  while (Serial2.available())
  {
    if(Serial2.available()){
    inString += String(char(Serial2.read()));
    }
  }

  if(inString.length()>0)
  {

    Serial.println(inString);
  
  }

  // mandar tiempo hacia la pantalla
 /* sprintf(buf, "t1.txt=\"%d:%d:%d\"", horas, mins, seg);
  Serial.print(buf);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("j2.val=");
  Serial.print(temp1);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("j3.val=");
  Serial.print(temp2);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("j4.val=");
  Serial.print(temp3);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("j0.val=");
  Serial.print(Nivel_Bateria);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("j1.val=");
  Serial.print(Pedal_Derecho);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);*/




  //delay(100);
}
