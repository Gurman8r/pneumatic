#include <core/os/memory.hpp>
#include <core/os/os.hpp>
#include <core/templates/batch.hpp>
#include <core/error/error_macros.hpp>

#ifndef SHOW_FINAL_ALLOCATIONS
#define SHOW_FINAL_ALLOCATIONS 1
#endif

#ifndef CLEAR_FINAL_ALLOCATIONS
#define CLEAR_FINAL_ALLOCATIONS 0
#endif

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// memory tracker
	static struct MemoryTracker final
	{
		size_t index{};

		enum { ID_Index, ID_Size, ID_Addr, ID_Desc };

		Batch<size_t, size_t, void *, cstring> records{};

		~MemoryTracker()
		{
#if SHOW_FINAL_ALLOCATIONS
			if (!records.empty())
			{
				std::cerr << "\nMEMORY LEAKS DETECTED:\n";

				for (size_t i{}; i < records.size(); ++i)
				{
					records.expand_all(i, [&](size_t index, size_t size, void * addr, cstring desc)
					{
						std::cerr << "index:" << index << " | size:" << size << " | addr:" << addr;
						if (desc) { std::cerr << " | desc:\"" << desc; }
						std::cerr << "\"\n";
					});
				}
#if SYSTEM_WINDOWS
				std::system("pause");
#endif // SYSTEM_WINDOWS
			}
#endif

#if CLEAR_FINAL_ALLOCATIONS
			while (!records.empty()) { memfree(records.back<ID_Addr>()); }
#else
			ASSERT("MEMORY LEAKS DETECTED" && g_memory_tracker.records.empty());
#endif
		}
	}
	g_memory_tracker{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void * Memory::alloc_static(size_t size_in_bytes, cstring desc)
	{
		return std::get<MemoryTracker::ID_Addr>(g_memory_tracker.records.push_back
		(
			++g_memory_tracker.index,
			size_in_bytes,
			std::pmr::get_default_resource()->allocate(size_in_bytes),
			desc
		));
	}

	void * Memory::realloc_static(void * ptr, size_t old_size, size_t new_size)
	{
		if (new_size == 0)
		{
			free_static(ptr);

			return nullptr;
		}
		else if (ptr == nullptr)
		{
			return alloc_static(new_size);
		}
		else if (new_size <= old_size)
		{
			return ptr;
		}
		else
		{
			void * temp{ alloc_static(new_size) };
			if (temp)
			{
				copymem(temp, ptr, old_size);

				free_static(ptr);
			}
			return temp;
		}
	}

	void Memory::free_static(void * ptr)
	{
		if (auto const i{ g_memory_tracker.records.index_of<MemoryTracker::ID_Addr>(ptr) }
		; i != g_memory_tracker.records.npos)
		{
			auto const size_in_bytes{ g_memory_tracker.records.get<MemoryTracker::ID_Size>(i) };

			std::pmr::get_default_resource()->deallocate(ptr, size_in_bytes);

			g_memory_tracker.records.erase(i);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void * operator new(Pnu::size_t size, Pnu::cstring desc)
{
	return Pnu::Memory::alloc_static(size, desc);
}

void * operator new(Pnu::size_t size, void * (*alloc_fn)(Pnu::size_t))
{
	return alloc_fn(size);
}

void operator delete(void * ptr, Pnu::cstring desc)
{
	CRASH("this should never be called");
}

void operator delete(void * ptr, void * (*alloc_fn)(Pnu::size_t))
{
	CRASH("this should never be called");
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */