long read_int=0;

void setup() {
//  Serial.begin(9600);
//  while(Serial.read()>0)
//  {
//    
//  }
  pinMode(LED_BUILTIN,OUTPUT);

}

void looop() {
  // put your main code here, to run repeatedly:

  while (Serial.available())
  {
      read_int=Serial.parseInt();
      Serial.print("Light Brightness: ");
      Serial.println(read_int,DEC);

      int brightness=int(read_int);
      
      analogWrite(13,brightness);
//      delay(1000);
  }
  

}
void foo(int i) {
  for(int j = 0; j < 5; j++){
  digitalWrite(13, HIGH);
  delay(i);
  digitalWrite(13, LOW);
  delay(10-i);
  }
}

void loop() {
//  // put your main code here, to run repeatedly:
//
//  while (Serial.available())
//  {
//      read_int=Serial.parseInt();
//      Serial.print("Light Brightness: ");
//      Serial.println(read_int,DEC);
//
//      int brightness=int(read_int);
//      
//      foo(13,brightness);
////      delay(1000);
//  }
  for(int i=0;i<10;i+=10)
  {
    foo(i);
    delay(1000);
  }

}