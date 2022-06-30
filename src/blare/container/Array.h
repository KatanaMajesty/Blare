#pragma once

#include <cstddef>

namespace blare::container
{
	template<typename T, size_t N> class Array
	{
	public:
		Array();
		Array(const Array& other);
		Array(Array&& other);
		Array& operator=(const Array& other);
		Array& operator=(Array&& other);
		~Array() = default;
	private:
		T m_Data[N];
	};
}
