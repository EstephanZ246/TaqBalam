void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);

 
}
 
void loop()
{
  //char ch;
  String inString;
  while (Serial2.available())
  {
    if(Serial2.available()){
    inString += String(char(Serial2.read()));
    }
  }
  if(inString.length()>0)
  {
    //Serial.println(inString);
    String potval;
    potval= inString.substring(9,12);
    Serial.println(potval);
  
  }
  //delay(100);
}
