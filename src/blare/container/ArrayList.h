#pragma once

#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <algorithm>

namespace blare::container
{
	template<typename T> class ArrayList
	{
	public:
		using Iterator = T*;
	public:
		ArrayList(size_t initialCapacity = 2, float factor = 1.5f);
		ArrayList(const ArrayList& other);
		ArrayList(ArrayList&& other);
		ArrayList& operator=(const ArrayList& other);
		ArrayList& operator=(ArrayList&& other);
		~ArrayList();

		void pushBack(const T& t);
		void pushBack(T&& t);
		void popBack();

		constexpr T& at(size_t index);
		constexpr const T& at(size_t index) const;

		constexpr T& operator[](size_t index){ return m_Data[index]; }
		constexpr const T& operator[](size_t index)const { return m_Data[index]; }

		void clear();
		void compress();

		constexpr T& back() { return *(end() - 1); }
		constexpr const T& back() const { return *(end() - 1); }
		constexpr T& front() { return *begin(); }
		constexpr const T& front() const { return *begin(); }

		constexpr Iterator begin() noexcept { return &m_Data[0]; }
		constexpr Iterator end() noexcept { return &m_Data[m_Size]; }
		constexpr const Iterator begin() const noexcept { return &m_Data[0]; }
		constexpr const Iterator end() const noexcept { return &m_Data[m_Size]; }

		constexpr size_t size() const noexcept { return m_Size; }
		constexpr size_t capacity() const noexcept { return m_Capacity; }
		constexpr T* data() noexcept { return m_Data; }
		constexpr const T* data() const noexcept { return m_Data; }
		constexpr bool isEmpty() const { return m_Size == 0; }

		operator bool() const { return m_Data; }

	private:
		void reallocate(size_t capacity);
	private:
		T* m_Data; // should be first for reinterpret_cast safety
		size_t m_Size;
		float m_Factor;
		size_t m_Capacity;
	};

	template<typename T> ArrayList<T>::ArrayList(size_t initialCapacity, float factor)
		: m_Size(0), m_Factor(factor), m_Capacity(initialCapacity + 1) 
	{ 
		if (m_Factor < 1.0f) // for efficiency sake
			m_Factor = 1.2f;
		m_Data = (T*)operator new[](m_Capacity * sizeof(T)); 
	}

	template<typename T> ArrayList<T>::ArrayList(const ArrayList& other)
		: m_Size(other.m_Size), m_Factor(other.m_Factor), m_Capacity(other.m_Capacity)
	{
		m_Data = (T*)operator new[](m_Capacity * sizeof(T)); 
		for (ptrdiff_t i = 0; i < m_Size; i++)
			new((T*)m_Data + i) T(other.at(i));
	}
	template<typename T> ArrayList<T>::ArrayList(ArrayList&& other)
		: m_Size(other.m_Size), m_Factor(other.m_Factor), m_Data(other.m_Data), m_Capacity(other.m_Capacity)
	{
		other.m_Data = nullptr;
		other.m_Size = 0;
		other.m_Factor = 0.0f;
		other.m_Capacity = 0;
	}
	template<typename T> ArrayList<T>& ArrayList<T>::operator=(const ArrayList& other)
	{
		if (this == &other)
			return *this;

		this->clear();
		if (m_Capacity != other.m_Capacity)
		{
			m_Capacity = other.m_Capacity;
			this->reallocate(m_Capacity);
		}
		m_Size = other.m_Size;
		m_Factor = other.m_Factor;
		for (ptrdiff_t i = 0; i < m_Size; i++)
			new((T*)m_Data + i) T(other.at(i));


		return *this;
	}
	template<typename T> ArrayList<T>& ArrayList<T>::operator=(ArrayList&& other)
	{
		for (T& t : *this)
			t.~T();
		operator delete[](m_Data, m_Capacity * sizeof(T));
		m_Data = other.m_Data;
		m_Size = other.m_Size;
		m_Factor = other.m_Factor;
		m_Capacity = other.m_Capacity;

		other.m_Data = nullptr;
		other.m_Size = 0;
		other.m_Factor = 0.0f;
		other.m_Capacity = 0;
		return *this;
	}
	template<typename T> ArrayList<T>::~ArrayList() 
	{ 
		if (*this)
		{
			for (T& t : *this)
				t.~T();				
		}
		operator delete[](m_Data, m_Capacity * sizeof(T)); 
	}

	template<typename T> void ArrayList<T>::pushBack(const T& t)
	{
		if (m_Size >= m_Capacity)
			reallocate((size_t) (m_Capacity * static_cast<float>(m_Factor) + 1.0f));
		m_Size++;
		new(&back()) T(t);
	}
	template<typename T> void ArrayList<T>::pushBack(T&& t)
	{
		if (m_Size >= m_Capacity) 
			reallocate((size_t) (m_Capacity * static_cast<float>(m_Factor) + 1.0f));
		m_Size++;
		new(&back()) T(std::move(t));
	}
	template<typename T> void ArrayList<T>::popBack()
	{
		if (m_Size == 0)
			return;

		at(m_Size - 1).~T();
		m_Size--;
		if (m_Size <= (size_t) (m_Capacity / (m_Factor * 2.0f)))
			reallocate((size_t) (m_Capacity / (m_Factor * 2.0f)));
	}

	template<typename T> constexpr T& ArrayList<T>::at(size_t index) 
	{
		if (index >= size()) 
			throw std::out_of_range("Arraylist$at: out of bounds");
		return this->operator[](index); 
	}
	template<typename T> constexpr const T& ArrayList<T>::at(size_t index) const
	{
		if (index >= size()) 
			throw std::out_of_range("Arraylist$at: out of bounds");
		return this->operator[](index);
	}

	template<typename T> void ArrayList<T>::clear()
	{
		for (T& t : *this)
			t.~T();
		m_Size = 0;
	}
	template<typename T> void ArrayList<T>::compress() 
	{ 
		size_t capacity = m_Size == 0 ? 1 : m_Size;
		reallocate(capacity); 
	}

	template<typename T> void ArrayList<T>::reallocate(size_t capacity)
	{
		size_t oldCapacity = m_Capacity;
		m_Capacity = capacity;
		if (m_Size > m_Capacity)
			m_Size = m_Capacity;

		T* data = (T*)::operator new[](m_Capacity * sizeof(T));
		for (ptrdiff_t i = 0; i < m_Size; i++)
		{
			new((T*)data + i) T(std::move(m_Data[i]));
			m_Data[i].~T();
		}
		
		operator delete[](m_Data, oldCapacity * sizeof(T));
		m_Data = data;
	}

} // blare::container

