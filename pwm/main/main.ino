


void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  TCCR0B &= 0b00000000 | 0b00000001;
  setTone(5, 3.57);
  
}


// enum class Notes : float
// {

// }

void loop() {
  // delay(2000 * 32);
  // setTone(5, 4.4696);
  // delay(2000 * 32);
  // setTone(5, 2.9393);
}

void setTone(uint8_t generator, float voltage)
{
  analogWrite(generator, voltage/5.f*255.f);
}
