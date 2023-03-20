#pragma once

using KeyCallback = void(*)(int, bool);

class ADInput
{
public:
  ADInput() {}

	/// @brief 
	/// @param analog_count 
	/// @param analog_pins
	/// takes ownership
	/// @param analog_resolution 
	ADInput(int analog_count, int* analog_pins, int analog_resolution, int resolution_offset)
	{
		m_analog_pins = analog_pins;
		m_analog_count = analog_count;
    m_analog_resolution = analog_resolution;
		
		m_reading_base = 0.5;

		for (int i = 0; i < analog_resolution + resolution_offset; i++)
			m_reading_base /= 2;

		m_start_reading = 0.5;

		for (int i = 0; i < resolution_offset; i++)
			m_start_reading /= 2;

    m_bitmap_length = (analog_count * analog_resolution + 8 - 1) / 8 + 1;

    m_down = new byte[m_bitmap_length];
    m_prev_down = new byte[m_bitmap_length];
    m_toggled = new byte[m_bitmap_length];

    memset(m_down, m_bitmap_length, 0);
    memset(m_prev_down, m_bitmap_length, 0);
    memset(m_toggled, m_bitmap_length, 0);
	}

	poll()
	{
    memcpy(m_prev_down, m_down, m_bitmap_length);

    for(int i = 0; i < m_analog_count; i++)
      pollPin(m_analog_pins[i], m_analog_resolution * i);

    for(int i = 0; i < m_analog_resolution * m_analog_count; i++)
    {
        if(getBitmap(m_prev_down, i) != getBitmap(m_down, i))
        {
          setBitmap(m_toggled, i, true);
          
          if(m_callback != nullptr)
            m_callback(i, isDown(i));
        }
        else
          setBitmap(m_toggled, i, false);
    }
	}

	bool isDown(int device)
	{
		return getBitmap(m_down, device);
	}

	bool isToggled(int device)
	{
		return getBitmap(m_toggled, device);
	}

	void setPressedCallback(KeyCallback callback)
	{
    m_callback = callback;
	}

  void printDown()
  {
    printBitmap(m_down);
  }

  void printToggled()
  {
    printBitmap(m_toggled);
  }

protected:

	double roundToNearest(double v, double base)
	{
		return round(v * 1 / base) * base;
	}

	void pollPin(int pin, int bitmap_offset)
	{
    // update down state

    // printBitmap(m_prev_down);

    // Serial.println(analogRead(pin) / 1024.);
		double target_val = roundToNearest((analogRead(pin) / 1024. - 0.5) * 2, m_reading_base);

    // Serial.println(target_val);

		double step_size = m_start_reading;

		double summed_val = 0;

		int bitmap_indx = 0;

		while (step_size >= m_reading_base)
		{

      // Serial.println(step_size);

      int off_indx = bitmap_offset + bitmap_indx;
      
      // Serial.println(summed_val + step_size);

			if ((summed_val + step_size) <= target_val)
			{
				setBitmap(m_down, off_indx, true);
				
				summed_val += step_size;
			}
			else
			{
				setBitmap(m_down, off_indx, false);
			}

			step_size /= 2;
			bitmap_indx++;
		}


	}

  void printBitmap(byte* bitmap)
  {
    char* buffer = new char[m_analog_count * m_analog_resolution + 1];

    for(int i = 0; i < m_analog_count * m_analog_resolution; i++)
      buffer[i] = getBitmap(bitmap, i) ? '1' : '0';

    buffer[m_analog_count * m_analog_resolution] = 0;
    
    Serial.println(buffer);

    delete[] buffer;
  }

	void setBitmap(byte* bitmap, int indx, bool value)
	{
		bitmap[indx / 8] = (bitmap[indx / 8] & ~(1UL << indx % 8)) | (value << indx % 8);
	}

	bool getBitmap(byte* bitmap, int indx)
	{
		return ((bitmap[indx / 8]) >> (indx % 8)) & 1UL;
	}

	int* m_analog_pins;
	int m_analog_count;
  int m_analog_resolution;
  size_t m_bitmap_length;
	double m_reading_base;
	double m_start_reading;

  KeyCallback m_callback = nullptr;

	byte* m_toggled;
	byte* m_down;
	byte* m_prev_down;
};
