#pragma once

#include "SortedList.h"

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

struct RNote
{
  uint32_t time;
  int device;

  bool is_down;
};

class Recorder
{
public:
  static void s_KeyCallback(int device, bool is_down, void* recorder)
  {
    ((Recorder*)recorder)->keyCallback(device, is_down);
  }
  
  Recorder() {}

  Recorder(int keyboard_range, int record_device, int clear_device, int octave_down_device, int octave_up_device)
  : m_keyboard_range(keyboard_range), m_record_device(record_device), m_clear_device(clear_device), m_octave_down_device(octave_down_device), m_octave_up_device(octave_up_device)
  {
    Serial.println(sizeof(RNote));
  }

  void begin()
  {
    m_recording.clear(1);
    m_recording_span = -1;
    m_recording_start = 0;
    m_prev_time = millis();
  }

  void setupCallback(ADInput& ad_input)
  {
    ad_input.setKeyCallback(s_KeyCallback, this);
  }

  void keyCallback(int device, bool is_down)
  {
    if(device < m_keyboard_range)
    {
      if(m_is_recording)
      {
        RNote note = RNote { (uint32_t)(millis() - m_recording_start), device, is_down };

        if(m_recording_span != -1)
          note.time %= m_recording_span;

        m_recording.add(note);
      }
      else
        sendNote(device, is_down);
    }

    if(device == m_record_device && is_down)
      toggleRecording();

    if(device == m_clear_device && is_down)
      begin();

    // if(device == m_octave_down_device && is_down)
    //   offsetOctave(-1);

    // if(device == m_octave_up_device && is_down)
    //   offsetOctave(1);

  }

  // should be called every loop.
  void loop()
  {
    unsigned long current_time = millis() - m_recording_start;


    if(m_recording_span != -1)
      current_time %= m_recording_span;

    
    if(current_time < m_prev_time)
    {
      Serial.print("FREE: ");
      Serial.println(freeRam());
      m_recording_indx = 0;
    }
   while(m_recording_indx < m_recording.size() && m_recording[m_recording_indx].time < current_time)
   {
     Serial.print("DEVICE: ");
      Serial.println(m_recording[m_recording_indx].device);

      sendNote(m_recording[m_recording_indx].device, m_recording[m_recording_indx].is_down);

      m_recording_indx++;
   }

   m_prev_time = current_time;
  }


  void toggleRecording()
  {
    Serial.println("TOGGLE");

    if(!m_is_recording)
    {
      if(m_recording_span == -1)
        m_recording_start = millis();
    }

    if(m_is_recording)
      if(m_recording_span == -1)
        m_recording_span = millis() - m_recording_start;

    m_is_recording = !m_is_recording;
  }

  void sendNote(int device, bool is_down)
  {
    // TODO: send note code
  }

  // void offsetOctave(int offset) {}

  // void clearRecording()
  // {
  //   m_recording_span = -1;
  //   m_is_recording = false;
  // }

protected:

  int m_keyboard_range;
  int m_record_device;
  int m_clear_device;
  int m_octave_down_device;
  int m_octave_up_device;

  long m_recording_span = -1;
  unsigned long m_recording_start = 0;
  unsigned long m_prev_time = millis();

  size_t m_recording_indx = 0;

  bool m_is_recording = false;

  SortedList<RNote> m_recording = SortedList<RNote>([](const RNote& a, const RNote& b) -> long { return a.time - b.time; });
};
