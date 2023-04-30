#ifndef _PNU_TYPE_INFO_HPP_
#define _PNU_TYPE_INFO_HPP_

#include <core/string/string_view.hpp>

#include <typeinfo>
#include <typeindex>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// compile time type info
	namespace ctti
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// non-template view type needed ahead
		class NameView : public StringView {
		public:
			using StringView::StringView;
			using StringView::operator=;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if CC_MSVC
#define PRETTY_FUNCTION		__FUNCSIG__
#define PRETTY_TYPE_PREFIX	"class Pnu::ctti::NameView __cdecl Pnu::ctti::nameof<"
#define PRETTY_TYPE_SUFFIX	">(void)"
#elif CC_CLANG
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"Pnu::ctti::NameView Pnu::ctti::nameof() [T = "
#define PRETTY_TYPE_SUFFIX	"]"
#elif CC_GCC
#define PRETTY_FUNCTION		__PRETTY_FUNCTION__
#define PRETTY_TYPE_PREFIX	"constexpr Pnu::ctti::NameView Pnu::ctti::nameof() [with T = "
#define PRETTY_TYPE_SUFFIX	"]"
#else
#error "ctti is unavailable"
#endif

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> constexpr NameView nameof()
		{
			NameView temp{ PRETTY_FUNCTION };
			temp.remove_prefix(sizeof(PRETTY_TYPE_PREFIX) - 1);
			temp.remove_suffix(sizeof(PRETTY_TYPE_SUFFIX) - 1);
			return temp;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// nameof_t
	template <class T
	> struct nameof_t { static constexpr StringView value{ ctti::nameof<T>() }; };

	// nameof_v
	template <class T
	> constexpr StringView nameof_v{ nameof_t<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// hashof_t
	template <class T
	> struct hashof_t { static constexpr size_t value{ nameof_v<T>.hash_code() }; };

	// hashof_v
	template <class T
	> constexpr size_t hashof_v{ hashof_t<T>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace rtti
	{
		inline bool same_type(std::type_info const & lhs, std::type_info const & rhs) noexcept
		{
			return lhs.name() == rhs.name() || std::strcmp(lhs.name(), rhs.name()) == 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TYPE_INFO_HPP_
