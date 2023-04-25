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

// class responsible for recording input from an ADInput instance.
// assumes the following device layout:
// [KEYBOARD_DEVICE_0], [KEYBOARD_DEVICE_1], ..., [KEYBOARD_DEVICE_N], [RECORD_DEVICE], [CLEAR_DEVICE], [OCTAVE_DOWN_DEVICE], [OCTAVE_UP_DEVICE]
class Recorder
{
public:
  // key callback, subscribed to the ADInputs key callback.
  // the user data is equal to the current Recorder instance.
  static void s_KeyCallback(int device, bool is_down, void* recorder)
  {
    ((Recorder*)recorder)->keyCallback(device, is_down);
  }
  
  Recorder() {}

  /// @brief 
  /// @param keyboard_range 
  /// how many notes the keyboard consists of.
  /// @param record_device 
  /// ADInput device index, for toggeling recording (should not be a part of the keyboard range)
  /// @param clear_device 
  /// ADInput device index, for clearing recording (should not be a part of the keyboard range)
  /// 
  /// @param note_range 
  /// how many total unique notes can be recorded.
  /// 
  /// @param octave_down_device
  /// ADInput device index, for shifting the keyboards notes one octave down.
  ///  
  /// @param octave_up_device 
  /// ADInput device index, for shifting the keyboards notes one octave up.
  ///
  /// @param toggle_bpm_device
  /// ADInput device index, for toggeling between predefined BPM values.
  /// 
  Recorder(int keyboard_range, int record_device, int clear_device, int note_range, int octave_down_device, int octave_up_device, int toggle_bpm_device, int bpm = 120)
  : m_keyboard_range(keyboard_range), m_record_device(record_device), m_clear_device(clear_device), m_note_range(note_range), m_octave_down_device(octave_down_device), m_octave_up_device(octave_up_device), m_toggle_bpm_device(toggle_bpm_device), m_bpm(bpm)
  {
    Serial.println(getBpm());
    Serial.println(beatDuration());
  }

  bool isRecording() { return m_is_recording; }
  bool hasRecording() { return m_recording_span == -1;}
  
  int getBpm() { return m_bpm; }

  int beatNumber() 
  {
      return measureTime(millis() - m_recording_start) / beatDuration();
  }

  bool displayBeat()
  {
    unsigned long time = millis();
    if(hasRecording())
      time -= m_recording_start;

    return (measureTime(time) % beatDuration()) < m_beat_display_interval;
  }

  /// @brief sets the bpm
  /// 
  /// if currently recording, this will do nothing.
  /// 
  /// @return whether the bpm was changed.
  bool setBpm(int new_bpm)
  {
      if (isRecording())
          return false;

      m_bpm = new_bpm;

      return true;
  }

  /// @brief begins listening for key events
  /// not the same as starting to record, this is handles by the record_device.
  void begin()
  {
    m_recording.clear(1);
    m_recording_span = -1;
    m_recording_start = 0;
    m_prev_time = millis();
    m_is_recording = false;  
  }

  /// @brief setsup the key callback from the given ADInput
  /// this controls which ADInput instance, the recorder will be recording from.
  void setupCallback(ADInput& ad_input)
  {
    ad_input.setKeyCallback(s_KeyCallback, this);
  }

  /// @brief should only be called by an ADInput instance.
  /// @param device 
  /// @param is_down 
  void keyCallback(int device, bool is_down)
  {
    // if the device is inside the keyboard range, record the key event.
    if(device < m_keyboard_range)
    {
      int note = device + m_octave_offset;

      if(m_is_recording)
      {
        // a recorded note must store the note value itself, and the timestamp of which it has been recorded.
        // this is relative to the recording start.
        RNote recorded_note = RNote { (uint32_t)(millis() - m_recording_start), note, is_down };

        // if the recorder is overdubbing, the timestamp must roll over, if it is greater than the recording span.
        if(m_recording_span != -1)
          recorded_note.time %= m_recording_span;

        // m_recording is a sorted list, which uses the timestamp as the comparing value.
        m_recording.add(recorded_note);
      }
      else
        // if not recording, simply do not store the key, but still send the key event, to the output device.
        sendNote(note, is_down);
    }

    // if not within the keyboard range, figure out which function the device has, and call the corresponding method.

    if(device == m_record_device && is_down)
      requestRecordingToggle();

    if(device == m_clear_device && is_down)
      begin();

    if(device == m_octave_down_device && is_down)
      offsetOctave(-1);

    if(device == m_octave_up_device && is_down)
      offsetOctave(1);

    if(device == m_toggle_bpm_device && is_down)
      toggleBpm();

  }

  /// @brief Should be called every loop
  /// responsible for sending stored notes to the output device.
  void loop()
  {
    unsigned long current_time = millis() - m_recording_start;

    if (m_recording_toggle_requested)
    {
        toggleRecording(current_time);
        m_recording_toggle_requested = false;
    }


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

  void requestRecordingToggle()
  {
      m_recording_toggle_requested = true;
  }

  /// @brief toggles the recording.
  /// check isRecording, to see the state of recording.
  void toggleRecording(unsigned long time)
  {
    Serial.println("TOGGLE");

    if(!m_is_recording)
    {
      if(m_recording_span == -1)
        m_recording_start = millis();
    }

    if (m_is_recording)
        if (m_recording_span == -1)
            m_recording_span = time;

    m_is_recording = !m_is_recording;
  }

  void sendNote(int device, bool is_down)
  {
    // TODO: send note code
  }

  /// @brief 
  /// offset the keyboards note range by [offset] octaves.
  /// positive = upwards shift
  /// negative = downwards shift.
  void offsetOctave(int offset)
  {
    m_octave_offset += 12 * offset;

    m_octave_offset = min(max(m_octave_offset, 0), m_note_range - m_keyboard_range);

  }

  void toggleBpm()
  {
    int new_bpm_indx = (m_current_bpm_indx + 1) % (sizeof(BPMS) / sizeof(int));
    Serial.println(new_bpm_indx);

    if(setBpm(BPMS[new_bpm_indx]))
      m_current_bpm_indx = new_bpm_indx;
  }

protected:

    /// @brief gets the duration the recorder has been inside the current measure.
    /// @return milliseconds
    long measureTime(long millis)
    {
        return millis % measureDuration();
    }

    /// @brief gets the duration in milliseconds, of a single measure.
    long measureDuration() { return beatDuration() * m_beats_pr_measure;  }

    unsigned long beatDuration() { return ((60ULL * 1000ULL) / ((unsigned long)getBpm())) * 64; }

  int m_keyboard_range;
  int m_record_device;
  int m_clear_device;
  int m_octave_down_device;
  int m_octave_up_device;
  int m_toggle_bpm_device;
  int m_note_range;
  int m_octave_offset;

  int m_bpm;

  int m_beats_pr_measure = 4;

  long m_recording_span = -1;
  unsigned long m_recording_start = 0;
  unsigned long m_prev_time = millis();
  unsigned long m_beat_display_interval = 100 * 64;

  size_t m_recording_indx = 0;

  bool m_is_recording = false;
  bool m_recording_toggle_requested = false;

  int BPMS[5] = { 60, 80, 120, 140, 200 };
  int m_current_bpm_indx = 0;

  SortedList<RNote> m_recording = SortedList<RNote>([](const RNote& a, const RNote& b) -> long { return a.time - b.time; });
};
