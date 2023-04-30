#ifndef _PN_RATIO_HPP_
#define _PN_RATIO_HPP_

#include <core/templates/utility.hpp>

#include <ratio>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <i64 N, i64 D = 1
	> using Ratio = std::ratio<N, D>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using Atto =	Ratio<1, 1000000000000000000LL>;
	using Femto =	Ratio<1, 1000000000000000LL>;
	using Pico =	Ratio<1, 1000000000000LL>;
	using Nano =	Ratio<1, 1000000000>;
	using Micro =	Ratio<1, 1000000>;
	using Milli =	Ratio<1, 1000>;
	using Centi =	Ratio<1, 100>;
	using Deci =	Ratio<1, 10>;
	using Deca =	Ratio<10, 1>;
	using Hecto =	Ratio<100, 1>;
	using Kilo =	Ratio<1000, 1>;
	using Mega =	Ratio<1000000, 1>;
	using Giga =	Ratio<1000000000, 1>;
	using Tera =	Ratio<1000000000000LL, 1>;
	using Peta =	Ratio<1000000000000000LL, 1>;
	using Exa =		Ratio<1000000000000000000LL, 1>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, i64 Num, i64 Den
	> constexpr auto ratio_cast(T v, Ratio<Num, Den> const & r)
	{
		constexpr T one{ (T)1 }, num{ (T)r.num }, den{ (T)r.den };

		if constexpr (num == one && den == one)
		{
			return v;
		}
		else if constexpr (num != one && den == one)
		{
			return v * num;
		}
		else if constexpr (num == one && den != one)
		{
			return v / den;
		}
		else
		{
			return v * num / den;
		}
	}

	template <class R, class T = f32
	> constexpr auto ratio_cast(T v) noexcept
	{
		return ratio_cast(v, R{});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> constexpr auto power_of_2(T v)
	{
		// https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
		if constexpr (std::is_floating_point_v<T>)
		{
			return gcem::round(gcem::pow(T{ 2 }, gcem::ceil(gcem::log(v) / gcem::log(T{ 2 }))));
		}
		else
		{
			static_assert(std::is_integral_v<T>);
			v--;
			if constexpr (sizeof(T) >= 1)
				v |= v >> 1; v |= v >> 2; v |= v >> 4;	// 8-bit
			if constexpr (sizeof(T) >= 2) v |= v >> 8;	// 16-bit
			if constexpr (sizeof(T) >= 4) v |= v >> 16; // 32-bit
			if constexpr (sizeof(T) >= 8) v |= v >> 32; // 64-bit
			v++;
			return v;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// kibibyte
	constexpr u64 operator"" _KiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Kilo{}));
	}

	constexpr u64 operator"" _KiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Kilo{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// mebibyte
	constexpr u64 operator"" _MiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Mega{}));
	}

	constexpr u64 operator"" _MiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Mega{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// gibibyte
	constexpr u64 operator"" _GiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Giga{}));
	}

	constexpr u64 operator"" _GiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Giga{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// tebibyte
	constexpr u64 operator"" _TiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Tera{}));
	}

	constexpr u64 operator"" _TiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Tera{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// pebibyte
	constexpr u64 operator"" _PiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Peta{}));
	}

	constexpr u64 operator"" _PiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Peta{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// exbibyte
	constexpr u64 operator"" _EiB(u64 n) noexcept
	{
		return power_of_2(ratio_cast(n, Exa{}));
	}

	constexpr u64 operator"" _EiB(f80 n) noexcept
	{
		return static_cast<u64>(power_of_2(ratio_cast(n, Exa{})));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_PN_RATIO_HPP_
