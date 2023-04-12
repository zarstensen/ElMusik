#pragma once

#include "MusicNotes.h"


class MusicPlayer
{
public:
  MusicPlayer(){}


    MusicPlayer(int* pins, int pin_count)
        : m_pins(pins), m_pin_count(pin_count)
    {
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
        setTone(5, better_notes[i]);
      }
        delay(32* 1000);

      }
    }

    void multiTest()
    {
      setTone(11, Notes::C4);
      setTone(10, Notes::E4);
      setTone(9, Notes::G4);
      setTone(5, Notes::C3);
      setTone(3, Notes::C3);
    }    

    

protected:

    int* m_pins;
    int m_pin_count;

    void setTone(uint8_t generator, Notes voltage)
    {
        analogWrite(generator, ((float)voltage / 1000.0)/5.f*255.f);
    }
};