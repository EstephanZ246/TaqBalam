// TaqBalam 2022-2023
// Codigo pruebas volante

int tiempo = 0;
int horas = 1;
int mins = 10; 
int seg = 20;
char buf[32];

int temp1,temp2,temp3;
int Nivel_Bateria;
int Pedal_Derecho;
int Velocidad, RPM;

void setup() {
  Serial1.begin(9600);
  
  delay(1000);
  Serial.print("page LOGOS");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  delay(2000);
  Serial1.print("page DATOS");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(2000);
  Serial1.print("t0.txt=\"ON\"");
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void loop() {
  // put your main code here, to run repeatedly:
  tiempo = millis()/1000;
  horas = tiempo/3600;
  mins = tiempo/60;
  seg = tiempo-(horas*3600+mins*60);
  
  // mandar tiempo hacia la pantalla
  sprintf(buf, "t1.txt=\"%d:%d:%d\"", horas, mins, seg);
  Serial1.print(buf);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // temperaturas
  if(temp1>= 100){
    temp1 = 0;      
  }else{
    temp1=temp1+10;
  }
  Serial1.print("j2.val=");
  Serial1.print(temp1);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if(temp2>= 100){
    temp2 = 0;      
  }else{
    temp2=temp2+20;
  }
  Serial1.print("j3.val=");
  Serial1.print(temp2);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  if(temp3>= 100){
    temp3 = 0;      
  }else{
    temp3=temp3+15;
  }
  Serial1.print("j4.val=");
  Serial1.print(temp3);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // Bateria
  if(Nivel_Bateria>= 100){
    Nivel_Bateria = 0;      
  }else{
    Nivel_Bateria=Nivel_Bateria+20;
  }
  Serial1.print("j0.val=");
  Serial1.print(Nivel_Bateria);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // Pedal 
  if(Pedal_Derecho>= 100){
    Pedal_Derecho = 0;      
  }else{
    Pedal_Derecho = Pedal_Derecho+10;
  }
  Serial1.print("j1.val=");
  Serial1.print(Pedal_Derecho);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  // Velocidad y RPM
  if(Velocidad>= 40){
    Velocidad = 0;
    RPM = 0;      
  }else{
    Velocidad = Velocidad + 3;
    RPM = RPM+5;
  }
  Serial1.print("n0.val=");
  Serial1.print(Velocidad);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);

  Serial1.print("n1.val=");
  Serial1.print(RPM);
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
  delay(500);  
  
}
