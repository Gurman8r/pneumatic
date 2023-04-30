#ifndef _PNU_STRING_UTILITY_HPP_
#define _PNU_STRING_UTILITY_HPP_

#include <core/templates/optional.hpp>
#include <core/string/string_stream.hpp>

namespace Pnu::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Fn, class ... Args
	> Optional<T> parse_answer(cstring ptr, Fn && fn, Args && ... args) noexcept
	{
		char * end{};
		auto const answer{ FWD(fn)(ptr, &end, FWD(args)...) };
		if (!(*end != 0 || end == ptr)) {
			return static_cast<T>(answer);
		}
		else {
			return nullopt;
		}
	}

	template <class T
	> char * uintegral_to_buffer(char * next, T value) noexcept
	{
		if constexpr (sizeof(T) > 4)
		{
			while (value > 0xFFFFFFFFU) {
				auto chunk{ static_cast<u32>(value % 1000000000) };
				value /= 1000000000;
				for (i32 i = 0; i != 9; ++i) {
					*--next = static_cast<char>('0' + chunk % 10);
					chunk /= 10;
				}
			}
		}
		auto trunc{ static_cast<u32>(value) };
		do {
			*--next = static_cast<char>('0' + trunc % 10);
			trunc /= 10;
		} while (trunc != 0);
		return next;
	}

	template <class T
	> String integral_to_string(T value) noexcept
	{
		static_assert(std::is_integral_v<T>);
		char			buf[21]	{ /* can hold 2^64 - 1, plus NUL */ };
		char * const	end		{ std::end(buf) };
		char *			next	{ end };
		auto const		uval	{ static_cast<std::make_unsigned_t<T>>(value) };
		if (value < 0) {
			next = uintegral_to_buffer(next, 0 - uval);
			*--next = '-';
		}
		else {
			next = uintegral_to_buffer(next, uval);
		}
		return String{ next, end };
	}

	template <class T
	> String uintegral_to_string(T value) noexcept
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		static_assert(std::is_unsigned_v<T>, "T must be unsigned");
		char buf[21]{ /* can hold 2^64 - 1, plus NUL */ };
		char * const end{ std::end(buf) };
		char * const next{ uintegral_to_buffer(end, value) };
		return String{ next, end };
	}

	template <class T
	> String floating_point_to_string(T value) noexcept
	{
		static_assert(std::is_floating_point_v<T>);
		auto const n{ static_cast<size_t>(_CSTD _scprintf("%f", value)) };
		String temp{ n, '\0', String::allocator_type{} };
		_CSTD sprintf_s(temp.data(), n + 1, "%f", value);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Optional<i8> to_i8(String const & value, i32 base = 10) noexcept { return parse_answer<i8>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<i16> to_i16(String const & value, i32 base = 10) noexcept { return parse_answer<i16>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<i32> to_i32(String const & value, i32 base = 10) noexcept { return parse_answer<i32>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<i64> to_i64(String const & value, i32 base = 10) noexcept { return parse_answer<i64>(value.c_str(), &_CSTD strtoll, base); }

	inline Optional<u8> to_u8(String const & value, i32 base = 10) noexcept { return parse_answer<u8>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<u16> to_u16(String const & value, i32 base = 10) noexcept { return parse_answer<u16>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<u32> to_u32(String const & value, i32 base = 10) noexcept { return parse_answer<u32>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<u64> to_u64(String const & value, i32 base = 10) noexcept { return parse_answer<u64>(value.c_str(), &_CSTD strtoull, base); }

	inline Optional<f32> to_f32(String const & value) noexcept { return parse_answer<f32>(value.c_str(), &_CSTD strtod); }

	inline Optional<f64> to_f64(String const & value) noexcept { return parse_answer<f64>(value.c_str(), &_CSTD strtod); }

	inline Optional<f80> to_f80(String const & value) noexcept { return parse_answer<f80>(value.c_str(), &_CSTD strtold); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline String to_string(i8 value) noexcept { return integral_to_string(value); }

	inline String to_string(i16 value) noexcept { return integral_to_string(value); }

	inline String to_string(i32 value) noexcept { return integral_to_string(value); }

	inline String to_string(i64 value) noexcept { return integral_to_string(value); }

	inline String to_string(u8 value) noexcept { return uintegral_to_string(value); }

	inline String to_string(u16 value) noexcept { return uintegral_to_string(value); }

	inline String to_string(u32 value) noexcept { return uintegral_to_string(value); }

	inline String to_string(u64 value) noexcept { return uintegral_to_string(value); }

	inline String to_string(f32 value) noexcept { return floating_point_to_string(value); }

	inline String to_string(f64 value) noexcept { return floating_point_to_string(value); }

	inline String to_string(f80 value) noexcept { return floating_point_to_string(value); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	inline Optional<bool> to_bool(String const & value) noexcept
	{
		switch (value.to_lower().hash_code()) {
		case "1"_hash:
		case "yes"_hash:
		case "true"_hash: return make_optional(true);
		case "0"_hash:
		case "no"_hash:
		case "false"_hash: return make_optional(false);
		}
		return nullopt;
	}

	inline String to_string(bool value) noexcept
	{
		return value ? "true"_s : "false"_s;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_STRING_UTILITY_HPP_
