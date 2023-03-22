#include "ADInput.h"

ADInput input;

void setup() {
  Serial.begin(9600);

  delay(1000);
  
  int* pins = new int[3]{A0, A1, A2};

  input = ADInput(3, pins, 6, 1, 0.01, 10);
  input.setPressedCallback(key);

  pinMode(5, OUTPUT);
  TCCR0B &= 0b00000000 | 0b00000001;

}

void setTone(uint8_t generator, float voltage)
{
  analogWrite(generator, voltage/5.f*255.f);
}

double roundToNearest(double v, double base)
{
  return round(v * 1/base) * base;
}

void key(int device, bool pressed)
{
  Serial.print("BUTTON ");
  Serial.print(device);
  Serial.println(pressed ? " DOWN" : " UP");
  Serial.println();

  if(pressed)
    setTone(5, ((12 - device) / 12.) * 5);
  else
    setTone(5, 5);
}

void loop() {

  input.poll();

  // double v = 0.5;
  // double target_v = roundToNearest((analogRead(A0)/1024.) * 2 - 0.5, 0.01953125);
  // double curr_v = 0;
  
  // for(int i = 0; i < 7; i++)
  // {
  //     if(((curr_v + v) - target_v) > -(curr_v + v) * 0 )
  //     {
  //         v /= 2;
  //         Serial.print('0');
  //         continue;
  //     }
      
  //     curr_v += v;
  //     v /= 2;
      
  //     Serial.print('1');
  // }

  // Serial.println();
}
