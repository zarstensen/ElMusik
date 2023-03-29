#pragma once

// int = the device that was toggled
// bool = the down state of the device, after the toggle
// void* = user data, passed when setting the callback
using KeyCallback = void(*)(int, bool, void*);

// class responsible for handling multiplexed inputs, via. the analog input pints
class ADInput
{
public:
  ADInput() {}

	/// @brief 
	/// @param analog_count 
	/// number of analog pings passed
	/// @param analog_pins
	/// the analog pins to use for input
	/// takes ownership
	/// @param analog_resolution 
	/// how many digital type switches should be assumed to be on each analog pin
	/// @param resolutoin_offset
	/// how much the starting voltage should be offset from maximum voltage / 2.
	/// @param repeat_margin
	/// how much a voltage value must diverge, before it is considered not equal to a previous voltage value
	/// @param repeat_count
	/// how many iterations a voltage must be within the repat_margin, before it is considered valid, and is polled from.
	ADInput(int analog_count, int* analog_pins, int analog_resolution, int resolution_offset, double repeat_margin, uint8_t repeat_count)
	{
		m_analog_pins = analog_pins;
		m_analog_count = analog_count;
		m_analog_resolution = analog_resolution;
		m_repeat_margin = repeat_margin;
		m_repeat_count = repeat_count;

		// create the reading base, from the resolution offset.

		m_reading_base = 0.5;

		for (int i = 0; i < analog_resolution + resolution_offset; i++)
			m_reading_base /= 2;

		m_start_reading = 0.5;

		for (int i = 0; i < resolution_offset; i++)
			m_start_reading /= 2;

		// construct bitmaps and voltage trackers.

		m_bitmap_length = (analog_count * analog_resolution + 8 - 1) / 8 + 1;

		m_down = new byte[m_bitmap_length];
		m_prev_down = new byte[m_bitmap_length];
		m_toggled = new byte[m_bitmap_length];

		memset(m_down, m_bitmap_length, 0);
		memset(m_prev_down, m_bitmap_length, 0);
		memset(m_toggled, m_bitmap_length, 0);

		m_prev_voltages = new double[m_analog_count];
		m_stable_iterations = new uint8_t[m_analog_count];

		memset(m_prev_voltages, m_analog_count * sizeof(double), 0);
		memset(m_stable_iterations, m_analog_count * sizeof(uint8_t), 0);
	}

	poll()
	{
		// store the previous down states, to be used later for detecting toggled inputs.
		memcpy(m_prev_down, m_down, m_bitmap_length);

		for(int i = 0; i < m_analog_count; i++)
		  pollPin(i, m_analog_resolution * i);


		// if the previous down state is not equal to the current down state, the input must have toggled

		for(int i = 0; i < m_analog_resolution * m_analog_count; i++)
		{
			if(getBitmap(m_prev_down, i) != getBitmap(m_down, i))
			{
			  setBitmap(m_toggled, i, true);
          
			  if(m_callback != nullptr)
        {
				  m_callback(i, isDown(i), m_user_data);
        }
			}
			else
			  setBitmap(m_toggled, i, false);
		}
	}

	/// @brief 
	/// checks whether the digital switch is down, at the given bitmap position.
	/// @param device 
	/// bitmap index of the down state
	bool isDown(int device)
	{
		return getBitmap(m_down, device);
	}

	/// @brief same as isDown, but for its toggled state.
	bool isToggled(int device)
	{
		return getBitmap(m_toggled, device);
	}

	/// @brief sets a callback, that will be called, any time a device is toggled.
	/// @param callback 
	void setKeyCallback(KeyCallback callback, void* user_data = nullptr)
	{
		m_callback = callback;
    m_user_data = user_data;
	}

	/// @brief prints the current down bitmap, to Serial.
	/// 0 = up
	/// 1 = down
	void printDown()
	{
		printBitmap(m_down);
	}

	void printToggled()
	{
		printBitmap(m_toggled);
	}

protected:

	// round the double to the nearest double, at the given resolution
	// example:
	// roundToNearest(25.2, .5) = 25.0
	// roundToNearest(25.3, .5) = 25.5
	double roundToNearest(double v, double base)
	{
		return round(v * 1 / base) * base;
	}

	void pollPin(int pin_indx, int bitmap_offset)
	{
		// update down state

		double target_val = roundToNearest((analogRead(m_analog_pins[pin_indx]) / 1024. - 0.5) * 2, m_reading_base);

		// only poll down state, if the target val has been stable for x iterations

		if(abs(target_val - m_prev_voltages[pin_indx]) < m_repeat_margin)
		  m_stable_iterations[pin_indx]++;
		else
		{
		  m_stable_iterations[pin_indx] = 0;
		  m_prev_voltages[pin_indx] = target_val;
		}

		if(m_stable_iterations[pin_indx] >= m_repeat_count)
		  m_stable_iterations[pin_indx] = m_repeat_count;
		else
		  return;

		// attempt to figure out which switches are on,
		// by assuming each switch contributes exactly half of the previous switch.
		// the figure out which switches are turned on,
		// by adding the amount of voltage, each switch will have contributed to the final voltage reading,
		// and checking if it matches the read voltage

		// step size controls how much the approximated value will be incremented
		double step_size = m_start_reading;

		// this stores the value that will approximate the current voltage reading.
		double summed_val = 0;

		int bitmap_indx = 0;

		// only continue as long as the step size, is greater than the specified resolution.

		while (step_size >= m_reading_base)
		{

			int off_indx = bitmap_offset + bitmap_indx;

			// if the current voltage approximation plus the step size is greater than the target value.
			// then the current switch cannot be turned on.

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

	// prints a bitmap, with assumed size m_analog_count * m_analog_resolution

	void printBitmap(byte* bitmap)
	{
		char* buffer = new char[m_analog_count * m_analog_resolution + 1];

		for(int i = 0; i < m_analog_count * m_analog_resolution; i++)
			buffer[i] = getBitmap(bitmap, i) ? '1' : '0';

		buffer[m_analog_count * m_analog_resolution] = 0;
    
		Serial.println(buffer);

		delete[] buffer;
	}

	// sets a bit, to value, in the given bitmap, at the given bit index.
	void setBitmap(byte* bitmap, int indx, bool value)
	{
		bitmap[indx / 8] = (bitmap[indx / 8] & ~(1UL << indx % 8)) | (value << indx % 8);
	}

	// retrives a bit state, at the given bit index.
	bool getBitmap(byte* bitmap, int indx)
	{
		return ((bitmap[indx / 8]) >> (indx % 8)) & 1UL;
	}

	int* m_analog_pins;
	int m_analog_count;
	int m_analog_resolution;
	uint8_t m_repeat_count;
	double m_repeat_margin;
	size_t m_bitmap_length;
	double m_reading_base;
	double m_start_reading;

	KeyCallback m_callback = nullptr;
  void* m_user_data = nullptr;

	byte* m_toggled;
	byte* m_down;
	byte* m_prev_down;
	double* m_prev_voltages;
	uint8_t* m_stable_iterations;
};
