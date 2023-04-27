#pragma once

#include "MusicNotes.h"


class MusicPlayer
{
public:
  MusicPlayer(){}


  MusicPlayer(int* pins, int pin_count, int data_pin, int clock_pin, int latch_pin)
      : m_pins(pins), m_pin_count(pin_count), m_data_pin(data_pin), m_clock_pin(clock_pin), m_latch_pin(latch_pin), m_active_pins(0)
  {
    pinMode(m_data_pin, OUTPUT);
    pinMode(m_clock_pin, OUTPUT);
    pinMode(m_latch_pin, OUTPUT);

    digitalWrite(m_latch_pin, LOW);
    shiftOut(m_data_pin, m_clock_pin, MSBFIRST, m_active_pins);
    digitalWrite(m_latch_pin, HIGH);
    
    for (int i = 0; i < GENERATOR_COUNT; i++)
      m_active_tones[i] = Notes::OFF;

    TCCR0B &= 0b00000001;
  }

  void test()
  {
    /*for (int i = 0; i < 36; i++)
    {
      setTone(true, (Notes)i);
      delay(64ULL*1000ULL);
      setTone(false, (Notes)i);
    }
    setTone(false, Notes::C3);*/
    setTone(true, Notes::C4);


  }


  void setTone(bool on, Notes note)
  {
    if (on)
    {
      if (getGeneratorIndex(note) != -1)
        return;
              
      int8_t generator_index = getAvailableGeneratorIndex();
      if (generator_index == -1)
        return;

      Serial.print("turned on tone: ");
      Serial.print((int)note);
      Serial.print(" in generator: ");
      Serial.println(generator_index);
      m_active_tones[generator_index] = note;
      m_active_pins |= 1 << generator_index;        
      analogWrite(m_pins[generator_index], (NOTES_TO_VOLT[(int)note])/5.f*255.f);
    }  
    else
    {
      int8_t generator_index = getGeneratorIndex(note);
      if (generator_index == -1)
        return;

      Serial.print("turned off tone: ");
      Serial.print((int)note);
      Serial.print(" in generator: ");
      Serial.println(generator_index);

      m_active_tones[generator_index] = Notes::OFF;
      m_active_pins &= ~(1 << generator_index);
    }
    Serial.print("active pins: ");
    Serial.println(m_active_pins);
    digitalWrite(m_latch_pin, LOW);
    shiftOut(m_data_pin, m_clock_pin, LSBFIRST, ~m_active_pins);
    digitalWrite(m_latch_pin, HIGH);
  }
    

protected:

    int m_data_pin;
    int m_clock_pin;
    int m_latch_pin;

    int* m_pins;
    int m_pin_count;

    byte m_active_pins;  

    static constexpr int GENERATOR_COUNT = 2;
    Notes m_active_tones[GENERATOR_COUNT];
    

    int8_t getGeneratorIndex(Notes note)
    {
      Serial.print("Searching for note: ");
      Serial.println((int)note);
      for (int i = 0; i < GENERATOR_COUNT; i++)
        if (m_active_tones[i] == note)
          return i;
      return -1;
    }

    int8_t getAvailableGeneratorIndex()
    {
      return getGeneratorIndex(Notes::OFF);
    }
    
};