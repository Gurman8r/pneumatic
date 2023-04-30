#ifndef _PN_CALL_HPP_
#define _PN_CALL_HPP_

#include <core/object/cast.hpp>

// call
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJ call_object(OBJ callable)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (VectorCallFunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, nullptr, 0);
			}
			else if (BinaryFunc tcall{ type->tp_call })
			{
				return tcall(callable, nullptr);
			}
			else
			{
				return nullptr;
			}
		}
	}

	inline OBJ call_object(OBJ callable, LIST args)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (VectorCallFunc vcall{ get_vectorcall_func(type, callable) })
			{
				if (args)
				{
					return vcall(callable, args.data(), args.size());
				}
				else
				{
					return vcall(callable, nullptr, 0);
				}
			}
			else if (BinaryFunc tcall{ type->tp_call })
			{
				return tcall(callable, args);
			}
			else
			{
				return nullptr;
			}
		}
	}

	inline OBJ call_object(OBJ callable, OBJ const * argv, size_t argc)
	{
		if (!callable) { return nullptr; }
		else
		{
			TYPE type{ typeof(callable) };

			if (VectorCallFunc vcall{ get_vectorcall_func(type, callable) })
			{
				return vcall(callable, argv, argc);
			}
			else if (BinaryFunc tcall{ type->tp_call })
			{
				ListObject args{ argv, argv + argc };

				return tcall(callable, &args);
			}
			else
			{
				return nullptr;
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument collector
	template <ReturnValuePolicy_ Policy = ReturnValuePolicy_AutomaticReference
	> struct ArgumentCollector final
	{
		template <class ... Args
		> explicit ArgumentCollector(Args && ... args) noexcept : m_argc{ sizeof...(Args) }
		{
			static_assert(sizeof...(Args) < MAX_ARGUMENTS, "TOO MANY ARGUMENTS");

			mpl::for_args_i([&](size_t i, auto && arg) noexcept
			{
				m_argv[i] = make_caster<decltype(arg)>::cast(FWD(arg), Policy, nullptr);
			}
			, FWD(args)...);
		}

		OBJ const * argv() const { return m_argv; }

		size_t argc() const { return m_argc; }

		OBJ call(OBJ const & callable) noexcept { return call_object(callable, m_argv, m_argc); }

	private:
		OBJ m_argv[MAX_ARGUMENTS]{};
		size_t m_argc{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument loader
	template <class ... Args
	> struct ArgumentLoader final
	{
		template <class Return, class Guard, class Func
		> using return_type = std::conditional_t<std::is_void_v<Return>, mpl::void_type, Return>;

		static constexpr size_t argument_count{ sizeof...(Args) };

		static constexpr auto indices{ std::make_index_sequence<argument_count>() };

		std::tuple<make_caster<Args>...> argcasters{};

		bool load_args(Batch<OBJ, bool> & args) { return impl_load_args(args, indices); }

		template <class Return, class Guard, class Func
		> auto call(Func && func) && -> return_type<Return, mpl::void_type, Return>
		{
			if constexpr (std::is_void_v<Return>)
			{
				std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});

				return mpl::void_type{};
			}
			else
			{
				return std::move(*this).impl_call<Return>(FWD(func), indices, Guard{});
			}
		}

	private:
		static bool impl_load_args(Batch<OBJ, bool> &, std::index_sequence<>) noexcept { return true; }

		template <size_t ... I> bool impl_load_args(Batch<OBJ, bool> & args, std::index_sequence<I...>)
		{
			return !(... || !std::get<I>(argcasters).load(args.get<OBJ>(I), args.get<bool>(I)));
		}

		template <class Return, class Func, size_t ... I, class Guard
		> Return impl_call(Func && func, std::index_sequence<I...>, Guard &&) &&
		{
			return func(cast_op<Args>(std::move(std::get<I>(argcasters)))...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// argument record
	struct ArgumentRecord final
	{
		String name{};

		OBJ value{};

		bool convert{}, none{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function record
	struct FunctionRecord final
	{
		~FunctionRecord() { if (free_data) { free_data(this); } }

		String name{}, signature{};

		OBJ(*impl)(struct FunctionCall &){};

		void * data[3]{};

		void(*free_data)(FunctionRecord *){};

		u16 argument_count{};

		Vector<ArgumentRecord> args{};

		ReturnValuePolicy_ policy{ ReturnValuePolicy_AutomaticReference };

		Object * scope{}, * sibling{};

		FunctionRecord * next{};

		bool is_stateless : 1, is_constructor : 1, is_operator : 1, is_method : 1;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// function call
	struct FunctionCall final
	{
		explicit FunctionCall(FunctionRecord const & record, OBJ const & parent)
			: record{ record }
			, parent{ parent }
			, args{ record.argument_count }
			, try_next_overload{}
		{
		}

		FunctionRecord const & record;
		
		OBJ parent;

		Batch<OBJ, bool> args;

		bool try_next_overload : 1;

		OBJ operator()()
		{
			OBJ result;
			{
				priv::LoaderLifeSupport guard{};
				result = record.impl(*this);
			}
			return result;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class /*Derived*/, class Func
	> auto method_adaptor(Func && func) -> decltype(FWD(func)) { return FWD(func); }

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...)) -> Return(Derived:: *)(Args...)
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	template <class Derived, class Return, class Class, class ... Args
	> auto method_adaptor(Return(Class:: * pmf)(Args...) const) -> Return(Derived:: *)(Args...) const
	{
		static_assert(
			mpl::is_accessible_base_of_v<Class, Derived>,
			"Cannot bind an inaccessible base class method; use a lambda definition instead");
		return pmf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object call operator
	template <class Derived
	> template <ReturnValuePolicy_ Policy, class ...Args
	> inline OBJ ObjectAPI<Derived>::operator()(Args && ... args) const
	{
		return ArgumentCollector<Policy>(FWD(args)...).call(derived().ptr());
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// attr
namespace pn::attr
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process attribute
	template <class T, class SFINAE = void> struct process_attribute;

	// process attribute default
	template <class T> struct process_attribute_default {
		using type = typename T;
		static void init(FunctionRecord &, T &&) noexcept {}
		static void precall(FunctionCall &) noexcept {}
		static void postcall(FunctionCall &, OBJ) noexcept {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// name
	struct name { cstring value; name(cstring value) : value{ value } {} };
	template <> struct process_attribute<name> : process_attribute_default<name> {
		static void init(FunctionRecord & r, name const & a) noexcept { r.name = a.value; }
	};

	// return policy
	template <> struct process_attribute<ReturnValuePolicy_> : process_attribute_default<ReturnValuePolicy_> {
		static void init(FunctionRecord & r, ReturnValuePolicy_ const & a) noexcept { r.policy = a; }
	};

	// sibling
	struct sibling { Object * value; sibling(Object * value) : value{ value } {} sibling(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<sibling> : process_attribute_default<sibling> {
		static void init(FunctionRecord & r, sibling const & a) noexcept { r.sibling = a.value; }
	};

	// is_method
	struct is_method { Object * value; is_method(Object * value) : value{ value } {} is_method(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<is_method> : process_attribute_default<is_method> {
		static void init(FunctionRecord & r, is_method const & a) noexcept { r.is_method = true; r.scope = a.value; }
	};

	// scope
	struct scope { Object * value; scope(Object * value) : value{ value } {} scope(OBJ value) : value{ *value } {} };
	template <> struct process_attribute<scope> : process_attribute_default<scope> {
		static void init(FunctionRecord & r, scope const & a) noexcept { r.scope = a.value; }
	};

	// is_operator
	struct is_operator {};
	template <> struct process_attribute<is_operator> : process_attribute_default<is_operator> {
		static void init(FunctionRecord & r, is_operator const & a) noexcept { r.is_operator = true; }
	};

	// is_constructor
	struct is_constructor {};
	template <> struct process_attribute<is_constructor> : process_attribute_default<is_constructor> {
		static void init(FunctionRecord & r, is_constructor const & a) noexcept { r.is_constructor = true; }
	};

	// prepend
	struct prepend {};
	template <> struct process_attribute<prepend> : process_attribute_default<prepend> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// call guard
	/*
	A call policy which places one or more guard variables (``Ts...``) around the function call.

	For example, this definition:
		m.def("foo", foo, attr::call_guard<T>());

	is equivalent to the following pseudocode:
		m.def("foo", [](args...) {
			T scope_guard;
			return foo(args...); // forwarded arguments
		});
	*/
	template <class... Ts> struct call_guard;

	template <> struct call_guard<> { using type = mpl::void_type; };

	template <class T
	> struct call_guard<T>
	{
		static_assert(std::is_default_constructible_v<T>, "The guard type must be default constructible");

		using type = T;
	};

	template <class T, class ... Ts
	> struct call_guard<T, Ts...>
	{
		struct type
		{
			T guard{}; // compose multiple guard types with left-to-right default-NewFunc order

			typename call_guard<Ts...>::type next{};
		};
	};

	template <class ... Ts
	> struct process_attribute<call_guard<Ts...>> : process_attribute_default<call_guard<Ts...>> {};

	// is call guard
	template <class T
	> using is_call_guard_t = mpl::is_instantiation_t<call_guard, T>;

	// extract call guard
	template <class ... Extra
	> using extract_guard_t = typename mpl::exactly_one_t<is_call_guard_t, call_guard<>, Extra...>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// process attributes
	template <class ... Args> struct process_attributes
	{
		template <class T> static void init(T & r, Args && ... args) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::init(r, FWD(args)), 0) ...);
		}

		static void precall(FunctionCall & call) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::precall(call), 0) ...);
		}

		static void postcall(FunctionCall & call, OBJ retv) noexcept {
			SINK(0, (process_attribute<std::decay_t<Args>>::postcall(call, retv), 0) ...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// initimpl
namespace pn::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// c++ type (Object)
	template <class Class> using Cpp = typename Class::value_type;

	// container type (Ref)
	template <class Class> using Holder = typename Class::holder_type;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// constructor
	template <class ... Args
	> struct Constructor
	{
		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra) noexcept
		{
			return c.def("__new__", [](Args ... args)
			{
				return Holder<Class>::new_(args...);
			}
			, FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> static Class & execute(Class & c, Extra && ... extra) noexcept
		{
			static_assert(!"NOT IMPLEMENTED");
			return c; // TODO
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// factory
	template <class Fn, class R = mpl::function_signature_t<Fn>, class ... Args
	> struct Factory
	{
		std::remove_reference_t<Fn> class_factory;

		Factory(Fn && fn) noexcept : class_factory{ FWD(fn) } {}

		template <class Class, class ... Extra, std::enable_if_t<is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra) noexcept
		{
			return c.def("__new__", [func = std::move(class_factory)](Args ... args)
			{
				return Holder<Class>::new_(func(args...));
			}
			, FWD(extra)...);
		}

		template <class Class, class ... Extra, std::enable_if_t<!is_base_object_v<Cpp<Class>>, int> = 0
		> Class & execute(Class & c, Extra && ... extra) noexcept
		{
			static_assert(!"NOT IMPLEMENTED");
			return c; // TODO
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// init
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> auto init() -> initimpl::Constructor<Args...> { return {}; }

	template <class Func, class Ret = initimpl::Factory<Func>
	> auto init(Func && fn) -> Ret { return { FWD(fn) }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_CALL_HPP_
