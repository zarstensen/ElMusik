#include "ADInput.h"
<<<<<<< Updated upstream
#include "SortedList.h"
=======
#include "MusicPlayer.h"
>>>>>>> Stashed changes

ADInput input;
MusicPlayer player;

int compareInt(const int& target, const int& other)
{
    return target - other;
}

void setup() {
  Serial.begin(9600);

  delay(1000);
<<<<<<< Updated upstream

  Serial.println("BEGIN");
  
  int* pins = new int[3]{A0, A1, A2};
=======
  int* music_pins = new int[1]{5};
>>>>>>> Stashed changes

  player = MusicPlayer(music_pins, 1);

  int* analog_pins = new int[3]{A0, A1, A2};

  input = ADInput(3, analog_pins, 6, 1, 0.01, 10);
  input.setPressedCallback(key);
<<<<<<< Updated upstream

  pinMode(5, OUTPUT);
  TCCR0B &= 0b00000000 | 0b00000001;

  SortedList<int> l = SortedList<int>(0, compareInt);

  l.add(25);
  l.add(500);
  l.add(10);
  l.add(-1);

  Serial.println(l.size());

  for (int i = 0; i < l.size(); i++)
  {
    Serial.print(i);
    Serial.println(':');
    Serial.println(l.data()[i]);
  }

}

void setTone(uint8_t generator, float voltage)
{
  analogWrite(generator, voltage/5.f*255.f);
=======
  player.test();
>>>>>>> Stashed changes
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
