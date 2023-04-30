#ifndef _PNU_TIME_HPP_
#define _PNU_TIME_HPP_

#include <core/templates/ratio.hpp>

#include <chrono>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// duration base
	using _DurationBase = std::chrono::duration<f32, Ratio<1>>;

	// length of time measured in seconds
	class Duration : public _DurationBase
	{
	public:
		using base_type = _DurationBase;
		using base_type::base_type;
		using base_type::count;
		using base_type::operator+;
		using base_type::operator-;
		using base_type::operator+=;
		using base_type::operator-=;
		using base_type::operator*=;
		using base_type::operator/=;

		constexpr operator f32() const noexcept { return count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr operator T() const noexcept { return (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr Duration & operator=(T const value) noexcept { return (*this) = Duration{ (rep)value }; }

		constexpr Duration & operator++() noexcept { return (*this) = count() + (rep)1; }
		constexpr Duration operator++(int) noexcept { Duration temp{ *this }; return ++temp, temp; }

		constexpr Duration & operator--() noexcept { return (*this) = count() - (rep)1; }
		constexpr Duration operator--(int) noexcept { Duration temp{ *this }; return --temp, temp; }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator==(T const value) const noexcept { return value == (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator!=(T const value) const noexcept { return value != (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator<(T const value) const noexcept { return value < (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator>(T const value) const noexcept { return value > (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator<=(T const value) const noexcept { return value <= (T)count(); }

		template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
		> constexpr bool operator>=(T const value) const noexcept { return value >= (T)count(); }

		template <class T = u32> constexpr T nanoseconds() const noexcept { return duration_cast<T, Nano>(); }
		template <class T = u32> constexpr T microseconds() const noexcept { return duration_cast<T, Micro>(); }
		template <class T = u32> constexpr T milliseconds() const noexcept { return duration_cast<T, Milli>(); }
		template <class T = u32> constexpr T seconds() const noexcept { return duration_cast<T, Ratio<1>>(); }
		template <class T = u32> constexpr T minutes() const noexcept { return duration_cast<T, Ratio<60>>(); }
		template <class T = u32> constexpr T hours() const noexcept { return duration_cast<T, Ratio<60 * 60>>(); }
		template <class T = u32> constexpr T days() const noexcept { return duration_cast<T, Ratio<60 * 60 * 24>>(); }

	private:
		template <class T = u32, class R = Ratio<1>
		> constexpr T duration_cast() const noexcept
		{
			return std::chrono::duration_cast<std::chrono::duration<T, R>>((_DurationBase const &)*this).count();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD constexpr Duration operator+(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c += b; }

	NODISCARD constexpr Duration operator-(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c -= b; }

	NODISCARD constexpr Duration operator*(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c *= b; }

	NODISCARD constexpr Duration operator/(Duration const & a, Duration const & b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator+=(T & a, Duration const & b) noexcept -> T & { return a = a + (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator-=(T & a, Duration const & b) noexcept -> T & { return a = a - (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator*=(T & a, Duration const & b) noexcept -> T & { return a = a * (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto operator/=(T & a, Duration const & b) noexcept -> T & { return a = a / (T)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator+(T const a, Duration const & b) noexcept { auto c{ a }; return c += b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator-(T const a, Duration const & b) noexcept { auto c{ a }; return c -= b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator*(T const a, Duration const & b) noexcept { auto c{ a }; return c *= b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator/(T const a, Duration const & b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator+=(Duration & a, T const b) noexcept { return a = (Duration::rep)a + (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator-=(Duration & a, T const b) noexcept { return a = (Duration::rep)a - (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator*=(Duration & a, T const b) noexcept { return a = (Duration::rep)a * (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> constexpr auto & operator/=(Duration & a, T const b) noexcept { return a = (Duration::rep)a / (Duration::rep)b; }

	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator+(Duration const & a, T const b) noexcept { auto c{ a }; return c += b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator-(Duration const & a, T const b) noexcept { auto c{ a }; return c -= b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator*(Duration const & a, T const b) noexcept { auto c{ a }; return c *= b; }
	
	template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0
	> NODISCARD constexpr auto operator/(Duration const & a, T const b) noexcept { auto c{ a }; return c /= b; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr auto operator"" _ns(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Nano{}) }; }
	constexpr auto operator"" _ns(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Nano{}) }; }

	constexpr auto operator"" _us(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Micro{}) }; }
	constexpr auto operator"" _us(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Micro{}) }; }

	constexpr auto operator"" _ms(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Milli{}) }; }
	constexpr auto operator"" _ms(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Milli{}) }; }

	constexpr auto operator"" _s(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<1>{}) }; }
	constexpr auto operator"" _s(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<1>{}) }; }

	constexpr auto operator"" _min(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60>{}) }; }
	constexpr auto operator"" _min(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60>{}) }; }

	constexpr auto operator"" _hrs(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60>{}) }; }
	constexpr auto operator"" _hrs(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60>{}) }; }

	constexpr auto operator"" _days(f80 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60 * 24>{}) }; }
	constexpr auto operator"" _days(u64 value) noexcept { return Duration{ ratio_cast(static_cast<f32>(value), Ratio<60 * 60 * 24>{}) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// clock base
	using _ClockBase = std::chrono::high_resolution_clock;

	// clock
	class Clock final
	{
		_ClockBase::time_point m_time;

	public:
		explicit Clock() noexcept : m_time{ _ClockBase::now() } {}
		Duration get_elapsed_time() const noexcept { return _ClockBase::now() - m_time; }
		void restart() noexcept { m_time = _ClockBase::now(); }

		Clock(Clock const &) = delete;
		Clock(Clock &&) noexcept = delete;
		Clock & operator=(Clock const &) = delete;
		Clock & operator=(Clock &&) noexcept = delete;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TIME_HPP_
