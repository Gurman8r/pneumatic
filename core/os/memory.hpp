#ifndef _PN_MEMORY_HPP_
#define _PN_MEMORY_HPP_

#include <core/templates/utility.hpp>

#include <memory>
#include <memory_resource>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// memory
namespace pn
{
	class PN_API Memory final
	{
	public:
		static void * alloc_static(size_t size_in_bytes, cstring desc = "");

		static void * realloc_static(void * ptr, size_t old_size, size_t new_size);

		static void free_static(void * ptr);
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// memalloc
#define memalloc \
		(pn::Memory::alloc_static)

// memrealloc
#define memrealloc(ptr, size_in_bytes) \
		(pn::Memory::realloc_static((size_in_bytes), (size), (size)))

// memrealloc (sized)
#define memrealloc_sized(ptr, old_size, new_size) \
		(pn::Memory::realloc_static((ptr), (old_size), (new_size)))

// memfree
#define memfree \
		(pn::Memory::free_static)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// operator new

PN_API_FUNC(void *) operator new(pn::size_t size, pn::cstring desc);

PN_API_FUNC(void *) operator new(pn::size_t size, void * (*alloc_fn)(pn::size_t));

FORCE_INLINE void * operator new(pn::size_t size, void * ptr, pn::size_t check, pn::cstring desc) { return ptr; }

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// operator delete

PN_API_FUNC(void) operator delete(void * ptr, pn::cstring desc);

PN_API_FUNC(void) operator delete(void * ptr, void * (*alloc_fn)(pn::size_t));

FORCE_INLINE void operator delete(void * placement, void * ptr, pn::size_t check, pn::cstring desc) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// postinitialize
	FORCE_INLINE void postinitialize_handler(void *) {}

	template <class T> T * _post_initialize(T * value)
	{
		postinitialize_handler(value);

		return value;
	}

	// memnew
#define memnew(T) \
		(pn::_post_initialize(new (TOSTR(T)) T))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// predelete
	FORCE_INLINE bool predelete_handler(void *) { return true; }

	// memdelete
	template <class T> void memdelete(T * ptr)
	{
		if (!predelete_handler(ptr)) { return; }

		if constexpr (!std::is_trivially_destructible_v<T>) { ptr->~T(); }

		memfree(ptr);
	}

	// memdelete nonzero
#define memdelete_nonzero(ptr)			\
		do {							\
			if (ptr)					\
			{							\
				pn::memdelete(ptr);	\
			}							\
		} while (0)

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// polymorphic allocator base
	template <class T
	> using _PolymorphicAllocatorBase = std::pmr::polymorphic_allocator<T>;

	// polymorphic allocator
	template <class T = u8
	> class PolymorphicAllocator : public _PolymorphicAllocatorBase<T>
	{
	public:
		using base_type = _PolymorphicAllocatorBase<T>;
		using base_type::base_type;
		using base_type::operator=;
	};

	// default allocator
	using DefaultAllocator = PolymorphicAllocator<u8>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// default delete
	template <class T> struct DefaultDelete
	{
		template <class U> void operator()(U * value) const
		{
			memdelete((T *)value);
		}
	};

	// default delete (void)
	template <> struct DefaultDelete<void>
	{
		void operator()(void * value) const
		{
			memfree(value);
		}
	};

	template <class T> void call_default_delete(T * value) noexcept
	{
		DefaultDelete<mpl::intrinsic_t<decltype(value)>>{}(value);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// no delete
	struct NoDelete final
	{
		template <class U> void operator()(U *) const noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// test memory resource
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// passthrough for getting information about an upstream memory resource
	class TestMemoryResource : public std::pmr::memory_resource
	{
	public:
		using pointer					= typename u8 *;
		using const_pointer				= typename u8 const *;
		using reference					= typename u8 &;
		using const_reference			= typename u8 const &;
		using iterator					= typename pointer;
		using const_iterator			= typename const_pointer;
		using reverse_iterator			= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator	= typename std::reverse_iterator<const_iterator>;
		using size_type					= typename size_t;
		using difference_type			= typename ptrdiff_t;

		TestMemoryResource(std::pmr::memory_resource * mres, void const * data, size_t const size) noexcept
			: m_upstream_resource{ mres }
			, m_buffer_data{ (u8 *)data }
			, m_buffer_size{ size }
		{
		}

		bool is_default_resource() const noexcept { return this == std::pmr::get_default_resource(); }
		auto upstream_resource() const noexcept -> std::pmr::memory_resource * const { return m_upstream_resource; }
		auto data() const noexcept -> pointer { return m_buffer_data; }
		auto size() const noexcept -> size_t { return m_buffer_size; }
		auto num_allocations() const noexcept -> size_t { return m_num_allocations; }
		auto bytes_used() const noexcept -> size_t { return m_bytes_used; }
		auto bytes_free() const noexcept -> size_t { return m_buffer_size - m_bytes_used; }

		auto begin() noexcept -> iterator { return m_buffer_data; }
		auto begin() const noexcept -> const_iterator { return m_buffer_data; }
		auto cbegin() const noexcept -> const_iterator { return begin(); }
		auto end() noexcept -> iterator { return m_buffer_data + m_buffer_size; }
		auto end() const noexcept -> const_iterator { return m_buffer_data + m_buffer_size; }
		auto cend() const noexcept -> const_iterator { return end(); }
		auto rbegin() noexcept -> reverse_iterator { return std::make_reverse_iterator(end()); }
		auto rbegin() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(end()); }
		auto crbegin() const noexcept -> const_reverse_iterator { return rbegin(); }
		auto rend() noexcept -> reverse_iterator { return std::make_reverse_iterator(begin()); }
		auto rend() const noexcept -> const_reverse_iterator { return std::make_reverse_iterator(begin()); }
		auto crend() const noexcept -> const_reverse_iterator { return crend(); }

	private:
		void * do_allocate(size_t bytes, size_t align) override
		{
			++m_num_allocations;
			m_bytes_used += bytes;
			return m_upstream_resource->allocate(bytes, align);
		}

		void do_deallocate(void * ptr, size_t bytes, size_t align) override
		{
			--m_num_allocations;
			m_bytes_used -= bytes;
			return m_upstream_resource->deallocate(ptr, bytes, align);
		}

		bool do_is_equal(std::pmr::memory_resource const & value) const noexcept override
		{
			return m_upstream_resource->is_equal(value);
		}

	private:
		std::pmr::memory_resource * const m_upstream_resource;
		pointer const m_buffer_data;
		size_t const m_buffer_size;
		size_t m_num_allocations{}, m_bytes_used{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_MEMORY_HPP_
