#ifndef _PN_MPL_HPP_
#define _PN_MPL_HPP_

#include <core/templates/tuple.hpp>

namespace pn::mpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type_list
	template <class ... Ts
	> struct type_list { static constexpr size_t size{ sizeof...(Ts) }; };

	// type_tag
	template <class T
	> struct type_tag { using type = typename T; };

#define TAG_TYPE(tag) std::decay_t<decltype(tag)::type>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn, size_t ... Is
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn, std::index_sequence<Is...>) noexcept
	{
		fn(std::get<Is>(tp)...);
	}

	template <class Tup, class Fn
	> constexpr void impl_tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(
			FWD(tp),
			FWD(fn),
			std::make_index_sequence<std::tuple_size_v<std::decay_t<Tup>>>());
	}

	// "unpacks" the contents of a tuple inside a function call
	template <class Tup, class Fn
	> constexpr void tuple_expand(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(FWD(tp), FWD(fn));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn, class ... Args
	> constexpr void impl_for_args(Fn && fn, Args && ... args) noexcept
	{
		if constexpr (0 < sizeof...(args)) // sanity check
		{
			(void)std::initializer_list<int>{ (fn(FWD(args)), 0)... };
		}
	}

	// invoke function on every argument
	template <class Fn, class ... Args
	> constexpr void for_args(Fn && fn, Args && ... args) noexcept
	{
		mpl::impl_for_args(FWD(fn), FWD(args)...);
	}

	// invoke function on every argument with index
	template <class Fn, class ... Args
	> constexpr void for_args_i(Fn && fn, Args && ... args) noexcept
	{
		size_t i{};
		mpl::for_args([&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		}
		, FWD(args)...);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Tup, class Fn
	> constexpr void impl_for_tuple(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_tuple_expand(FWD(tp), [&fn](auto && ... rest) noexcept
		{
			mpl::for_args(fn, FWD(rest)...);
		});
	}

	// invoke function on every tuple element
	template <class Tup, class Fn
	> constexpr void for_tuple(Tup && tp, Fn && fn) noexcept
	{
		mpl::impl_for_tuple(FWD(tp), FWD(fn));
	}

	// invoke function on every tuple element with index
	template <class Tup, class Fn
	> constexpr void for_tuple_i(Tup && tp, Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_tuple(FWD(tp), [&](auto && e) noexcept
		{
			fn(i++, FWD(e));
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* CONCAT - combine types */

	template <class ...
	> struct impl_concat
	{
		using type = typename type_list<>;
	};

	template <class ... Ts
	> using concat = typename impl_concat<Ts...>::type;

	template <class ... Ts
	> struct impl_concat<type_list<Ts...>>
	{
		using type = typename type_list<Ts...>;
	};

	template <class ... Ts0, class ... Ts1, class ... Rest
	> struct impl_concat<type_list<Ts0...>, type_list<Ts1...>, Rest...>
	{
		using type = typename concat<type_list<Ts0..., Ts1...>, Rest...>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* REMAP - modify inner types */

	template <template <class> class Pr, class
	> struct impl_remap
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> using remap = typename impl_remap<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_remap<Pr, type_list<T, Ts...>>
	{
		using type = typename concat<type_list<Pr<T>>, remap<Pr, type_list<Ts...>>>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* RENAME - modify outer type */

	template <template <class...> class To, class T
	> struct impl_rename;

	template <
		template <class ...> class To,
		template <class...> class From,
		class ... Ts
	> struct impl_rename<To, From<Ts...>>
	{
		using type = typename To<Ts...>;
	};

	template<
		template <class...> class To, class T
	> using rename = typename impl_rename<To, T>::type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* CONTAINS - check type_list contains type */

	template <class T, class Ls
	> struct contains;

	template <class T
	> struct contains<T, type_list<>>
		: std::false_type {};

	template <class T, class U, class ... Ts
	> struct contains<T, type_list<U, Ts...>>
		: contains<T, type_list<Ts...>> {};

	template <class T, class ... Ts
	> struct contains<T, type_list<T, Ts...>>
		: std::true_type {};

	template <class T, class Ls
	> constexpr bool contains_v{ contains<T, Ls>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* INDEX OF - get index of type in a type_list */

	template <class T, class Ls
	> struct index_of;

	template <class T, class ... Ts
	> struct index_of<T, type_list<T, Ts...>>
		: std::integral_constant<size_t, 0> {};

	template <class T, class U, class ... Ts
	> struct index_of<T, type_list<U, Ts...>>
		: std::integral_constant<size_t, 1 + index_of<T, type_list<Ts...>>::value> {};

	template <class T, class Ls
	> constexpr auto index_of_v{ index_of<T, Ls>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	template <class Ls
	> using tuple = rename<std::tuple, Ls>;
	
	template <class Ls
	> using tag_tuple = tuple<remap<type_tag, Ls>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls
	> constexpr size_t size() noexcept { return Ls::size; }

	template <class Ls, class T> using push_back = concat<Ls, type_list<T>>;

	template <class Ls, class T> using push_front = concat<type_list<T>, Ls>;

	template <size_t I, class Ls> using nth = std::tuple_element_t<I, tuple<Ls>>;

	template <class Ls> using head = nth<0, Ls>;

	template <class Ls> using tail = nth<size<Ls>() - 1, Ls>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ls, class Fn
	> constexpr void for_type_list(Fn && fn) noexcept
	{
		if constexpr (0 < mpl::size<Ls>()) // sanity check
		{
			mpl::for_tuple(mpl::tag_tuple<Ls>{}, fn);
		}
	}

	template <class Ls, class Fn
	> constexpr void for_type_list_i(Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_type_list<Ls>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Ts, class Fn
	> constexpr void for_types(Fn && fn) noexcept
	{
		mpl::for_type_list<mpl::type_list<Ts...>>(FWD(fn));
	}

	template <class ... Ts, class Fn
	> constexpr void for_types_i(Fn && fn) noexcept
	{
		size_t i{};
		mpl::for_types<Ts...>([&](auto tag) noexcept
		{
			fn(i++, tag);
		});
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* REPEAT - size N type_list of T */

	template <size_t N, class T
	> struct impl_repeat
	{
		using type = typename push_back<typename impl_repeat<N - 1, T>::type, T>;
	};

	template <class T
	> struct impl_repeat<0, T>
	{
		using type = typename type_list<>;
	};

	template <size_t N, class T
	> using repeat = typename impl_repeat<N, T>::type;

	template <class T, size_t N
	> using array = tuple<repeat<N, T>>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	/* FILTER - remove types from type_list */

	template <template <class> class Pr, class
	> struct impl_filter
	{
		using type = typename type_list<>;
	};

	template <template <class> class Pr, class Ls
	> using filter = typename impl_filter<Pr, Ls>::type;

	template <template <class> class Pr, class T, class ... Ts
	> struct impl_filter<Pr, type_list<T, Ts...>>
	{
		using next = typename filter<Pr, type_list<Ts...>>;

		using type = typename std::conditional_t<
			(Pr<T>{}),
			concat<type_list<T>, next>,
			next
		>;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_MPL_HPP_
