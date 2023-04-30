#ifndef _PN_STRING_HPP_
#define _PN_STRING_HPP_

#include <core/error/error_macros.hpp>
#include <core/string/string_view.hpp>
#include <core/templates/vector.hpp>

#include <string>

namespace pn::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD constexpr bool is_slash(char c) noexcept
	{
		return c == '/' || c == '\\';
	}

	NODISCARD constexpr bool is_drive_prefix(cstring first)
	{
		// test if first points to a prefix of the form X:
		return (first && *first)
			&& ((first[0] >= 'a' && first[0] <= 'z')
			|| (first[0] >= 'A' && first[0] <= 'Z'))
			&& (first[1] == ':');
	}

	NODISCARD constexpr bool has_drive_letter_prefix(cstring first, cstring last)
	{
		return (last - first >= 2) && is_drive_prefix(first);
	}

	NODISCARD inline cstring find_root_name_end(cstring first, cstring last)
	{
		// attempt to parse [first, last) as a path and return the end of root-name if it exists; otherwise, first
		if (last - first < 2) { return first; }

		// check for X: first because it's the most common root-name
		if (has_drive_letter_prefix(first, last)) { return first + 2; }

		// all the other root-names start with a slash; check that first because
		// we expect paths without a leading slash to be very common
		if (!is_slash(first[0])) { return first; }

		// $ means anything other than a slash, including potentially the end of the input
		if (last - first >= 4 && is_slash(first[3]) && (last - first == 4 || !is_slash(first[4])) // \xx\$
			&& ((is_slash(first[1]) && (first[2] == '?' || first[2] == '.')) // \\?\$ or \\.\$
				|| (first[1] == '?' && first[2] == '?'))) { // \??\$
			return first + 3;
		}

		// \\server
		if (last - first >= 3 && is_slash(first[1]) && !is_slash(first[2])) {
			return std::find_if(first + 3, last, is_slash);
		}

		// no match
		return first;
	}

	NODISCARD inline cstring find_relative_path(cstring first, cstring last)
	{
		return std::find_if_not(find_root_name_end(first, last), last, is_slash);
	}

	NODISCARD inline cstring find_filename(cstring first, cstring last)
	{
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		return last;
	}

	NODISCARD constexpr cstring find_extension(cstring filename, cstring ads)
	{
		// find dividing point between stem and extension in a generic format filename consisting of [filename, ads)
		auto extension{ ads };

		// empty path
		if (filename == extension) { return ads; }

		--extension;

		// path is length 1 and either dot, or has no dots; either way, extension is empty
		if (filename == extension) { return ads; }

		// we might have found the end of stem
		if (*extension == '.')
		{
			if (filename == extension - 1 && extension[-1] == '.')
			{
				// dotdot special case
				return ads;
			}
			else
			{
				// x.
				return extension;
			}
		}

		while (filename != --extension)
		{
			if (*extension == '.')
			{
				// found a dot which is not in first position, so it starts extension
				return extension;
			}
		}

		// if we got here, either there are no dots, in which case extension is empty, or the first element
		// is a dot, in which case we have the leading single dot special case, which also makes extension empty
		return ads;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline StringView parse_root_name(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_root_name_end(first, last) - first) };
	}

	NODISCARD inline StringView parse_root_directory(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const root_name_end{ find_root_name_end(first, last) };
		auto const relative_path{ std::find_if_not(root_name_end, last, is_slash) };
		return { root_name_end, static_cast<size_t>(relative_path - root_name_end) };
	}

	NODISCARD inline StringView parse_root_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		return { first, static_cast<size_t>(find_relative_path(first, last) - first) };
	}

	NODISCARD inline StringView parse_relative_path(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		return { relative_path, static_cast<size_t>(last - relative_path) };
	}

	NODISCARD inline StringView parse_parent_path(StringView const value)
	{
		auto const first{ value.data() };
		auto last{ first + value.size() };
		auto const relative_path{ find_relative_path(first, last) };
		while (relative_path != last && !is_slash(last[-1])) { --last; }
		while (relative_path != last && is_slash(last[-1])) { --last; }
		return { first, static_cast<size_t>(last - first) };
	}

	NODISCARD inline StringView parse_filename(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		return { filename, static_cast<size_t>(last - filename) };
	}

	NODISCARD inline StringView parse_stem(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		auto const ads{ std::find(filename, last, ':') };
		auto const extension{ find_extension(filename, ads) };
		return { filename, static_cast<size_t>(extension - filename) };
	}

	NODISCARD inline StringView parse_extension(StringView const value)
	{
		auto const first{ value.data() };
		auto const last{ first + value.size() };
		auto const filename{ find_filename(first, last) };
		auto const ads{ std::find(filename, last, ':') };
		auto const extension{ find_extension(filename, ads) };
		return { extension, static_cast<size_t>(ads - extension) };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string base
	template <class C = char
	> using _StringBase = std::basic_string<C, std::char_traits<C>, PolymorphicAllocator<C>>;

	// basic string
	template <class C = char
	> class BasicString : public _StringBase<C>
	{
	public:
		using self_type = typename BasicString<C>;
		using base_type = typename _StringBase<C>;
		using traits_type = typename base_type::traits_type;
		using value_type = typename base_type::value_type;
		using pointer = typename base_type::pointer;
		using const_pointer = typename base_type::const_pointer;
		using reference = typename base_type::reference;
		using const_reference = typename base_type::const_reference;
		using iterator = typename base_type::iterator;
		using const_iterator = typename base_type::const_iterator;
		using reverse_iterator = typename base_type::reverse_iterator;
		using const_reverse_iterator = typename base_type::const_reverse_iterator;
		using size_type = typename base_type::size_type;
		using difference_type = typename base_type::difference_type;

	public:
		using base_type::base_type;
		using base_type::operator=;
		BasicString(BasicStringView<C> const v) : base_type{ v.data(), v.size() } {}

	public:
		static constexpr bool is_narrow{ 1 == sizeof(C) };
		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }
		NODISCARD auto hash_code() const noexcept { return hash_representation(data(), size()); }
		NODISCARD auto view() const noexcept { return BasicStringView<C>{ data(), size() }; }
		NODISCARD operator BasicStringView<C>() const noexcept { return { data(), size() }; }
		template <class T> self_type & operator+=(T && a) noexcept { return base_type::operator+=(FWD(a)), (*this); }

	public:
		NODISCARD bool contains(C const a) const noexcept { return find(a) != npos; }
		NODISCARD bool contains(C const * a) const noexcept { return find(a) != npos; }
		NODISCARD bool contains(self_type const & a) const noexcept { return find(a) != npos; }

		NODISCARD bool begins_with(C const a) const noexcept { return !empty() && front() == a; }
		NODISCARD bool begins_with(self_type const & a) const noexcept { return (a.size() <= size()) && (a == substr(0, a.size())); }

		NODISCARD bool ends_with(C const a) const noexcept { return !empty() && back() == a; }
		NODISCARD bool ends_with(self_type const & a) const noexcept { return (a.size() <= size()) && (a == substr(size() - a.size(), a.size())); }

	public:
		auto & replace_first(self_type const & a, self_type const & b) {
			if (size_t const i{ find(a, 0) }; i != npos) {
				replace(i, a.size(), b);
			}
			return (*this);
		}

		auto & replace_all(self_type const & a, self_type const & b) {
			for (size_t i{}; (i = find(a, i)) != npos;) {
				replace(i, a.size(), b);
				i += a.size();
			}
			return (*this);
		}

		auto replace_first(self_type const & a, self_type const & b) const noexcept { return String{ *this }.replace_first(a, b); }
		auto replace_all(self_type const & a, self_type const & b) const noexcept { return String{ *this }.replace_all(a, b); }

	public:
		auto & erase_duplicates(C const a) { return erase(std::unique(begin(), end(), [a](C a, C b) { return (a == b) && (a == a); }), end()), (*this); }

		template <class Fn = C(*)(C)
		> auto & transform(Fn fn) noexcept { return std::transform(begin(), end(), begin(), fn), (*this); }

	public:
		template <class = std::enable_if_t<is_narrow>
		> auto const & narrow() const noexcept { return (*this); }

		template <class = std::enable_if_t<!is_narrow>
		> auto const & widen() const noexcept { return (*this); }

		template <class = std::enable_if_t<!is_narrow>
		> auto narrow() const {
			BasicString<char> temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(static_cast<char>(c)); }
			return temp;
		}

		template <class = std::enable_if_t<is_narrow>
		> auto widen() const {
			BasicString<wchar_t> temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(static_cast<wchar_t>(c)); }
			return temp;
		}

		auto to_upper() const {
			self_type temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(std::toupper(c)); }
			return temp;
		}

		auto to_lower() const {
			self_type temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(std::tolower(c)); }
			return temp;
		}
		
	public:
		template <class Fn = int(*)(int)
		> auto & trim_back(Fn fn = std::isspace) {
			while (!empty() && fn(back())) { pop_back(); }
			return (*this);
		}

		template <class Fn = int(*)(int)
		> auto & trim_front(Fn fn = std::isspace) {
			while (!empty() && fn(front())) { erase(begin()); }
			return (*this);
		}

		template <class Fn = int(*)(int)
		> auto & trim(Fn fn = std::isspace) {
			while (!empty() && fn(back())) { pop_back(); }
			while (!empty() && fn(front())) { erase(begin()); }
			return (*this);
		}

	public:
		NODISCARD static auto split(self_type s, C const delimiter)
		{
			Vector<self_type> v{};
			size_t i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back((self_type)s.substr(0, i));
				s.erase(0, i + 1);
			}
			v.push_back(s);
			return v;
		}

		NODISCARD static auto split(self_type s, cstring delimiter)
		{
			Vector<self_type> v{};
			size_t i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back((self_type)s.substr(0, i));
				s.erase(0, i + std::strlen(delimiter));
			}
			v.push_back(s);
			return v;
		}

		NODISCARD static auto split(self_type s, self_type const & delimiter)
		{
			Vector<self_type> v{};
			size_t i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back((self_type)s.substr(0, i));
				s.erase(0, i + delimiter.size());
			}
			v.push_back(s);
			return v;
		}

		NODISCARD auto split(C const delimiter) const noexcept { return split(*this, delimiter); }

		NODISCARD auto split(C const * delimiter) const noexcept { return split(*this, delimiter); }

		NODISCARD auto split(self_type const & delimiter) const noexcept { return split(*this, delimiter); }

	public:
		template <size_t buffer_size = 0
		> static i32 format(self_type & s, cstring fmt, va_list args)
		{
			if constexpr (0 < buffer_size)
			{
				C buffer[buffer_size]{};
				i32 const n{ std::vsnprintf(buffer, buffer_size, fmt, args) };
				if (0 < n) {
					s = self_type{ buffer, buffer + (size_t)n, allocator_type{} };
				}
				return n;
			}
			else
			{
				va_list args_copy;
				va_copy(args_copy, args);
				i32 const n{ std::vsnprintf(nullptr, 0, fmt, args_copy) };
				va_end(args_copy);
				if (0 < n) {
					s.resize((size_t)n + 1);
					std::vsnprintf(s.data(), s.size(), fmt, args);
				}
				return n;
			}
		}

		template <size_t buffer_size = 0
		> static i32 format(self_type & s, cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			i32 const n{ format<buffer_size>(s, fmt, args) };
			va_end(args);
			return n;
		}

		template <size_t buffer_size = 0
		> NODISCARD static self_type format(cstring fmt, va_list args) noexcept {
			self_type s;
			format<buffer_size>(s, fmt, args);
			return s;
		}

		template <size_t buffer_size = 0
		> NODISCARD static self_type format(cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			auto const s{ format<buffer_size>(fmt, args) };
			va_end(args);
			return s;
		}

	public:
		NODISCARD auto root_name() const noexcept -> self_type { return util::parse_root_name(narrow().view()); }
		NODISCARD auto root_directory() const noexcept -> self_type { return util::parse_root_directory(narrow().view()); }
		NODISCARD auto root_path() const noexcept -> self_type { return util::parse_root_path(narrow().view()); }
		NODISCARD auto relative_path() const noexcept -> self_type { return util::parse_relative_path(narrow().view()); }
		NODISCARD auto parent_path() const noexcept -> self_type { return util::parse_parent_path(narrow().view()); }
		NODISCARD auto filename() const noexcept -> self_type { return util::parse_filename(narrow().view()); }
		NODISCARD auto stem() const noexcept -> self_type { return util::parse_stem(narrow().view()); }
		NODISCARD auto extension() const noexcept -> self_type { return util::parse_extension(narrow().view()); }

		NODISCARD bool has_root_name() const noexcept { return !util::parse_root_name(narrow().view()).empty(); }
		NODISCARD bool has_root_directory() const noexcept { return !util::parse_root_directory(narrow().view()).empty(); }
		NODISCARD bool has_root_path() const noexcept { return !util::parse_root_path(narrow().view()).empty(); }
		NODISCARD bool has_relative_path() const noexcept { return !util::parse_relative_path(narrow().view()).empty(); }
		NODISCARD bool has_parent_path() const noexcept { return !util::parse_parent_path(narrow().view()).empty(); }
		NODISCARD bool has_filename() const noexcept { return !util::parse_filename(narrow().view()).empty(); }
		NODISCARD bool has_stem() const noexcept { return !util::parse_stem(narrow().view()).empty(); }
		NODISCARD bool has_extension() const noexcept { return !util::parse_extension(narrow().view()).empty(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string
	using String = BasicString<char>;

	// unicode
	using Unicode = BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class C> constexpr bool is_string_v<BasicString<C>>{ true }; }

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline Unicode operator "" _s(cwstring s, size_t n) noexcept { return Unicode{ s, n }; }

	template <class C> struct Hasher<BasicString<C>> { size_t operator()(BasicString<C> const & s) const noexcept { return s.hash_code(); } };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> const & b) {
		const auto a_size = a.size();
		const auto b_size = b.size();
		if (a.max_size() - a_size < b_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> const & b) {
		using _Traits = typename BasicString<C>::traits_type;
		using _Size_type = typename BasicString<C>::size_type;
		const auto a_size = std::_Convert_size<_Size_type>(_Traits::length(a));
		const auto b_size = b.size();
		if (b.max_size() - b_size < a_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, b, a, a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> const & b) {
		const auto b_size = b.size();
		if (b_size == b.max_size()) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, b, std::addressof(a), 1, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const * const b) {
		using _Traits = typename BasicString<C>::traits_type;
		using _Size_type = typename BasicString<C>::size_type;
		const auto a_size = a.size();
		const auto b_size = std::_Convert_size<_Size_type>(_Traits::length(b));
		if (a.max_size() - a_size < b_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b, b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const b) {
		const auto a_size = a.size();
		if (a_size == a.max_size()) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, std::addressof(b), 1 };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> && b) {
		return b.insert(0, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> const & b) {
		return a.append(b), std::move(a);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> && b) {
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(std::addressof(a) != std::addressof(b),
			"You cannot concatenate the same moved string to itself. See N4910 16.4.5.9 [res.on.arguments]/1.3: "
			"If a function argument is bound to an rvalue reference parameter, the implementation may assume that "
			"this parameter is a unique reference to this argument, except that the argument passed to "
			"a move-assignment operator may be a reference to *this (16.4.6.15 [lib.types.movedfrom]).");
#endif // _ITERATOR_DEBUG_LEVEL == 2
		return { std::_String_constructor_concat_tag{}, a, b };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> && b) {
		return b.insert(0, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> && b) {
		return b.insert(0, 1, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const * const b) {
		return a.append(b), std::move(a);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const b) {
		return a.push_back(b), std::move(a);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator==(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a._Equal(b); }
	template <class C> NODISCARD bool operator==(BasicString<C> const & a, C const * const b) { return a._Equal(b); }
	template <class C> NODISCARD bool operator==(C const * const a, BasicString<C> const & b) { return b._Equal(a); }

	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a == b); }
	template <class C> NODISCARD bool operator!=(C const * const a, BasicString<C> const & b) { return !(a == b); }
	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, C const * const b) { return !(a == b); }

	template <class C> NODISCARD bool operator<(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a.compare(b) < 0; }
	template <class C> NODISCARD bool operator<(C const * const a, BasicString<C> const & b) { return b.compare(a) > 0; }
	template <class C> NODISCARD bool operator<(BasicString<C> const & a, C const * const b) { return a.compare(b) < 0; }

	template <class C> NODISCARD bool operator>(BasicString<C> const & a, BasicString<C> const & b) noexcept { return b < a; }
	template <class C> NODISCARD bool operator>(C const * const a, BasicString<C> const & b) { return b < a; }
	template <class C> NODISCARD bool operator>(BasicString<C> const & a, C const * const b) { return b < a; }

	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(b < a); }
	template <class C> NODISCARD bool operator<=(C const * const a, BasicString<C> const & b) { return !(b < a); }
	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, C const * const b) { return !(b < a); }

	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a < b); }
	template <class C> NODISCARD bool operator>=(C const * const a, BasicString<C> const & b) { return !(a < b); }
	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, C const * const b) { return !(a < b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_STRING_HPP_
