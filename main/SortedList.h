#pragma once

/// @brief function signature, for the comparer function, used when sorting the datatype T, in SortedList
// < 0 = target is less than other
// == 0 = target is equal to other
// > 0 = target is greater than other.
template<typename T>
using Comparer = long(*)(const T& target, const T& other);

/// @brief contains a sorted list of elements of datatype T
/// must be reconstructed or cleared, if elements are to be removed.
template<typename T>
class SortedList
{
public:
	/// @param capacity how much initial memory to allocate
	/// @param comparer_function what function to use, when comparing elements
	SortedList(Comparer<T> comparer_function)
		: m_comparer(comparer_function), m_data(nullptr) {}
  
  ~SortedList()
  {
	delete[] m_data;
  }

  /// @brief
  /// clears the internal buffer, and allocates a new one, with the specified size.
  void clear(unsigned int new_capacity)
  {
    if(m_data != nullptr)
		delete[] m_data;

    m_size = 0;
    m_capacity = new_capacity;

    m_data = new T[m_capacity];
  }

	/// @brief adds the element into the list, whilst making sure there is enough memory to store the new element,
	/// and that it is inserted at a sorted position.
	void add(const T& data)
	{
		m_size++;

		// double the capacity of data, if there is not enough space.

		if (m_size > m_capacity)
		{
    #if DEB_SORTED_LIST
			Serial.print("CAPACITY: ");
			Serial.println(m_capacity);
    #endif
			// copy the old buffer into the new larger buffer, store the old buffer in a temporary, and delete it, after the buffers have been swapped.
     
			T* larger_data = new T[m_capacity * 2];

    #if DEB_SORTED_LIST
			Serial.print("PREV_POINTER: ");
			Serial.println((size_t)m_data);
			Serial.print("NEW_POINTER: ");
			Serial.println((size_t)larger_data);
    #endif

			 memcpy(larger_data, m_data, (m_capacity) * sizeof(T));
    #if DEB_SORTED_LIST
			Serial.print("FIRST ELEMENT OF NEW DATA: ");
			Serial.println(larger_data[0].device);
			Serial.println(larger_data[0].time);
			Serial.println("==========================");

			Serial.println((size_t)larger_data);
    #endif

			delete[] m_data;

    #if DEB_SORTED_LIST
			Serial.print("FIRST ELEMENT OF NEW DATA AFTER DELETE: ");
			Serial.println(larger_data[0].device);
			Serial.println(larger_data[0].time);
			Serial.println("==========================");
    #endif

			m_data = larger_data;
    
    #if DEB_SORTED_LIST
			Serial.println((size_t)m_data);
    #endif

			m_capacity *= 2;
		}

		// inserts data into the current memory buffer, using binary search, to find its target index.
		insert(data);
	}

	/// @brief same as data()[indx]
	T& operator [](unsigned int indx)
	{ 
		return data()[indx];
	}

	/// @brief returns the current buffer, that stored the sorted elements.
	/// may be deleted, if add() is called after it has been accessed.
	/// therefore the return value, should not be stored for long term usage.
	/// use size(), to get the size of the data array.
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
			long compared = m_comparer(data, m_data[middle_indx]);

			if (compared < 0)
			{
				high_indx = middle_indx;
			}
			else
			{
				low_indx = middle_indx + 1;
			}
		}

		// the high_indx will be the target index of the insert operation.

		// only perform the insert operation, if elements need to be moved around.
		if(high_indx + 1 != m_size)
    {
    	// insert the index at the given position, by first shifting everything to the left of the insert index, 1 element to the right
			memmove(&m_data[high_indx + 1], &m_data[high_indx], (m_size - high_indx - 1) * sizeof(T));
    }
    
		m_data[high_indx] = data;
	}

	unsigned int m_size = 0;
	unsigned int m_capacity;
	T* m_data;
	Comparer<T> m_comparer;
};
