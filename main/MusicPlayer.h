#pragma once

#include "MusicNotes.h"


class MusicPlayer
{
public:
  MusicPlayer(){}


    MusicPlayer(int* pins, int pin_count, int data_pin, int clock_pin, int latch_pin)
        : m_pins(pins), m_pin_count(pin_count), m_data_pin(data_pin), m_clock_pin(clock_pin), m_latch_pin(latch_pin)
    {
      m_active_pins = 0;

      pinMode(m_data_pin, OUTPUT);
      pinMode(m_clock_pin, OUTPUT);
      pinMode(m_latch_pin, OUTPUT);

      memset(m_active_pins, false, pin_count);
      digitalWrite(m_latch_pin, LOW);
      shiftOut(m_data_pin, m_clock_pin, MSBFIRST, m_active_pins);
      digitalWrite(m_latch_pin, HIGH);
      
      TCCR0B &= 0b00000000 | 0b00000001;
    }

    void test()
    {
      Notes* notes = new Notes[37] {Notes::C3, Notes::CS3, Notes::D3, Notes::DS3, Notes::E3, Notes::F3, Notes::FS3, Notes::G3, Notes::GS3, Notes::A3, Notes::AS3, Notes::B3, Notes::C4, Notes::CS4, Notes::D4, Notes::DS4, Notes::E4, Notes::F4, Notes::FS4, Notes::G4, Notes::GS4, Notes::A4, Notes::AS4, Notes::B4, Notes::C5, Notes::CS5, Notes::D5, Notes::DS5, Notes::E5, Notes::F5, Notes::FS5, Notes::G5, Notes::GS5, Notes::A5, Notes::AS5, Notes::B5, Notes::C6};
      Notes* better_notes = new Notes[8] {Notes::C5, Notes::D5, Notes::E5, Notes::F5, Notes::G5, Notes::A5, Notes::B5, Notes::C6};
      while (true)
      {
      for (int i = 0; i < 8; i++)
      {
        delay(32* 2500);
        setTone(0, better_notes[i]);
      }
        delay(32* 1000);

      }
    }

    void multiTest()
    {
      setTone(0, Notes::A4);
      setTone(1, Notes::C5);
      setTone(2, Notes::G5);
      setTone(3, Notes::B5);
      setTone(4, Notes::OFF);

          
  }    

    

protected:

    int m_data_pin;
    int m_clock_pin;
    int m_latch_pin;

    int* m_pins;
    int m_pin_count;

    byte m_active_pins;    

    void setTone(uint8_t generator_index, Notes voltage)
    {
      if (voltage != Notes::OFF)
      {
        m_active_pins |= 1 << generator_index;
        analogWrite(m_pins[generator_index], (NOTES_TO_VOLT[(int)voltage])/5.f*255.f);
      }  
      else
      {
        m_active_pins &= ~(1 << generator_index);
      }
      digitalWrite(m_latch_pin, LOW);
      Serial.println(m_active_pins);
      shiftOut(m_data_pin, m_clock_pin, MSBFIRST, m_active_pins);
      digitalWrite(m_latch_pin, HIGH);
    }
};