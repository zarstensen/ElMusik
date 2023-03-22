#pragma once

#include "MusicNotes.h"


class MusicPlayer
{
public:
    musicPlayer(int* pins, int pin_count)
        : m_pins(pins), m_pin_count(pin_count)
    {
        TCCR0B &= 0b00000000 | 0b00000001;
    }

    void test()
    {
      Notes* notes = new Notes[37] {C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3, C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4, C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5, C6}

      for (int i = 0; i < 37; i++)
      {
        delay(32* 500);
        setTone(5, notes[i]);
      }
    }

    

protected:

    int* m_pins;
    int m_pin_count;

    void setTone(uint8_t generator, Notes voltage)
    {
        analogWrite(generator, (float)(voltage)/5.f*255.f);
    }
};