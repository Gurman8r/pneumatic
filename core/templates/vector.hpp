#ifndef _PNU_VECTOR_HPP_
#define _PNU_VECTOR_HPP_

#include <core/os/memory.hpp>

#include <vector>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// vector base
	template <class T
	> using _VectorBase = std::vector<T, PolymorphicAllocator<T>>;

	// vector
	template <class T
	> class Vector : public _VectorBase<T>
	{
	public:
		using base_type = _VectorBase<T>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }

		NODISCARD auto find(T const & value) { return std::find(begin(), end(), value); }
		NODISCARD auto find(T const & value) const { return std::find(begin(), end(), value); }
		NODISCARD bool contains(T const & value) const { return end() != find(value); }

		NODISCARD auto find(T && value) noexcept { return std::find(begin(), end(), FWD(value)); }
		NODISCARD auto find(T && value) const noexcept { return std::find(begin(), end(), FWD(value)); }
		NODISCARD bool contains(T && value) const noexcept { return end() != find(FWD(value)); }

		template <class Pr
		> NODISCARD auto find_if(Pr && pr) noexcept { return std::find_if(begin(), end(), FWD(pr)); }

		template <class Pr
		> NODISCARD auto find_if(Pr && pr) const noexcept { return std::find_if(begin(), end(), FWD(pr)); }

		template <class Fn = T(*)(T const &)
		> auto & transform(Fn fn) noexcept { return std::transform(begin(), end(), begin(), fn), (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		template <class V, class K
		> auto getptr(Vector<V> const & l, K && value) noexcept
		{
			if (auto const it{ l.find(FWD(value)) }; it != l.end())
			{
				return (V *)(void *)std::addressof(*it);
			}
			else
			{
				return nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_VECTOR_HPP_
