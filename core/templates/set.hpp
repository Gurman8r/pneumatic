#ifndef _PNU_SET_HPP_
#define _PNU_SET_HPP_

#include <core/os/memory.hpp>

#include <set>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// set base
	template <class T, class Pr = Less<T>
	> using _SetBase = std::set<T, Pr, PolymorphicAllocator<T>>;

	// set
	template <class T, class Pr = Less<T>
	> class Set : public _SetBase<T, Pr>
	{
	public:
		using base_type = _SetBase<T, Pr>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// multi set base
	template <class T, class Pr = Less<T>
	> using _MultiSetBase = std::multiset<T, Pr, PolymorphicAllocator<T>>;

	// multi set
	template <class T, class Pr = Less<T>
	> class MultiSet : public _MultiSetBase<T, Pr>
	{
	public:
		using base_type = _MultiSetBase<T, Pr>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return (void *)(intptr_t)!empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_SET_HPP_
