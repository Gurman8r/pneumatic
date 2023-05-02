#ifndef _PNU_BUFFER_HPP_
#define _PNU_BUFFER_HPP_

#include <core/templates/array.hpp>
#include <core/string/string.hpp>
#include <core/templates/mpl.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// dynamic buffer
	class DynamicBuffer
	{
		Vector<u8> m_data{};

	public:
		using self_type			= typename DynamicBuffer;
		using storage_type		= typename decltype(m_data);
		using allocator_type	= typename DefaultAllocator;
		using reference			= typename u8 &;
		using const_reference	= typename u8 const &;
		using pointer			= typename u8 *;
		using const_pointer		= typename u8 const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		DynamicBuffer(allocator_type alloc = {}) noexcept : m_data{ alloc } {}

		DynamicBuffer(size_t size_in_bytes, u8 value = 0, allocator_type alloc = {}) : m_data{ alloc } { resize(size_in_bytes, value); }

		DynamicBuffer(void const * src, size_t size_in_bytes, allocator_type alloc = {}) : m_data{ alloc } { write(src, size_in_bytes); }

		template <class Iter = iterator
		> DynamicBuffer(Iter first, Iter last, allocator_type alloc = {}) : m_data{ alloc } { write(first, std::distance(first, last) / sizeof(*first)); }

		DynamicBuffer(storage_type const & data, allocator_type alloc = {}) : m_data{ data, alloc } {}

		DynamicBuffer(storage_type && data, allocator_type alloc = {}) noexcept : m_data{ std::move(data), alloc } {}

		DynamicBuffer(self_type const & value, allocator_type alloc = {}) : m_data{ alloc } { copy(value); }

		DynamicBuffer(self_type && value, allocator_type alloc = {}) noexcept : m_data{ alloc } { swap(value); }

		self_type & operator=(self_type const & value) { return copy(value); }

		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			return m_data.clear(), (*this);
		}

		self_type & copy(storage_type const & value)
		{
			return (m_data = value), (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
			return (*this);
		}

		self_type & resize(size_t size_in_bytes, u8 value = 0)
		{
			return m_data.resize(size_in_bytes, value), (*this);
		}

		self_type & shrink_to_fit()
		{
			return m_data.shrink_to_fit(), (*this);
		}

		self_type & swap(storage_type & value) noexcept
		{
			return m_data.swap(value), (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		NODISCARD auto capacity() const noexcept -> size_t { return m_data.capacity(); }
		NODISCARD auto size() const noexcept -> size_t { return m_data.size(); }
		NODISCARD auto max_size() const noexcept -> size_t { return m_data.max_size(); }

		NODISCARD auto data() noexcept -> pointer { return m_data.data(); }
		NODISCARD auto data() const noexcept -> const_pointer { return m_data.data(); }
		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)data(); }

		NODISCARD auto begin() noexcept -> iterator { return data(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return data(); }

		NODISCARD auto end() noexcept -> iterator { return begin() + size(); }
		NODISCARD auto end() const noexcept -> const_iterator { return begin() + size(); }

		NODISCARD auto front() noexcept -> reference { ASSERT(!empty()); return *(begin()); }
		NODISCARD auto front() const noexcept -> const_reference { ASSERT(!empty()); return *(begin()); }

		NODISCARD auto back() noexcept -> reference { ASSERT(!empty()); return *(end() - 1); }
		NODISCARD auto back() const noexcept -> const_reference { ASSERT(!empty()); return *(end() - 1); }

		NODISCARD auto operator[](size_t i) noexcept -> reference { ASSERT(i < size()); return m_data[i]; }
		NODISCARD auto operator[](size_t i) const noexcept -> u8 { ASSERT(i < size()); return m_data[i]; }

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }
		NODISCARD operator String() const noexcept { return { c_str(), size() }; }

	public:
		void do_read(size_t index, void * dst, size_t size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes < size() && "OPERATION WOULD CAUSE AN OVERFLOW");
			copymem(dst, data() + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(size_t index, T & value) const
		{
			return do_read(index, std::addressof(value), sizeof(T)), value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t index, void const * src, size_t size_in_bytes, size_t align = 0)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			size_t n{ (0 < align && align % 2 == 0) ? SIZE_ROUND_UP(size_in_bytes, align) : size_in_bytes };
			if (size() < index + n) { resize(index + n); }
			copymem(data() + index, src, n);
		}

		self_type & write(size_t index, void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(size(), src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t index, T const & value)
		{
			do_write(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			do_write(size(), std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		self_type & printv(size_t index, cstring fmt, va_list args)
		{
			char s[4096]{};
			if (i32 const n{ std::vsnprintf(s, sizeof(s), fmt, args) }
			; 0 < n) { do_write(index, s, (size_t)n); }
			return (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(size(), fmt, args);
		}

		self_type & printf(size_t index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t index, cstring str, size_t size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
		}

		self_type & print(size_t index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
		}

		self_type & print(size_t index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}

	public:
		template <class T, std::enable_if_t<(1 == sizeof(T)), int> = 0
		> self_type & operator<<(T const & value)
		{
			return m_data.push_back(static_cast<u8>(value)), (*this);
		}

		template <class T, std::enable_if_t<(1 < sizeof(T)), int> = 0
		> self_type & operator<<(T const & value)
		{
			return write(value);
		}

		self_type & operator<<(cstring str)
		{
			return print(str);
		}

		self_type & operator<<(String const & str)
		{
			return print(str);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// static buffer
	template <size_t _SizeInBytes
	> class StaticBuffer
	{
		static_assert(0 < _SizeInBytes);

		Array<u8, _SizeInBytes> m_data{};

	public:
		using self_type			= typename StaticBuffer<_SizeInBytes>;
		using storage_type		= typename decltype(m_data);
		using reference			= typename u8 &;
		using const_reference	= typename u8 const &;
		using pointer			= typename u8 *;
		using const_pointer		= typename u8 const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		StaticBuffer() noexcept {}

		StaticBuffer(size_t index, void const * src, size_t size_in_bytes) noexcept { write(index, src, size_in_bytes); }

		StaticBuffer(void const * src, size_t size_in_bytes) noexcept { write(src, size_in_bytes); }

		template <class Iter = iterator
		> StaticBuffer(Iter first, Iter last) { write(first, std::distance(first, last) / sizeof(*first)); }

		StaticBuffer(storage_type const & value) { copy(value); }

		StaticBuffer(storage_type && value) noexcept { swap(value); }

		StaticBuffer(self_type const & value) { copy(value); }

		StaticBuffer(self_type && value) noexcept { swap(value); }
	
		self_type & operator=(storage_type const & value) { return copy(value); }
	
		self_type & operator=(storage_type && value) noexcept { return swap(value); }
	
		self_type & operator=(self_type const & value) { return copy(value); }
	
		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _SizeInBytes);
			return (*this);
		}

		self_type & copy(storage_type const & value)
		{
			copymem(m_data, value, _SizeInBytes);
			return (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
			return (*this);
		}

		self_type & swap(storage_type & value) noexcept
		{
			storage_type temp;
			copymem(temp, m_data, _SizeInBytes);
			copymem(m_data, value, _SizeInBytes);
			copymem(value, temp, _SizeInBytes);
			return (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		NODISCARD static constexpr bool empty() noexcept { return false; }
		NODISCARD static constexpr auto capacity() noexcept -> size_t { return _SizeInBytes; }
		NODISCARD static constexpr auto size() noexcept -> size_t { return _SizeInBytes; }
		NODISCARD static constexpr auto max_size() noexcept -> size_t { return _SizeInBytes; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }
		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }
		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		NODISCARD auto begin() noexcept -> iterator { return m_data; }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data; }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data; }

		NODISCARD auto end() noexcept -> iterator { return m_data + size(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data + size(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data + size(); }

		NODISCARD auto front() noexcept -> reference { return *m_data; }
		NODISCARD auto front() const noexcept -> const_reference { return *m_data; }

		NODISCARD auto back() noexcept -> reference { return *(m_data + size() - 1); }
		NODISCARD auto back() const noexcept -> const_reference { return *(m_data + size() - 1); }

		NODISCARD auto operator[](size_t i) noexcept -> reference { ASSERT(i < size()); return m_data[i]; }
		NODISCARD auto operator[](size_t i) const noexcept -> u8 { ASSERT(i < size()); return m_data[i]; }

		NODISCARD operator void * () const noexcept { return (void *)data(); }
		NODISCARD operator DynamicBuffer() const noexcept { return { m_data, size() }; }
		NODISCARD operator String() const noexcept { return { (cstring)m_data, size() }; }

	public:
		void do_read(size_t index, void * dst, size_t size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= size() && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, m_data + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(size_t index, T & value) const
		{
			return do_read(index, std::addressof(value), sizeof(T)), value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

	public:
		void do_write(size_t index, void const * src, size_t size_in_bytes, size_t align = 0)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			size_t n{ (0 < align && align % 2 == 0) ? SIZE_ROUND_UP(size_in_bytes, align) : size_in_bytes };
			ASSERT(index + n <= size() && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(m_data + index, src, n);
		}

		self_type & write(size_t index, void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t index, T const & value)
		{
			do_write(index, std::addressof(value), sizeof(T));
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			do_write(0, std::addressof(value), sizeof(T));
			return (*this);
		}

	public:
		self_type & printv(size_t index, cstring fmt, va_list args)
		{
			return std::vsnprintf((char *)m_data + index, size() - index, fmt, args), (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(size(), fmt, args);
		}

		self_type & printf(size_t index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t index, cstring str, size_t size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
		}

		self_type & print(size_t index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
		}

		self_type & print(size_t index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// constant buffer
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <size_t _Alignment, class ... _Types
	> class ConstantBuffer
	{
		static_assert(0 < _Alignment && _Alignment % 2 == 0);

		static_assert(0 < sizeof...(_Types));

		template <size_t I
		> using type_i = mpl::nth<I, mpl::type_list<_Types...>>;

		template <size_t I = 0, size_t Result = 0
		> static constexpr size_t _calc_size() noexcept
		{
			if constexpr (I < sizeof...(_Types))
			{
				return _calc_size<I + 1, Result + SIZE_ROUND_UP(sizeof(type_i<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		template <size_t Index, size_t I = 0, size_t Result = 0
		> static constexpr size_t _calc_index() noexcept
		{
			static_assert(Index < sizeof...(_Types));
			if constexpr (I < Index)
			{
				return _calc_index<Index, I + 1, Result + SIZE_ROUND_UP(sizeof(type_i<I>), _Alignment)>();
			}
			else
			{
				return Result;
			}
		}

		static constexpr size_t _SizeInBytes{ _calc_size() };

		Array<u8, _SizeInBytes> m_data{};

	public:
		using self_type			= typename ConstantBuffer<_Alignment, _Types...>;
		using storage_type		= typename decltype(m_data);
		using reference			= typename u8 &;
		using const_reference	= typename u8 const &;
		using pointer			= typename u8 *;
		using const_pointer		= typename u8 const *;
		using iterator			= typename pointer;
		using const_iterator	= typename const_pointer;

		ConstantBuffer() noexcept {}

		ConstantBuffer(size_t index, void const * src, size_t size_in_bytes) noexcept { write(index, src, size_in_bytes); }

		ConstantBuffer(void const * src, size_t size_in_bytes) noexcept { write(src, size_in_bytes); }

		template <class Iter = iterator
		> ConstantBuffer(Iter first, Iter last) { write(first, std::distance(first, last) / sizeof(*first)); }

		ConstantBuffer(storage_type const & value) { copy(value); }

		ConstantBuffer(storage_type && value) noexcept { swap(value); }

		ConstantBuffer(self_type const & value) { copy(value); }

		ConstantBuffer(self_type && value) noexcept { swap(value); }

		self_type & operator=(storage_type const & value) { return copy(value); }

		self_type & operator=(storage_type && value) noexcept { return swap(value); }

		self_type & operator=(self_type const & value) { return copy(value); }

		self_type & operator=(self_type && value) noexcept { return swap(value); }

	public:
		self_type & clear() noexcept
		{
			zeromem(m_data, _SizeInBytes);
			return (*this);
		}

		self_type & copy(storage_type const & value)
		{
			copymem(m_data, value, _SizeInBytes);
			return (*this);
		}

		self_type & copy(self_type const & value)
		{
			if (this != std::addressof(value)) { copy(value.m_data); }
			return (*this);
		}

		self_type & swap(storage_type & value) noexcept
		{
			storage_type temp;
			copymem(temp, m_data, _SizeInBytes);
			copymem(m_data, value, _SizeInBytes);
			copymem(value, temp, _SizeInBytes);
			return (*this);
		}

		self_type & swap(self_type & value) noexcept
		{
			if (this != std::addressof(value)) { swap(value.m_data); }
			return (*this);
		}

	public:
		NODISCARD static constexpr bool empty() noexcept { return false; }
		NODISCARD static constexpr auto capacity() noexcept -> size_t { return _SizeInBytes; }
		NODISCARD static constexpr auto size() noexcept -> size_t { return _SizeInBytes; }
		NODISCARD static constexpr auto max_size() noexcept -> size_t { return _SizeInBytes; }

		NODISCARD auto data() noexcept -> pointer { return m_data; }
		NODISCARD auto data() const noexcept -> const_pointer { return m_data; }
		NODISCARD auto c_str() const noexcept -> cstring { return (cstring)m_data; }

		NODISCARD auto begin() noexcept -> iterator { return m_data; }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data; }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data; }

		NODISCARD auto end() noexcept -> iterator { return m_data + size(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data + size(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data + size(); }

		NODISCARD auto front() noexcept -> reference { return *m_data; }
		NODISCARD auto front() const noexcept -> const_reference { return *m_data; }

		NODISCARD auto back() noexcept -> reference { return *(m_data + size() - 1); }
		NODISCARD auto back() const noexcept -> const_reference { return *(m_data + size() - 1); }

		NODISCARD auto operator[](size_t i) noexcept -> reference { ASSERT(i < size()); return m_data[i]; }
		NODISCARD auto operator[](size_t i) const noexcept -> u8 { ASSERT(i < size()); return m_data[i]; }

		NODISCARD operator void * () const noexcept { return (void *)data(); }
		NODISCARD operator DynamicBuffer() const noexcept { return { m_data, size() }; }
		NODISCARD operator String() const noexcept { return { (cstring)m_data, size() }; }

	public:
		void do_read(size_t index, void * dst, size_t size_in_bytes) const
		{
			ASSERT(dst && "INVALID DESTINATION");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= size() && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(dst, m_data + index, size_in_bytes);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(size_t index, T & value) const
		{
			return do_read(index, std::addressof(value), sizeof(T)), value;
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T & get_to(T & value) const
		{
			return get_to<T>(0, value);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> NODISCARD T get(size_t index = 0) const
		{
			T temp;
			return get_to<T>(index, temp);
		}

		template <size_t I
		> NODISCARD type_i<I> get() const
		{
			return get<type_i<I>>(_calc_index<I>());
		}

	public:
		void do_write(size_t index, void const * src, size_t size_in_bytes)
		{
			ASSERT(src && "INVALID SOURCE");
			ASSERT(0 < size_in_bytes && "SIZE MUST BE GREATER THAN ZERO");
			ASSERT(index + size_in_bytes <= size() && "OPERATION WOULD HAVE CAUSED AN OVERFLOW");
			copymem(m_data + index, src, size_in_bytes);
		}

		self_type & write(size_t index, void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(index, src, size_in_bytes); }
			return (*this);
		}

		self_type & write(void const * src, size_t size_in_bytes)
		{
			if (src && 0 < size_in_bytes) { do_write(0, src, size_in_bytes); }
			return (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(size_t index, T const & value)
		{
			return do_write(index, std::addressof(value), sizeof(T)), (*this);
		}

		template <class T, std::enable_if_t<std::is_trivially_copyable_v<T>, int> = 0
		> self_type & write(T const & value)
		{
			return do_write(0, std::addressof(value), sizeof(T)), (*this);
		}

		template <size_t I
		> self_type & set(type_i<I> const & value)
		{
			return write(_calc_index<I>(), value);
		}

	public:
		self_type & printv(size_t index, cstring fmt, va_list args)
		{
			return std::vsnprintf((char *)m_data + index, size() - index, fmt, args), (*this);
		}

		self_type & printv(cstring fmt, va_list args)
		{
			return printv(size(), fmt, args);
		}

		self_type & printf(size_t index, cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(index, fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & printf(cstring fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			printv(size(), fmt, args);
			va_end(args);
			return (*this);
		}

		self_type & print(size_t index, cstring str, size_t size_in_bytes) noexcept
		{
			return write(index, str, size_in_bytes);
		}

		self_type & print(size_t index, cstring str) noexcept
		{
			return print(index, str, std::strlen(str));
		}

		self_type & print(cstring str, size_t size_in_bytes) noexcept
		{
			return print(size(), str, size_in_bytes);
		}

		self_type & print(cstring str) noexcept
		{
			return print(size(), str, std::strlen(str));
		}

		self_type & print(size_t index, self_type const & str) noexcept
		{
			return print(index, str.c_str(), str.size());
		}

		self_type & print(self_type const & str) noexcept
		{
			return print(size(), str.c_str(), str.size());
		}

		self_type & print(size_t index, String const & str) noexcept
		{
			return print(index, str.data(), str.size());
		}

		self_type & print(String const & str) noexcept
		{
			return print(size(), str.data(), str.size());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_BUFFER_HPP_
