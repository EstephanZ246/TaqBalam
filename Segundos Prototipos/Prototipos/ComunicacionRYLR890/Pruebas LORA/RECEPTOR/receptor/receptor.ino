int tiempo = 0;
int horas = 0;
int mins = 0 ;
int seg = 0;
char buf[32];

int temp1, temp2, temp3;
int Nivel_Bateria;
int Pedal_Derecho;
int Velocidad, RPM;

#define led 21

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(led, OUTPUT);

  /*delay(1000);
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
    Serial.write(0xff);*/


}

void loop()
{


  //char ch;
  String inString;

  


  

  /*while (Serial2.available())
  {
   


    char temporal1 = Serial2.read();

    if (Serial2.available()) {


      //inString += String(char(Serial2.read()));
      if (temporal1 == 'B') {
        inString = Serial2.readStringUntil(',');
        temp1 = inString.toInt();

        //Serial.println(inString);
      }

      else if (temporal1 == 'Q') {
        inString = Serial2.readStringUntil(',');
        temp2 = inString.toInt();

      }
      else if (temporal1 == 'W') {
        inString = Serial2.readStringUntil(',');
        temp3 = inString.toInt();

      }
      else if (temporal1 == 'T') {
        inString = Serial2.readStringUntil(',');
        Nivel_Bateria = inString.toInt();

      }
      else if (temporal1 == 'Y') {
        inString = Serial2.readStringUntil(',');
        Pedal_Derecho = inString.toInt();

      }
      else if (temporal1 == 'U') {
        inString = Serial2.readStringUntil(',');
        Velocidad = inString.toInt();

      }
      else if (temporal1 == 'I') {
        inString = Serial2.readStringUntil(',');
        RPM = inString.toInt();

      }
      else if (temporal1 == 'Z') {
        inString = Serial2.readStringUntil(',');
        horas = inString.toInt();

      }
      else if (temporal1 == 'O') {
        inString = Serial2.readStringUntil(',');
        mins = inString.toInt();

      }
      else if (temporal1 == 'P') {
        inString = Serial2.readStringUntil(',');
        seg = inString.toInt();

      }
}*/
    /*Serial.print(" Temp1: ");
      Serial.print(temp1);
      Serial.print(" Temp2: ");
      Serial.print(temp2);
      Serial.print(" Temp3: ");
      Serial.print(temp3);
      Serial.print(" Nivel_Bateria: ");
      Serial.print(Nivel_Bateria);
      Serial.print(" Pedal_Derecho: ");
      Serial.print(Pedal_Derecho);
      Serial.print(" Velocidad: ");
      Serial.print(Velocidad);
      Serial.print(" RPM: ");
      Serial.println(RPM);*/


    //delay(500);

  //}
  /*
    Serial.print(" Temp1: ");
    Serial.print(temp1);
    Serial.print(" Temp2: ");
    Serial.print(temp2);
    Serial.print(" Temp3: ");
    Serial.print(temp3);
    Serial.print(" Nivel_Bateria: ");
    Serial.print(Nivel_Bateria);
    Serial.print(" Pedal_Derecho: ");
    Serial.print(Pedal_Derecho);
    Serial.print(" Velocidad: ");
    Serial.print(Velocidad);
    Serial.print(" RPM: ");
    Serial.print(RPM);
    Serial.print(" HORAS: ");
    Serial.print(horas);
    Serial.print(" MIN: ");
    Serial.print(mins);
    Serial.print(" SEG: ");
    Serial.println(seg);

*/
    while (Serial2.available())
    {
    if (Serial2.available()) {
      inString += String(char(Serial2.read()));
    }
    }

    if (inString.length() > 0)
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
