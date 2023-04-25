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
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(9600);

  delay(2000);

  Serial.println("BEGIN");
  Serial.println(sizeof(short));
  
  int* pins = new int[3]{A0, A1, A2};
  int* music_pins = new int[1]{5};

  player = MusicPlayer(music_pins, 1);

  int* analog_pins = new int[3]{A0, A1, A2};

  input = ADInput(3, analog_pins, 6, 1, 0.01, 10);

  recorder = Recorder(13, 13, 14, 37, 15, 16, 17);
  
  recorder.setupCallback(input);

  pinMode(5, OUTPUT);

  recorder.begin();

  

  // SortedList<RNote> slist = SortedList<RNote>(1, [](const RNote& a, const RNote& b) -> long { return a.time - b.time; });

  // slist.add(RNote{67237, 0});
  // slist.add(RNote{67348, 0});

  // for(int i = 0; i < slist.size(); i++)
  // {
  //   Serial.print(slist[i].device);
  //   Serial.print(':');
  //   Serial.println(slist[i].time);
  // }

}

void loop() {
  input.poll();
  recorder.loop();

  digitalWrite(13, recorder.displayBeat());

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
