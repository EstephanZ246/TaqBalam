


union BtoF
{
byte b[16];
float  fval;  
} u;

const int buffer_size = 16;
byte buf[buffer_size];




float prueba = 0;


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
 prueba = 0;
  for(int i = 0 ; i < 1000; i++){
    delay(500);
    
    Serial.print('V');
    WritetoMatlab(random(1000));
    Serial.print('\n');
  }

}

float ReadMatlab()
{
  int reln = Serial.readBytesUntil('\n',buf,buffer_size);
  for (int i = 0; i<buffer_size; i++)
  {
  u.b[i] = buf[i];
  }
  float output = u.fval;
  return output;
}


void WritetoMatlab(float number){
  byte *b = (byte *) &number;
  Serial.write(b,4);
  Serial.write(10);
  
}
