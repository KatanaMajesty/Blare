#pragma once

#include <cstddef>

namespace blare::container
{
	template<typename T, size_t N> class Array
	{
	public:
		using Iterator = T*;
	public:
		Array();
		Array(const Array& other);
		Array(Array&& other);
		Array& operator=(const Array& other);
		Array& operator=(Array&& other);
		~Array() = default;

		constexpr Iterator begin() noexcept { return &m_Data[0]; }
		constexpr const Iterator begin() const noexcept { return &m_Data[0]; }
		constexpr Iterator end() noexcept { return &m_Data[N - 1]; }
		constexpr const Iterator end() const noexcept { return &m_Data[N - 1]; }
	

	private:
		T m_Data[N];
	};
}
