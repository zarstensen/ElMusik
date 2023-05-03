#include "ADInput.h"
#include "SortedList.h"
#include "MusicPlayer.h"
#include "Recorder.h"
#include "LEDController.h"



ADInput input;
Recorder recorder;
MusicPlayer player;

void setup() {
  Serial.begin(9600);

  delay(1000);
  
  LEDController::setup(11, 13, 12);
  int* music_pins = new int[5]{6, 5, 3, 10, 9};

  player = MusicPlayer(music_pins, 5, 4, 7, 8);

  int* analog_pins = new int[6]{A0, A1, A2, A3, A4, A5};

  input = ADInput(6, analog_pins, 3, 1, 0.05, 10);

  recorder = Recorder(13, 15, 16, 49, 13, 14, 17);
  
  recorder.setupCallback(input);

  pinMode(2, OUTPUT);

  recorder.music_player = &player;

  recorder.begin();
}


double roundToNearest(double v, double base)
{
  return round(v * 1/base) * base;
}



void loop() {
  
  input.poll();
  recorder.loop();
  LEDController::displayDigits(recorder.getOctave(), recorder.getBpm()/10);


digitalWrite(2, recorder.displayBeat());

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
