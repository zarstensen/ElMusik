#include "ADInput.h"
#include "SortedList.h"
#include "MusicPlayer.h"

struct RNote
{
  unsigned long time;
  int device;
};

ADInput input;
MusicPlayer player;


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
  
  int* music_pins = new int[1]{5};

  player = MusicPlayer(music_pins, 1);

  int* analog_pins = new int[3]{A0, A1, A2};

  input = ADInput(3, analog_pins, 6, 1, 0.01, 10);
  input.setPressedCallback(key);

  pinMode(5, OUTPUT);

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

  player.test();
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
