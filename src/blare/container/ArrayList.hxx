#pragma once

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <type_traits>
#include <memory>

// To be moved away
#ifdef NDEBUG
	#define INDEXED_ACCESS_GUARD(i, sz, expression)
#else 
	#define INDEXED_ACCESS_GUARD(i, sz, expression) if (i >= sz) expression
#endif

namespace blare::container
{
	template<class Allocator> class AllocatorTraits : std::allocator_traits<Allocator>
	{
	public:
		using allocator_type = Allocator;
		using base_type = std::allocator_traits<allocator_type>;
		using value_type = typename base_type::value_type;
		using pointer = typename base_type::pointer;
		using const_pointer = typename base_type::const_pointer;
		using size_type = typename base_type::size_type;
    	using difference_type = typename base_type::difference_type;
		using reference = value_type&;
		using const_reference = const value_type&;
		using base_type::allocate;
		using base_type::deallocate;
		using base_type::construct;
		using base_type::destroy;
		using base_type::max_size;
	private:
		template<typename If, typename Else> struct impl_and : std::conditional_t<Else::value, If, Else> {};
		template<typename Ptr> using is_custom_pointer = impl_and<std::is_same<pointer, Ptr>, std::bool_constant<!bool(std::is_pointer<Ptr>::value)>>;
	public:
		// overload construct for non-standard pointer types
		template<typename Pointer, typename... Args> static constexpr std::enable_if_t<is_custom_pointer<Pointer>::value> 
		construct(allocator_type& a, Pointer p, Args&&... args)
		{
			base_type::construct(a, std::to_address(p), std::forward<Args>(args)...);
		}

		// overload destroy for non-standard pointer types
		template<typename Pointer> static constexpr std::enable_if_t<is_custom_pointer<Pointer>::value>
		destroy(allocator_type& a, Pointer p)
		{
			base_type::destroy(a, std::to_address(p));
		}

		template<typename T> struct [[deprecated("Since C++20")]] rebind
		{ 
			typedef typename base_type::rebind_alloc<T> other; 
		};
	};

	template<typename T, class Allocator = std::allocator<T>> class ArrayList
	{
	public:
		using allocator_traits = AllocatorTraits<Allocator>;
		using value_type = typename allocator_traits::value_type;
		using pointer = typename allocator_traits::pointer;
		using const_pointer = typename allocator_traits::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = pointer;
		using const_iterator = const_pointer;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using size_type = typename allocator_traits::size_type;
		using difference_type = typename allocator_traits::difference_type;
	public:
		ArrayList(size_type initialCapacity = 2, float factor = 1.5f)
			: m_Size(0), m_Factor(factor), m_Capacity(initialCapacity + 1) 
		{ 
			if (m_Factor < 1.0f)
				m_Factor = 1.0f;
			m_Data = allocator_traits::allocate(m_Alloc, m_Capacity);
		}
		ArrayList(const ArrayList& other)
			: m_Size(other.m_Size), m_Factor(other.m_Factor), m_Capacity(other.m_Capacity)
		{
			m_Data = allocator_traits::allocate(m_Alloc, m_Capacity);
			for (difference_type i = 0; i < m_Size; i++)
				allocator_traits::construct(m_Alloc, m_Data + i, other.at(i));
		}
		ArrayList(ArrayList&& other)
			: m_Size(other.m_Size), m_Factor(other.m_Factor), m_Data(other.m_Data), m_Capacity(other.m_Capacity)
		{
			other.m_Data = nullptr;
			other.m_Size = 0;
			other.m_Factor = 0.0f;
			other.m_Capacity = 0;
		}
		ArrayList& operator=(const ArrayList& other)
		{
			if (this == &other)
				return *this;

			clear();
			if (m_Capacity != other.m_Capacity)
			{
				m_Capacity = other.m_Capacity;
				reallocate(m_Capacity);
			}
			m_Size = other.m_Size;
			m_Factor = other.m_Factor;
			for (difference_type i = 0; i < m_Size; i++)
				allocator_traits::construct(m_Alloc, m_Data + i, other.at(i));

			return *this;
		}
		ArrayList& operator=(ArrayList&& other)
		{
			if (this == &other)
				return *this;

			for (value_type& val : *this)
				allocator_traits::destroy(m_Alloc, &val);
			allocator_traits::deallocate(m_Alloc, m_Data, m_Capacity);
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
		~ArrayList()
		{ 
			if (bool(*this))
				for (value_type& val : *this)
					allocator_traits::destroy(m_Alloc, &val);	
			allocator_traits::deallocate(m_Alloc, m_Data, m_Capacity);
		}

		void pushBack(const value_type& val)
		{
			if (m_Size >= m_Capacity)
				reallocate(static_cast<size_type>(m_Capacity * m_Factor + 1.0f));
			m_Size++;
			allocator_traits::construct(m_Alloc, &back(), val);
		}
		void pushBack(value_type&& val)
			{
			if (m_Size >= m_Capacity) 
				reallocate(static_cast<size_type>(m_Capacity * m_Factor + 1.0f));
			m_Size++;
			allocator_traits::construct(m_Alloc, &back(), std::move(val));
		}
		void popBack()
		{
			if (m_Size == 0)
				return;

			allocator_traits::destroy(m_Alloc, &back());
			m_Size--;
			if (size() <= static_cast<size_type>(m_Capacity / (m_Factor * 2.0f)))
				reallocate(static_cast<size_type>(m_Capacity / (m_Factor * 2.0f)));
		}

		constexpr reference at(size_type index)
		{
			INDEXED_ACCESS_GUARD(index, size(), throw std::out_of_range("Arraylist$at: out of bounds"))
			return operator[](index);
		}
		constexpr const_reference at(size_t index) const
		{
			INDEXED_ACCESS_GUARD(index, size(), throw std::out_of_range("Arraylist$at: out of bounds"))
			return operator[](index);
		}

		constexpr reference operator[](size_type index){ return m_Data[index]; }
		constexpr const_reference operator[](size_type index)const { return m_Data[index]; }

		void clear()
		{
			for (value_type& val : *this)
				allocator_traits::destroy(m_Alloc, &val);
			m_Size = 0;
		}
		void compress()
		{ 
			size_type capacity = m_Size == 0 ? 1 : m_Size;
			reallocate(capacity); 
		}

		constexpr reference back() 
		{ 
			return *(end() - 1); 
		}
		constexpr const_reference back() const 
		{ 
			return *(end() - 1); 
		}
		constexpr reference front() 
		{ 
			return *begin(); 
		}
		constexpr const_reference front() const 
		{ 
			return *begin(); 
		}

		constexpr iterator begin() noexcept 
		{ 
			return &m_Data[0]; 
		}
		constexpr iterator end() noexcept 
		{ 
			return &m_Data[m_Size]; 
		}
		constexpr const_iterator begin() const noexcept 
		{ 
			return &m_Data[0]; 
		}
		constexpr const_iterator end() const noexcept 
		{ 
			return &m_Data[m_Size]; 
		}

		constexpr reverse_iterator reverseBegin() noexcept 
		{ 
			return reverse_iterator(begin()); 
		}
		constexpr reverse_iterator reverseEnd() noexcept 
		{ 
			return reverse_iterator(end()); 
		}
		constexpr const_reverse_iterator reverseBegin() const noexcept 
		{ 
			return const_reverse_iterator(begin()); 
		}
		constexpr const_reverse_iterator reverseEnd() const noexcept 
		{ 
			return const_reverse_iterator(end()); 
		}

		constexpr size_type size() const noexcept 
		{ 
			return m_Size; 
		}
		constexpr size_type capacity() const noexcept 
		{ 
			return m_Capacity; 
		}
		constexpr pointer data() noexcept 
		{ 
			return m_Data; 
		}
		constexpr const_pointer data() const noexcept 
		{ 
			return m_Data; 
		}
		constexpr bool isEmpty() const noexcept 
		{ 
			return m_Size == 0; 
		}

		constexpr operator bool() const noexcept 
		{ 
			return m_Data; 
		}

	private:
		void reallocate(size_type capacity)
		{
			size_type oldCapacity = m_Capacity;
			m_Capacity = capacity;
			if (m_Size > m_Capacity)
				m_Size = m_Capacity;

			pointer data = allocator_traits::allocate(m_Alloc, m_Capacity);
			for (difference_type i = 0; i < m_Size; i++)
				allocator_traits::construct(m_Alloc, data + i, std::move(*(m_Data + i)));
				// allocator_traits::destroy(m_Alloc, *(m_Data + i)); // not needed, as we move element away (assert that is handled by user)
			allocator_traits::deallocate(m_Alloc, m_Data, oldCapacity);
			m_Data = data;
		}
	private:
		// To be rewritten using pointers only
		pointer m_Data;
		size_type m_Size;
		float m_Factor;
		size_type m_Capacity;
		allocator_traits::allocator_type m_Alloc;
	};
} // blare::container

