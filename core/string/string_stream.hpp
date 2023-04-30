#ifndef _PN_STRING_STREAM_HPP_
#define _PN_STRING_STREAM_HPP_

#include <core/string/string.hpp>

#include <sstream>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string stream base
	template <class C = char
	> using _StringStreamBase = std::basic_stringstream<C, std::char_traits<C>, PolymorphicAllocator<C>>;

	// basic string stream
	template <class C = char
	> class BasicStringStream : public _StringStreamBase<C>
	{
	public:
		using base_type = _StringStreamBase<C>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD auto view() const noexcept {
			auto const v{ rdbuf()->_Get_buffer_view()};
			return BasicStringView<C>{ v._Ptr, v._Size };
		}

		NODISCARD operator BasicStringView<C>() const noexcept {
			auto const v{ rdbuf()->_Get_buffer_view() };
			return BasicStringView<C>{ v._Ptr, v._Size };
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string stream
	using StringStream = BasicStringStream<char>;

	// wide string stream
	using WideStringStream = BasicStringStream<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_STRING_STREAM_HPP_
