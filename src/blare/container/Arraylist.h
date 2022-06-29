#pragma once

#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <cstring>

namespace blare
{
	template<typename T> class ArrayList // replace std::allocator with blare::DefaultAllocator
	{
	public:
		using Iterator = T*;
	public:
		explicit ArrayList(size_t initialCapacity = 2, float factor = 1.5f)
			: m_Size(0), m_Factor(factor), m_Capacity(initialCapacity + 1) 
		{ 
			if (m_Factor < 1.0f) // for efficiency sake
				m_Factor = 1.2f;
			m_Data = (T*)::operator new[](m_Capacity * sizeof(T)); 
		}
		ArrayList(const ArrayList& other);
		ArrayList(ArrayList&& other);
		ArrayList& operator=(const ArrayList& other);
		ArrayList& operator=(ArrayList&& other);
		~ArrayList() { delete[] m_Data; }

		void pushBack(const T& t)
		{
			if (m_Size >= m_Capacity)
			{
				reallocate((size_t) (m_Capacity * static_cast<float>(m_Factor) + 1.0f));
			}
			m_Size++;
			back() = t;
		}
		void pushBack(T&& t)
		{
			if (m_Size >= m_Capacity) 
				reallocate((size_t) (m_Capacity * static_cast<float>(m_Factor) + 1.0f));
			m_Size++;
			back() = std::move(t);
		}
		void popBack()
		{
			if (m_Size == 0)
				return;

			m_Size--;
			if (m_Size <= (size_t) (m_Capacity / (m_Factor * 2.0f)))
				reallocate((size_t) (m_Capacity / (m_Factor * 2.0f)));
		}

		constexpr T& at(size_t index) 
		{
			if (index >= size()) 
				throw std::out_of_range("Arraylist$at: out of bounds");
			return this->operator[](index); 
		}
		constexpr const T& at(size_t index) const
		{
			if (index >= size()) 
				throw std::out_of_range("Arraylist$at: out of bounds");
			return this->operator[](index);
		}

		constexpr T& operator[](size_t index){ return m_Data[index]; }
		constexpr const T& operator[](size_t index)const { return m_Data[index]; }

		void clear()
		{
			delete[] m_Data;
			m_Size = 0;
			m_Data = new T[m_Capacity];
		}
		void compress() 
		{ 
			size_t capacity = m_Size == 0 ? 1 : m_Size;
			reallocate(capacity); 
		}

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

	private:
		void reallocate(size_t capacity)
		{
			m_Capacity = capacity;
			if (m_Size > m_Capacity)
				m_Size = m_Capacity;
			
			T* data = (T*)::operator new[](m_Capacity * sizeof(T));
			new((T*)data) T(std::move(begin(), end()));
			
			delete[] m_Data;
			m_Data = data;
		}
	private:
		T* m_Data; // should be first for reinterpret_cast safety
		size_t m_Size;
		float m_Factor;
		size_t m_Capacity;
	};
}