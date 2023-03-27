#include "ADInput.h"
#include "SortedList.h"

struct RNote
{
  unsigned long time;
  int device;
};

ADInput input;


long compareInt(const int& target, const int& other)
{
    return target - other;
}

long compareRNote(const RNote& target, const RNote& other)
{
  return target.time - other.time;
}

SortedList<RNote> recording = SortedList<RNote>(100, compareRNote);


void setup() {

  Serial.begin(9600);

  delay(1000);

  Serial.println("BEGIN");
  
  int* pins = new int[3]{A0, A1, A2};

  input = ADInput(3, pins, 6, 1, 0.01, 20);
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

void key(int device, bool pressed, void* _)
{
  Serial.print("BUTTON ");
  Serial.print(device);
  Serial.println(pressed ? " DOWN" : " UP");
  Serial.println();

  if(device != 13)
  {
    RNote note = RNote { millis(), device };

    recording.add(RNote { millis(), device });
  }
  else if(pressed)
  {
    for(int i = 0; i < recording.size(); i++)
    {
      // Serial.print(recording.data()[i].time);
      // Serial.print(':');
      Serial.println(recording.data()[i].device);
    }
  }
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
