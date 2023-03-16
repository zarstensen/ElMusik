


void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  TCCR0B &= 0b00000000 | 0b00000001;
  
}

void loop() {
  delay(1000*32);
  analogWrite(5, 255);
  delay(1000*32);
  analogWrite(5, 127.5);
}
