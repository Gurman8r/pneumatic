#ifndef _PNU_STRING_HPP_
#define _PNU_STRING_HPP_

#include <core/templates/optional.hpp>
#include <core/string/string_view.hpp>
#include <core/templates/vector.hpp>
#include <string>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string base
	template <class C = char
	> using _StringBase = std::basic_string<C, std::char_traits<C>, PolymorphicAllocator<C>>;

	// basic string
	template <class C
	> class BasicString
	{
	public:
		using self_type					= typename BasicString<C>;
		using base_type					= typename _StringBase<C>;
		using value_type				= typename base_type::value_type;
		using traits_type				= typename base_type::traits_type;
		using pointer					= typename base_type::pointer;
		using const_pointer				= typename base_type::const_pointer;
		using reference					= typename base_type::reference;
		using const_reference			= typename base_type::const_reference;
		using iterator					= typename base_type::iterator;
		using const_iterator			= typename base_type::const_iterator;
		using reverse_iterator			= typename base_type::reverse_iterator;
		using const_reverse_iterator	= typename base_type::const_reverse_iterator;
		using size_type					= typename base_type::size_type;
		using difference_type			= typename base_type::difference_type;
		using allocator_type			= typename base_type::allocator_type;

		static constexpr size_type npos{ static_cast<size_type>(-1) };

		static constexpr bool is_narrow{ 1 == sizeof(C) };

	public:
		base_type m_string{};

		BasicString() noexcept : m_string{} {}

		explicit BasicString(allocator_type const & alloc) : m_string{ alloc } {}

		explicit BasicString(base_type const & value) : m_string{ value } {}

		explicit BasicString(base_type && value) : m_string{ std::move(value) } {}

		BasicString(self_type const & value) : m_string{ value.m_string } {}

		BasicString(self_type const & value, allocator_type const & alloc) : m_string{ value.m_string, alloc } {}

		BasicString(self_type const & value, size_type const offset, allocator_type const & alloc) : m_string{ value.m_string, offset, alloc } {}

		BasicString(self_type const & value, size_type const offset, size_type const count, allocator_type const & alloc) : m_string{ value.m_string, offset, count, alloc } {}

		BasicString(const_pointer const data, size_type const size) : m_string{ data, size } {}

		BasicString(const_pointer const data, size_type const size, allocator_type const & alloc) : m_string{ data, size, alloc } {}

		BasicString(const_pointer const data) : m_string{ data } {}

		BasicString(const_pointer const data, allocator_type const & alloc) : m_string{ data, alloc } {}

		BasicString(size_type const count, value_type const elem, allocator_type const & alloc) : m_string{ count, elem, alloc } {}

		template <class Iter, std::enable_if_t<std::_Is_iterator_v<Iter>, int> = 0
		> BasicString(Iter first, Iter last, allocator_type const & alloc = {}) : m_string{ first, last, alloc } {}
		
		BasicString(self_type && value) noexcept : m_string{ std::move(value.m_string) } {}

		BasicString(self_type && value, allocator_type const & alloc) noexcept : m_string{ std::move(value.m_string), alloc } {}

		BasicString(BasicStringView<value_type> const view, allocator_type const & alloc = {}) : m_string{ view.data(), view.size(), alloc } {}

		BasicString(std::initializer_list<value_type> value, allocator_type const & alloc = {}) : m_string{ value, alloc } {}

	public:
		NODISCARD auto operator[](size_type const i) noexcept -> reference { return m_string[i]; }

		NODISCARD auto operator[](size_type const i) const noexcept -> const_reference { return m_string[i]; }

		NODISCARD auto at(size_type const i) -> reference { return m_string.at(i); }

		NODISCARD auto at(size_type const i) const -> const_reference { return m_string.at(i); }

		NODISCARD auto back() noexcept -> reference { return m_string.back(); }

		NODISCARD auto back() const noexcept -> const_reference { return m_string.back(); }

		NODISCARD auto front() noexcept -> reference { return m_string.front(); }

		NODISCARD auto front() const noexcept -> const_reference { return m_string.front(); }

		NODISCARD auto c_str() const noexcept -> const_pointer { return m_string.c_str(); }

		NODISCARD auto data() noexcept -> pointer { return m_string.data(); }

		NODISCARD auto data() const noexcept -> const_pointer { return m_string.data(); }

		NODISCARD bool empty() const noexcept { return m_string.empty(); }

		NODISCARD auto get_allocator() const noexcept -> allocator_type { return m_string.get_allocator(); }

		NODISCARD auto max_size() const noexcept -> size_type { return m_string.max_size(); }

		NODISCARD auto size() const noexcept -> size_type { return m_string.size(); }

		NODISCARD auto native() & -> base_type & { return m_string; }

		NODISCARD auto native() && -> base_type && { return std::move(m_string); }

		NODISCARD auto native() const & -> base_type const & { return m_string; }

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }

		NODISCARD auto hash_code() const noexcept { return hash_representation(data(), size()); }

		NODISCARD auto view() const noexcept { return BasicStringView<value_type>{ data(), size() }; }

		NODISCARD operator BasicStringView<value_type>() const noexcept { return { data(), size() }; }

	public:
		NODISCARD auto begin() noexcept -> iterator { return m_string.begin(); }

		NODISCARD auto end() noexcept -> iterator { return m_string.end(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return m_string.begin(); }

		NODISCARD auto end() const noexcept -> const_iterator { return m_string.end(); }

		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_string.begin(); }

		NODISCARD auto cend() const noexcept -> const_iterator { return m_string.begin(); }

		NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_string.rbegin(); }

		NODISCARD auto rend() noexcept -> reverse_iterator { return m_string.rend(); }

		NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_string.rbegin(); }

		NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_string.rend(); }

		NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_string.crbegin(); }

		NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_string.crend(); }

	public:
		self_type & assign(self_type && value) noexcept { return (m_string = std::move(value.m_string)), (*this); }

		self_type & assign(std::initializer_list<value_type> value) { return m_string.assign(value), (*this); }

		self_type & assign(self_type const & value) { return (m_string = value.m_string), (*this); }

		self_type & assign(const_pointer const value) { return (m_string = value), (*this); }

		self_type & operator=(std::initializer_list<value_type> const value) { return (m_string = value), (*this); }

		self_type & operator=(self_type && value) noexcept { return (m_string = std::move(value.m_string)), (*this); }

		self_type & operator=(self_type const & value) { return (m_string = value.m_string), (*this); }

		self_type & operator=(const_pointer const value) { return (m_string = value), (*this); }

	public:
		self_type & append(std::initializer_list<value_type> value) { return m_string.append(value), (*this); }

		self_type & append(self_type const & value) { return m_string.append(value.m_string), (*this); }
		
		self_type & append(const_pointer const value) { return m_string.append(value), (*this); }
		
		self_type & append(value_type const value) { return m_string.push_back(value), (*this); }

		self_type & operator+=(std::initializer_list<value_type> const value) { return (m_string += value), (*this); }

		self_type & operator+=(self_type const & value) { return (m_string += value.m_string), (*this); }

		self_type & operator+=(const_pointer const value) { return (m_string += value), (*this); }

		self_type & operator+=(value_type const value) { return (m_string += value), (*this); }

	public:
		self_type & clear() noexcept { return m_string.clear(), (*this); }

		self_type & erase(size_type const offset = 0) { return m_string.erase(offset), (*this); }

		self_type & erase(size_type const offset, size_type const count) { return m_string.erase(offset, count), (*this); }

		self_type & erase(const_iterator first) { return m_string.erase(offset, count), (*this); }

		self_type & erase(const_iterator first, const_iterator last) { return m_string.erase(first, last), (*this); }

		self_type & erase_duplicates(value_type const a) { return m_string.erase(std::unique(begin(), end(), [a](value_type a, value_type b) { return (a == b) && (a == a); }), end()), (*this); }

	public:
		self_type & insert(size_type const index, value_type const value) { return m_string.insert(index, value), (*this); }
		
		self_type & insert(iterator it, value_type const value) { return m_string.insert(index, value), (*this); }
		
		self_type & insert(const_iterator it, value_type const value) { return m_string.insert(index, value), (*this); }

		self_type & insert(size_type const index, self_type const & value) { return m_string.insert(index, value.m_string), (*this); }
		
		self_type & insert(iterator it, self_type const & value) { return m_string.insert(index, value.m_string), (*this); }
		
		self_type & insert(const_iterator it, self_type const & value) { return m_string.insert(index, value.m_string), (*this); }

	public:
		self_type & push_front(value_type const value) noexcept { return m_string.insert(begin(), value), (this); }

		self_type & push_back(value_type const value) noexcept { return m_string.push_back(value), (*this); }

		self_type & pop_front() { return m_string.erase(begin()), (*this); }

		self_type & pop_back() { return m_string.pop_back(), (*this); }

		self_type & resize(size_type const count, value_type const elem = value_type{}) { return m_string.resize(count, elem), (*this); }

		self_type & reserve(size_type const count) { return m_string.reserve(count), (*this); }

		self_type & shrink_to_fit() noexcept { return m_string.shrink_to_fit(), (*this); }

		self_type & swap(self_type & value) noexcept { return BRANCHLESS_IF((this != std::addressof(value)), m_string.swap(value.m_string)), (*this); }

		template <class Fn = value_type(*)(value_type)
		> self_type & transform(Fn fn) noexcept { return std::transform(begin(), end(), begin(), fn), (*this); }

	public:
		NODISCARD self_type substr(size_type const offset, size_type const count = npos) const { return self_type{ m_string.substr(offset, count) }; }

		NODISCARD auto compare(self_type const & value) const noexcept { if (this == std::addressof(value)) { return 0; } return m_string.compare(value.m_string); }

		NODISCARD bool equal_to(self_type const & value) const noexcept { return (this == std::addressof(value)) || m_string._Equal(value.m_string); }

	public:
		NODISCARD size_type find(value_type const value, size_type const offset = 0) const { return m_string.find(value, offset); }
		
		NODISCARD size_type find(self_type const & value, size_type const offset = 0) const { return m_string.find(value.m_string, offset); }

		NODISCARD size_type rfind(value_type const value, size_type const offset = 0) const { return m_string.rfind(value, offset); }
		
		NODISCARD size_type rfind(self_type const & value, size_type const offset = 0) const { return m_string.rfind(value.m_string, offset); }

		NODISCARD size_type find_first_of(value_type const value, size_type const offset = 0) const { return m_string.find_first_of(value, offset); }
		
		NODISCARD size_type find_first_of(self_type const & value, size_type const offset = 0) const { return m_string.find_first_of(value.m_string, offset); }

		NODISCARD size_type find_first_not_of(value_type const value, size_type const offset = 0) const { return m_string.find_first_not_of(value, offset); }
		
		NODISCARD size_type find_first_not_of(self_type const & value, size_type const offset = 0) const { return m_string.find_first_not_of(value.m_string, offset); }

		NODISCARD size_type find_last_of(value_type const value, size_type const offset = 0) const { return m_string.find_last_of(value, offset); }
		
		NODISCARD size_type find_last_of(self_type const & value, size_type const offset = 0) const { return m_string.find_last_of(value.m_string, offset); }

		NODISCARD size_type find_last_not_of(value_type const value, size_type const offset = 0) const { return m_string.find_last_not_of(value, offset); }
		
		NODISCARD size_type find_last_not_of(self_type const & value, size_type const offset = 0) const { return m_string.find_last_not_of(value.m_string, offset); }

		NODISCARD bool contains(value_type const value) const noexcept { return m_string.find(value) != npos; }
		
		NODISCARD bool contains(self_type const & value) const noexcept { return m_string.find(value.m_string) != npos; }

		NODISCARD bool has_prefix(value_type const a) const noexcept { return !empty() && front() == a; }
		
		NODISCARD bool has_prefix(self_type const & a) const noexcept { return (a.size() <= size()) && (a == substr(0, a.size())); }

		NODISCARD bool has_suffix(value_type const a) const noexcept { return !empty() && back() == a; }
		
		NODISCARD bool has_suffix(self_type const & a) const noexcept { return (a.size() <= size()) && (a == substr(size() - a.size(), a.size())); }

	public:
		NODISCARD auto narrow() const noexcept -> std::conditional_t<is_narrow, BasicString<char> const &, BasicString<char>> {
			if constexpr (is_narrow) {
				return (*this);
			}
			else {
				BasicString<char> temp;
				temp.reserve(size());
				for (auto const c : *this) { temp.push_back(static_cast<char>(c)); }
				return temp;
			}
		}

		NODISCARD auto widen() const -> std::conditional_t<!is_narrow, BasicString<wchar_t> const &, BasicString<wchar_t>> {
			if constexpr (!is_narrow) {
				return (*this);
			}
			else {
				BasicString<wchar_t> temp;
				temp.reserve(size());
				for (auto const c : *this) { temp.push_back(static_cast<wchar_t>(c)); }
				return temp;
			}
		}

	public:
		NODISCARD self_type to_upper() const {
			self_type temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(std::toupper(c)); }
			return temp;
		}

		NODISCARD self_type to_lower() const {
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
			while (!empty() && fn(front())) { pop_front(); }
			return (*this);
		}

		template <class Fn = int(*)(int)
		> auto & trim(Fn fn = std::isspace) {
			while (!empty() && fn(back())) { pop_back(); }
			while (!empty() && fn(front())) { pop_front(); }
			return (*this);
		}

	public:
		NODISCARD static auto split(self_type s, value_type const delimiter) {
			Vector<self_type> v{};
			size_type i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back(s.substr(0, i));
				s.erase(0, i + 1);
			}
			v.push_back(s);
			return v;
		}

		NODISCARD static auto split(self_type s, cstring delimiter) {
			Vector<self_type> v{};
			size_type i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back(s.substr(0, i));
				s.erase(0, i + std::strlen(delimiter));
			}
			v.push_back(s);
			return v;
		}

		NODISCARD static auto split(self_type s, self_type const & delimiter) {
			Vector<self_type> v{};
			size_type i{};
			while ((i = s.find(delimiter)) != npos) {
				v.push_back(s.substr(0, i));
				s.erase(0, i + delimiter.size());
			}
			v.push_back(s);
			return v;
		}

		NODISCARD auto split(value_type const delimiter) const noexcept { return split(*this, delimiter); }

		NODISCARD auto split(value_type const * delimiter) const noexcept { return split(*this, delimiter); }

		NODISCARD auto split(self_type const & delimiter) const noexcept { return split(*this, delimiter); }

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

	public:
		template <size_type buffer_size = 0
		> static i32 format(self_type & s, const_pointer fmt, va_list args)
		{
			// select the correct formatter
			auto _formatter = [](auto ... v) noexcept {
				if constexpr (is_narrow) { return std::vsnprintf(v...); }
				else { return std::vswprintf(v...); }
			};

			// static buffer size
			if constexpr (0 < buffer_size)
			{
				value_type buffer[buffer_size]{};
				i32 const ns{ _formatter(buffer, buffer_size, fmt, args) };
				if (0 < ns) {
					s = self_type{ buffer, buffer + (size_type)ns + 1, allocator_type{} };
				}
				return ns;
			}
			// dynamic buffer size
			else
			{
				va_list args_copy;
				va_copy(args_copy, args);
				i32 const ns{ _formatter(nullptr, 0, fmt, args_copy) };
				va_end(args_copy);
				if (0 < ns) {
					s.resize((size_type)ns + 1);
					_formatter(s.data(), s.size(), fmt, args);
				}
				return ns;
			}
		}

		template <size_type buffer_size = 0
		> static i32 format(self_type & s, const_pointer fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			i32 const n{ format<buffer_size>(s, fmt, args) };
			va_end(args);
			return n;
		}

		template <size_type buffer_size = 0
		> NODISCARD static self_type format(const_pointer fmt, va_list args) noexcept {
			self_type s;
			format<buffer_size>(s, fmt, args);
			return s;
		}

		template <size_type buffer_size = 0
		> NODISCARD static self_type format(const_pointer fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			auto const s{ format<buffer_size>(fmt, args) };
			va_end(args);
			return s;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string
	using String = BasicString<char>;

	// unicode (wide string)
	using Unicode = BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class C> constexpr bool is_string_v<BasicString<C>>{ true }; }

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline Unicode operator "" _s(cwstring s, size_t n) noexcept { return Unicode{ s, n }; }

	template <class C> struct Hasher<BasicString<C>> { size_t operator()(BasicString<C> const & s) const noexcept { return s.hash_code(); } };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> const & b) { return BasicString<C>{ a.m_string + b.m_string }; }
	
	template <class C> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> const & b) { return BasicString<C>{ a + b.m_string }; }
	
	template <class C> NODISCARD BasicString<C> operator+(C const a, BasicString<C> const & b) { return BasicString<C>{ a + b.m_string }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const * const b) { return BasicString<C>{ a.m_string + b }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const b) { return BasicString<C>{ a.m_string + b }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> && b) { return BasicString<C>{ a.m_string + std::move(b.m_string) }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> const & b) { return BasicString<C>{ std::move(a.m_string) + b.m_string }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> && b) { return BasicString<C>{ std::move(a.m_string) + std::move(b.m_string) }; }
	
	template <class C> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> && b) { return BasicString<C>{ a + std::move(b.m_string) }; }
	
	template <class C> NODISCARD BasicString<C> operator+(C const a, BasicString<C> && b) { return BasicString<C>{ a + std::move(b.m_string) }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const * const b) { return BasicString<C>{ std::move(a.m_string) + b }; }
	
	template <class C> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const b) { return BasicString<C>{ std::move(a.m_string) + b }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator==(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a.equal_to(b); }

	template <class C> NODISCARD bool operator==(BasicString<C> const & a, C const * const b) { return a.equal_to(b); }

	template <class C> NODISCARD bool operator==(C const * const a, BasicString<C> const & b) { return b.equal_to(a); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a == b); }
	
	template <class C> NODISCARD bool operator!=(C const * const a, BasicString<C> const & b) { return !(a == b); }
	
	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, C const * const b) { return !(a == b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator<(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a.compare(b) < 0; }
	
	template <class C> NODISCARD bool operator<(C const * const a, BasicString<C> const & b) { return b.compare(a) > 0; }
	
	template <class C> NODISCARD bool operator<(BasicString<C> const & a, C const * const b) { return a.compare(b) < 0; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator>(BasicString<C> const & a, BasicString<C> const & b) noexcept { return b < a; }
	
	template <class C> NODISCARD bool operator>(C const * const a, BasicString<C> const & b) { return b < a; }
	
	template <class C> NODISCARD bool operator>(BasicString<C> const & a, C const * const b) { return b < a; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(b < a); }
	
	template <class C> NODISCARD bool operator<=(C const * const a, BasicString<C> const & b) { return !(b < a); }
	
	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, C const * const b) { return !(b < a); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a < b); }
	
	template <class C> NODISCARD bool operator>=(C const * const a, BasicString<C> const & b) { return !(a < b); }
	
	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, C const * const b) { return !(a < b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

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
		char			buf[21]{ /* can hold 2^64 - 1, plus NUL */ };
		char * const	end{ std::end(buf) };
		char * next{ end };
		auto const		uval{ static_cast<std::make_unsigned_t<T>>(value) };
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
		auto const n{ static_cast<size_t>(_scprintf("%f", value)) };
		String temp{ n, '\0', String::allocator_type{} };
		sprintf_s(temp.data(), n + 1, "%f", value);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Optional<i8> to_i8(String const & value, i32 base = 10) noexcept { return parse_answer<i8>(value.c_str(), &strtol, base); }

	inline Optional<i16> to_i16(String const & value, i32 base = 10) noexcept { return parse_answer<i16>(value.c_str(), &strtol, base); }

	inline Optional<i32> to_i32(String const & value, i32 base = 10) noexcept { return parse_answer<i32>(value.c_str(), &strtol, base); }

	inline Optional<i64> to_i64(String const & value, i32 base = 10) noexcept { return parse_answer<i64>(value.c_str(), &strtoll, base); }

	inline Optional<u8> to_u8(String const & value, i32 base = 10) noexcept { return parse_answer<u8>(value.c_str(), &strtoul, base); }

	inline Optional<u16> to_u16(String const & value, i32 base = 10) noexcept { return parse_answer<u16>(value.c_str(), &strtoul, base); }

	inline Optional<u32> to_u32(String const & value, i32 base = 10) noexcept { return parse_answer<u32>(value.c_str(), &strtoul, base); }

	inline Optional<u64> to_u64(String const & value, i32 base = 10) noexcept { return parse_answer<u64>(value.c_str(), &strtoull, base); }

	inline Optional<f32> to_f32(String const & value) noexcept { return parse_answer<f32>(value.c_str(), &strtod); }

	inline Optional<f64> to_f64(String const & value) noexcept { return parse_answer<f64>(value.c_str(), &strtod); }

	inline Optional<f80> to_f80(String const & value) noexcept { return parse_answer<f80>(value.c_str(), &strtold); }

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

namespace Pnu::util
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

#endif // !_PNU_STRING_HPP_