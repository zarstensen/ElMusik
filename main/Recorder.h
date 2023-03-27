#pragma once

#include "SortedList.h"

static void RecorderKeyCallback(int device, bool is_down, void* recorder)
{
  ((Recorder*)recorder)->keyCallback(device, is_down);
}

class Recorder
{
public:
  Recorder(ADInput& ad_input, int keyboard_range, int record_device, int clear_device, int octave_down_device, int octave_up_device)
  {
    ad_input.setCallback(RecorderKeyCallback, this);
  }

  void keyCallback(int device, bool is_down)
  {

  }


}
