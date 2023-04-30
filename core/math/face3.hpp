#ifndef _PNU_FACE3_HPP_
#define _PNU_FACE3_HPP_

#include <core/math/matrix.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Face3
	{
		Vec3 vertex[3]{};

		constexpr Face3(Face3 const &) = default;
		constexpr Face3(Face3 &&) noexcept = default;
		constexpr Face3 & operator=(Face3 const &) = default;
		constexpr Face3 & operator=(Face3 &&) noexcept = default;
		constexpr Face3(Vec3 const & a = {}, Vec3 const & b = {}, Vec3 const & c = {}) noexcept : vertex{ a, b, c } {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_FACE3_HPP_
