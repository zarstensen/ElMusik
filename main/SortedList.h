#pragma once

template<typename T>
using Comparer = int(*)(const T& target, const T& other);

template<typename T>
class SortedList
{
public:
	SortedList(unsigned int capacity, Comparer<T> comparer_function)
		: m_capacity(capacity), m_comparer(comparer_function)
	{
    if(capacity == 0)
      Serial.println("CAPACITY MUST BE GREATER THAN 0!!!");

		m_data = new T[m_capacity];
	}

	void add(const T& data)
	{
		m_size++;

		// double the capacity of data, if there is not enough space.

		if (m_size > m_capacity)
		{
			T* larger_data = new T[m_capacity * 2];

			memcpy(larger_data, m_data, m_capacity * sizeof(T));

			T* tmp = m_data;

			m_data = larger_data;

			delete[] tmp;

			m_capacity *= 2;
		}

		insert(data);
	}

	T* data()
	{
		return m_data;
	}

	unsigned int size()
	{
		return m_size;
	}

protected:

	void insert(const T& data)
	{
		if (m_size - 1 == 0)
		{
			m_data[0] = data;
			return;
		}
		// find the target index, using binary search.

		int low_indx = 0;
		int high_indx = m_size - 1;


		while (low_indx < high_indx)
		{
			unsigned int middle_indx = (high_indx + low_indx) / 2;
			int compared = m_comparer(data, m_data[middle_indx]);

			if (compared < 0)
			{
				high_indx = middle_indx;
				//target_indx = high_indx;
			}
			else
			{
				low_indx = middle_indx + 1;
			}
		}

		unsigned int target_indx = high_indx;

		if(target_indx != m_size - 1)
			// insert the index at the given position, by first shifting everything to the left of the insert index, 1 element to the right
			memcpy(m_data + target_indx + 1, m_data + target_indx, (m_size - target_indx - 1) * sizeof(T));

		m_data[target_indx] = data;
	}

	unsigned int m_size = 0;
	unsigned int m_capacity;
	T* m_data;
	Comparer<T> m_comparer;
};
