#ifndef _PN_ACCESSORS_HPP_
#define _PN_ACCESSORS_HPP_

#include <core/object/object_api.hpp>

// accessors
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// accessor
	template <class Policy
	> class Accessor : public ObjectAPI<Accessor<Policy>>
	{
	public:
		using obj_type = typename Ref<Object>;
		using key_type = typename Policy::key_type;

		Accessor(obj_type const & obj, key_type const & key) : m_obj{ obj }, m_key{ key } {}

		Accessor(Accessor const &) = default;

		Accessor(Accessor &&) noexcept = default;

		void operator=(Accessor const & a) && { std::move(*this).operator=(obj_type(a)); }

		void operator=(Accessor const & a) & { operator=(obj_type(a)); }

		template <class Value> decltype(auto) operator=(Value && value) &&
		{
			return Policy::set(m_obj, m_key, object_or_cast(value)), (*this);
		}

		template <class Value> decltype(auto) operator=(Value && value) &
		{
			return (get_cache() = object_or_cast(value)), (*this);
		}

		auto ptr() const { return (Object *)(get_cache().ptr()); }

		template <class T> operator Ref<T>() const { return get_cache(); }

		template <class T> auto cast() const -> T { return get_cache().cast<T>(); }

	protected:
		obj_type & get_cache() const { return BRANCHLESS_IF(!m_cache, m_cache = Policy::get(m_obj, m_key)), m_cache; }

	private:
		obj_type m_obj;
		key_type m_key;
		mutable obj_type m_cache;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// attribute accessor
	template <class T> struct accessor_policies::AttrPolicy
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static auto get(O && o, Index && i) { return getattr(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { setattr(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// item accessor
	template <class T> struct accessor_policies::ItemPolicy
	{
		using key_type = T;

		template <class O = OBJ, class Index = T
		> static auto get(O && o, Index && i) { return getitem(FWD(o), FWD(i)); }

		template <class O = OBJ, class Index = T, class Value = OBJ
		> static void set(O && o, Index && i, Value && v) { setitem(FWD(o), FWD(i), FWD(v)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_ACCESSORS_HPP_
