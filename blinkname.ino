int led = D7;

void longBlink() {
  
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(500);
  
}

void shortBlink() {
  
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
  delay(500);
  
}

void setup() 
{
    pinMode(led, OUTPUT);
}

void loop() 
{
     //E
  shortBlink();
  
   //V
  shortBlink();
  shortBlink();
  shortBlink();
  longBlink();
  
   //E
  shortBlink();
}