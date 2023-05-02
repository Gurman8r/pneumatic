#ifndef _PNU_OBJECT_API_HPP_
#define _PNU_OBJECT_API_HPP_

#include <core/object/common.hpp>

// object_api
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Derived
	> class ObjectAPI : public _ObjectAPI_Tag
	{
		auto derived() const & noexcept -> Derived const & { return static_cast<Derived const &>(*this); }

	public:
		template <class Index = cstring
		> auto attr(Index && i) const { return AttrAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Index = cstring
		> auto operator[](Index && i) const { return ItemAccessor<Index>{ derived().ptr(), FWD(i) }; }

		template <class Value = ObjectRef
		> bool contains(Value && value) const { return attr("__contains__")(FWD(value)).cast<bool>(); }

		template <ReturnValuePolicy_ policy = ReturnValuePolicy_AutomaticReference, class ... Args
		> ObjectRef operator()(Args && ... args) const; // call.hpp

	public:
		bool is(ObjectAPI const & other) const noexcept { return derived().ptr() == other.derived().ptr(); }

		template <class O, class = std::enable_if_t<is_base_object_v<O>>
		> bool is(O const * other) const noexcept { return derived().ptr() == other; }

		template <class O, class = std::enable_if_t<is_object_api_v<O>>
		> bool is(O const & other) const noexcept { return derived().ptr() == other.ptr(); }
		
	public:
		auto hash_code() const
		{
			Object * const self{ derived().ptr() };
			HashFunc const hasher{ typeof(self)->tp_hash };
			return hasher ? hasher(self) : Hasher<intptr_t>{}((intptr_t)self);
		}

		auto rich_compare(ObjectAPI const & o) const
		{
			if (Object * const self{ derived().ptr() }, * other{ o.derived().ptr() }
			; self == other)
			{
				return 0;
			}
			else if (!self || !other)
			{
				return compare((intptr_t)self, (intptr_t)other);
			}
			else if (CmpFunc cmp{ typeof(self)->tp_cmp })
			{
				return cmp(self, other);
			}
			else
			{
				return compare(self->hash_code(), other->hash_code());
			}
		}

		bool equal_to(ObjectAPI const & other) const noexcept { return rich_compare(other) == 0; }
		
		bool not_equal_to(ObjectAPI const & other) const noexcept { return rich_compare(other) != 0; }
		
		bool less(ObjectAPI const & other) const noexcept { return rich_compare(other) < 0; }
		
		bool less_equal(ObjectAPI const & other) const noexcept { return rich_compare(other) <= 0; }
		
		bool greater(ObjectAPI const & other) const noexcept { return rich_compare(other) > 0; }
		
		bool greater_equal(ObjectAPI const & other) const noexcept { return rich_compare(other) >= 0; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> bool operator==(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.equal_to(b); }

	template <class T
	> bool operator!=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.not_equal_to(b); }

	template <class T
	> bool operator<(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less(b); }

	template <class T
	> bool operator<=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less_equal(b); }

	template <class T
	> bool operator>(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater(b); }

	template <class T
	> bool operator>=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher<ObjectAPI<T>>
	{
		template <class U> size_t operator()(U const & o) const { return o.hash_code(); }
	};

	template <class T> struct EqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<ObjectAPI<T>>
	{
		template <class A, class B> bool operator()(A const & a, B const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_OBJECT_API_HPP_
