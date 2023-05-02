#ifndef _PNU_CLASS_HPP_
#define _PNU_CLASS_HPP_

#include <core/object/builtins/module_object.hpp>

// class
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// class interface
	template <class T, class ... Options
	> class CLASS_ : public TypeRef
	{
	public:
		using holder_type = std::conditional_t<is_base_object_v<T>, Ref<T>, T>;

		using value_type = std::conditional_t<is_base_object_v<T>, T, typename holder_type::value_type>;
		
		using type = T;

	public:
		CLASS_(TypeRef type) : TypeRef{ type } { ASSERT(is_valid()); }

		template <class ... Args, class ... Extra
		> CLASS_ & def(initimpl::Constructor<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class ... Args, class ... Extra
		> CLASS_ & def(initimpl::Factory<Args...> && init, Extra && ... extra) noexcept
		{
			return FWD(init).execute(*this, FWD(extra)...); // init.hpp
		}

		template <class F, class ... Extra
		> CLASS_ & def(cstring name, F && fn, Extra && ... extra) noexcept
		{
			CppFunctionRef cf({
				method_adaptor<type>(FWD(fn)),
				attr::name(name),
				attr::is_method(*this),
				attr::sibling(getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class F, class ... Extra
		> CLASS_ & def_static(cstring name, F && fn, Extra && ... extra) noexcept
		{
			CppFunctionRef cf({
				FWD(fn),
				attr::name(name),
				attr::scope(*this),
				attr::sibling(getattr(*this, name, nullptr)),
				FWD(extra)... });
			return add_object(cf.name(), cf), (*this);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readwrite(cstring name, D C::*pm, Extra && ... extra) noexcept
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readwrite() requires a class member (or base class member)");
			CppFunctionRef fget({ [pm](type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			CppFunctionRef fset({ [pm](type & c, D const & value) { c.*pm = value; }, attr::is_method(*this) });
			return def_property(name, fget, fset, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class C, class D, class ... Extra
		> CLASS_ & def_readonly(cstring name, D const C::*pm, Extra && ... extra) noexcept
		{
			static_assert(std::is_same_v<C, type> || std::is_base_of_v<C, type>, "def_readonly() requires a class member (or base class member)");
			CppFunctionRef fget({ [pm](type const & c) -> D const & { return c.*pm; }, attr::is_method(*this) });
			return def_property_readonly(name, fget, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readwrite_static(cstring name, D * pm, Extra && ... extra) noexcept
		{
			CppFunctionRef fget({ [pm](ObjectRef) -> D const & { return *pm; }, attr::scope(*this) });
			CppFunctionRef fset({ [pm](ObjectRef, D const & value) { *pm = value; }, attr::scope(*this) });
			return def_property_static(name, fget, fset, ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class D, class ... Extra
		> CLASS_ & def_readonly_static(cstring name, D const * pm, Extra && ... extra) noexcept
		{
			CppFunctionRef fget({ [pm](ObjectRef) -> D const & { return *pm; }, attr::scope(*this) });
			return def_property_readonly_static(name, fget, ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly(cstring name, Getter const & fget, Extra && ... extra) noexcept
		{
			return def_property_readonly(name, CppFunctionRef({ method_adaptor<type>(fget) }), ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly(cstring name, CppFunctionRef const & fget, Extra && ... extra) noexcept
		{
			return def_property(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, Getter const & fget, Extra && ... extra) noexcept
		{
			return def_property_readonly_static(name, CppFunctionRef({ fget }), ReturnValuePolicy_Reference, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property_readonly_static(cstring name, CppFunctionRef const & fget, Extra && ... extra) noexcept
		{
			return def_property_static(name, fget, nullptr, FWD(extra)...);
		}

		template <class Getter, class Setter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, Setter const & fset, Extra && ... extra) noexcept
		{
			return def_property(name, fget, CppFunctionRef({ method_adaptor<type>(fset) }), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property(cstring name, Getter const & fget, CppFunctionRef const & fset, Extra && ... extra) noexcept
		{
			return def_property(name, CppFunctionRef({ method_adaptor<type>(fget) }), fset, ReturnValuePolicy_ReferenceInternal, FWD(extra)...);
		}

		template <class ... Extra
		> CLASS_ & def_property(cstring name, CppFunctionRef const & fget, CppFunctionRef const & fset, Extra && ... extra) noexcept
		{
			return def_property_static(name, fget, fset, attr::is_method(*this), FWD(extra)...);
		}

		template <class Getter, class ... Extra
		> CLASS_ & def_property_static(cstring name, Getter const & fget, CppFunctionRef const & fset, Extra && ... extra) noexcept
		{
			return def_property_static(name, CppFunctionRef({ fget }), fset, ReturnValuePolicy_Reference, FWD(extra)...);
		}
	
		template <class ... Extra
		> CLASS_ & def_property_static(cstring name, CppFunctionRef const & fget, CppFunctionRef const & fset, Extra && ... extra) noexcept
		{
			if (fget) { attr::process_attributes<Extra...>::init(***fget, FWD(extra)...); }
	
			if (fset) { attr::process_attributes<Extra...>::init(***fset, FWD(extra)...); }

			return add_object(name, PropertyRef({ fget, fset })), (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu::priv
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// class installer helper

	template <class T> struct BindClassHelper final
	{
		constexpr BindClassHelper() noexcept = default;

		constexpr auto operator+(CLASS_<T>(*fn)(CLASS_<T>)) const noexcept
		{
			return static_cast<BindClassFunc>(static_cast<void *>(fn));
		}
	};

#define BIND_CLASS(m_class, m_var) \
	(Pnu::priv::BindClassHelper<m_class>{}) + [](Pnu::CLASS_<m_class> m_var) -> Pnu::CLASS_<m_class>

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_CLASS_HPP_
