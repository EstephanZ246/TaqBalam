int contador;

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
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop()
{

  tiempo = millis() / 1000;
  horas = tiempo / 3600;
  mins = tiempo / 60;
  seg = tiempo - (horas * 3600 + mins * 60);


  // temperaturas
  if (temp1 >= 100) {
    temp1 = 0;
  } else {
    temp1 = temp1 + 10;
  }

  if (temp2 >= 100) {
    temp2 = 0;
  } else {
    temp2 = temp2 + 20;
  }

  if (temp3 >= 100) {
    temp3 = 0;
  } else {
    temp3 = temp3 + 15;
  }

  // Bateria
  if (Nivel_Bateria >= 100) {
    Nivel_Bateria = 0;
  } else {
    Nivel_Bateria = Nivel_Bateria + 20;
  }

  // Pedal
  if (Pedal_Derecho >= 100) {
    Pedal_Derecho = 0;
  } else {
    Pedal_Derecho = Pedal_Derecho + 10;
  }

  // Velocidad y RPM
  if (Velocidad >= 40) {
    Velocidad = 0;
    RPM = 0;
  } else {
    Velocidad = Velocidad + 3;
    RPM = RPM + 5;
  }


  String temps = String(temp1) + 'Q' + String(temp2)+ 'W' + String(temp3)+ 'T' + String(Nivel_Bateria)+ 'Y' + String(Pedal_Derecho);
  String cmd = "AT+SEND=0," + String(temps.length()) + ",B" + temps + "\r";
  Serial2.println(cmd);
  delay(10);

  /*temps = String(temp2);
  cmd = "AT+SEND=0,"  + String(temps.length())  + ",Q" + temps + "\r";
  Serial2.println(cmd);
  delay(10);

  temps =  String(temp3);
  cmd = "AT+SEND=0,"  + String(temps.length()) + ",W" + temps + "\r";
  Serial2.println(cmd);
  delay(10);

  temps =  String(Nivel_Bateria);
  cmd = "AT+SEND=0,"  + String(temps.length())  + ",T" + temps + "\r";
  Serial2.println(cmd);
  delay(10);

  temps =  String(Pedal_Derecho);
  cmd = "AT+SEND=0,"   + String(temps.length())  + ",Y" + temps + "\r";
  Serial2.println(cmd);
  delay(10);*/




}
