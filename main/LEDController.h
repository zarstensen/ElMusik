#pragma once

class LEDController
{
public:
  static void setup(int data_pin, int latch_pin, int clock_pin)
  {
    s_data_pin = data_pin;
    s_latch_pin = latch_pin;
    s_clock_pin = clock_pin;
    pinMode(s_data_pin, OUTPUT);
    pinMode(s_clock_pin, OUTPUT);
    pinMode(s_latch_pin, OUTPUT);
  }

  static void displayDigits(byte octave, byte BMP) 
  {
    digitalWrite(s_latch_pin, LOW);
    shiftOut(s_data_pin, s_clock_pin, MSBFIRST, ((BMP % 10) << 4) | BMP / 10 );
    shiftOut(s_data_pin, s_clock_pin, MSBFIRST, octave);
    digitalWrite(s_latch_pin, HIGH);
  }
protected:
  static inline int s_data_pin = -1;
  static inline int s_clock_pin = -1;
  static inline int s_latch_pin = -1;
};