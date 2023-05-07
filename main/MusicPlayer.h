#pragma once

#include "MusicNotes.h"


class MusicPlayer
{
public:
  MusicPlayer(){}


  MusicPlayer(int* pins, int pin_count, int data_pin, int latch_pin, int clock_pin)
      : m_pins(pins), m_pin_count(pin_count), m_data_pin(data_pin), m_clock_pin(clock_pin), m_latch_pin(latch_pin), m_active_pins(0)
  {
    // sets the ardunios clock speed for PWM output to the fastest
    TCCR0B &= 0b00000001;
    pinMode(m_data_pin, OUTPUT);
    pinMode(m_clock_pin, OUTPUT);
    pinMode(m_latch_pin, OUTPUT);

    // clears generators just to be safe
    clearGenerators();
  }

  // turns off all generators 
  void clearGenerators()
  {
    // makes all generators play lowest tone and changes the tracker array
    for (int i = 0; i < GENERATOR_COUNT; i++)
    {
      m_active_tones[i] = Notes::OFF;
      analogWrite(m_pins[i], 255);
    }

    // disable all generators and shift it out to the shift register
    m_active_pins = 0;
    digitalWrite(m_latch_pin, LOW);
    shiftOut(m_data_pin, m_clock_pin, MSBFIRST, ~m_active_pins);
    digitalWrite(m_latch_pin, HIGH);
  }

  // either starts a tone on the first available generator, or stops a currently playing tone from playing
  void setTone(bool on, Notes note)
  {
    // if the tone is supposed to start playing
    if (on)
    {
      // checks if the tone is already being played
      if (getGeneratorIndex(note) != -1)
        return;
      
      // finds the first available generator
      int8_t generator_index = getAvailableGeneratorIndex();
      
      // checks if there were no available generators
      if (generator_index == -1)
        return;

      // tracks the tone now being played
      m_active_tones[generator_index] = note;

      // changes the corresponding bit to enable the generator
      m_active_pins |= 1 << generator_index;        

      // analog write to the correct pin with the correct PWM value
      analogWrite(m_pins[generator_index], NOTES_TO_PWMS[generator_index][(int)note]);
    }  
    // if the tone is supposed to stop playing
    else
    {
      
      // searches for a generator playing the specified tone
      int8_t generator_index = getGeneratorIndex(note);
      // checks if no generator had the tone
      if (generator_index == -1)
        return;

      // sets the frequency to the lowest possible
      analogWrite(m_pins[generator_index], 255);

      // changes the current tone of the generator to off
      m_active_tones[generator_index] = Notes::OFF;

      // changes the corresponding bit to disable the generator
      m_active_pins &= ~(1 << generator_index);
    }

    // shift out the enabled generators to the shift register
    digitalWrite(m_latch_pin, LOW);
    shiftOut(m_data_pin, m_clock_pin, LSBFIRST, ~m_active_pins );
    digitalWrite(m_latch_pin, HIGH);
  }
    

protected:
    // pins for shift register
    int m_data_pin;
    int m_clock_pin;
    int m_latch_pin;

    // PWM pins and count
    int* m_pins;
    int m_pin_count;

    // byte to control generators
    byte m_active_pins;  

    // array to track generators notes
    static constexpr int GENERATOR_COUNT = 5;
    Notes m_active_tones[GENERATOR_COUNT];
  
    // finds the first generator playing the specified tone
    // returns -1 if no generator was playing the tone
    int8_t getGeneratorIndex(Notes note)
    {
      for (int i = 0; i < GENERATOR_COUNT; i++)
        if (m_active_tones[i] == note)
          return i;
      return -1;
    }

    // finds first generator not playing anything
    int8_t getAvailableGeneratorIndex()
    {
      return getGeneratorIndex(Notes::OFF);
    }
    
};