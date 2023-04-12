#include "ADInput.h"
#include "SortedList.h"
#include "MusicPlayer.h"
#include "Recorder.h"



ADInput input;
Recorder recorder;
MusicPlayer player;

void setTone(uint8_t generator, float voltage)
{
  analogWrite(generator, voltage/5.f*255.f);
  player.test();
}


void setup() {
  Serial.begin(9600);

  delay(2000);

  Serial.println("BEGIN");
  Serial.println(sizeof(short));
  
  int* pins = new int[3]{A0, A1, A2};
  int* music_pins = new int[1]{5};

  player = MusicPlayer(music_pins, 1);

  int* analog_pins = new int[3]{A0, A1, A2};

  input = ADInput(3, analog_pins, 6, 1, 0.01, 10);

  recorder = Recorder(13, 13, 14, 15, 16);
  
  recorder.setupCallback(input);

  Serial.println("OUTER THIS: ");
  Serial.print((size_t)&recorder);

  Serial.println(sizeof(recorder));

  pinMode(5, OUTPUT);

  recorder.begin();





  player.multiTest();
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
  recorder.loop();

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
