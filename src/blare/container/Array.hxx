#pragma once

#include <cstddef>
#include <stdexcept>
#include <type_traits>

#ifdef NDEBUG
	#define INDEXED_ACCESS_GUARD(i, sz, expression)
#else 
	#define INDEXED_ACCESS_GUARD(i, sz, expression) if (i >= sz) expression
#endif

namespace blare::container
{
	template<typename T, std::size_t N> struct ArrayTraits
	{
		typedef T array_type[N];
		using value_type = T;
		using is_swappable = std::is_swappable<value_type>;
		using is_nothrow_swappable = std::is_nothrow_swappable<value_type>;

		static constexpr value_type& ref(const array_type& ref, std::size_t n) noexcept
		{
			return const_cast<value_type&>(ref[n]);
		}

		static constexpr value_type* ptr(const array_type& ref) noexcept
		{
			return const_cast<value_type*>(ref);
		}
	};

	template<typename T> struct ArrayTraits<T, 0>
	{
		struct array_type {};
		using value_type = T;
		using is_swappable = std::true_type;
		using is_nothrow_swappable = std::true_type;

		static constexpr value_type& ref(const array_type& ref, std::size_t n) noexcept
		{
			return *static_cast<value_type*>(nullptr);
		}

		static constexpr value_type* ptr(const array_type& ref) noexcept
		{
			return nullptr;
		}
	};

	template<typename T, std::size_t N> class Array
	{
	public:
		using value_type = T;
		using pointer = value_type*;
		using const_pointer = const value_type*;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using size_type = std::size_t;
		using diff_type = std::ptrdiff_t;
		using traits_type = ArrayTraits<value_type, N>;
	public:
		Array() = default;
		Array(const Array& other) = default;
		Array(Array&& other) = default;
		Array& operator=(const Array& other) = default;
		Array& operator=(Array&& other) = default;
		~Array() = default;

		constexpr iterator begin() noexcept { return &m_Data[0]; }
		constexpr const_iterator begin() const noexcept { return &m_Data[0]; }
		constexpr iterator end() noexcept { return &m_Data[size()]; }
		constexpr const_iterator end() const noexcept { return &m_Data[size()]; }

		constexpr reverse_iterator reverseBegin() noexcept { return reverse_iterator(begin()); }
		constexpr const_reverse_iterator reverseBegin() const noexcept { return const_reverse_iterator(begin()); }
		constexpr reverse_iterator reverseEnd() noexcept { return reverse_iterator(end()); }
		constexpr const_reverse_iterator reverseEnd() const noexcept { return const_reverse_iterator(end()); }
	
		constexpr reference front() { return *begin(); }
		constexpr const_reference front() const { return *begin(); }
		constexpr reference back() { return *(end() - 1); }
		constexpr const_reference back() const { return *(end() - 1); }

		constexpr reference operator[](size_t index) { return traits_type::ref(m_Data, index); }
		constexpr const_reference operator[](size_t index) const { return const_cast<const_reference>(traits_type::ref(m_Data, index)); }

		constexpr reference at(size_t index)
		{
			INDEXED_ACCESS_GUARD(index, size(), throw std::out_of_range("Array.at() out of range"));
			return operator[](index);
		}
		constexpr const_reference at(size_t index) const
		{
			INDEXED_ACCESS_GUARD(index, size(), throw std::out_of_range("Array.at() out of range"));
			return operator[](index);
		}

		constexpr void swap(Array& other) noexcept(traits_type::is_nothrow_swappable::value)
		{
			std::swap_ranges(begin(), end(), other.begin());
		}

		constexpr size_type size() const noexcept { return N; }
		constexpr size_type capacity() const noexcept { return N; }
		constexpr pointer data() noexcept { return &m_Data[0]; }
		constexpr const_pointer data() const noexcept { return &m_Data[0]; }
		constexpr bool isEmpty() { return false; }
	private:
		value_type m_Data[N];
	};

	template<typename T, std::size_t N> bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<typename T, std::size_t N> bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename T, std::size_t N> bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename T, std::size_t N> bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return rhs < lhs;
	}

	template<typename T, std::size_t N> bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return !(lhs > rhs);
	}

	template<typename T, std::size_t N> bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs)
	{
		return !(lhs < rhs);
	}
}
