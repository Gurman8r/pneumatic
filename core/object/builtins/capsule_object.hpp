#ifndef _PNU_CAPSULE_OBJECT_HPP_
#define _PNU_CAPSULE_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// capsule
namespace Pnu
{
	// capsule object
	class PNU_API CapsuleObject : public Object
	{
		DEFINE_CLASS(CapsuleObject, Object);

		friend class CapsuleRef;

	public:
		void const * m_pointer{}, * m_context{};

		DelFunc m_closure{};

		virtual ~CapsuleObject() override { if (m_closure) { m_closure(this); } }

		CapsuleObject() noexcept {}

		CapsuleObject(nullptr_t) noexcept {}

		CapsuleObject(void const * value, DelFunc closure = nullptr)
		{
			m_pointer = value;
			m_context = nullptr;
			m_closure = closure;
		}

		CapsuleObject(void const * value, void(*closure)(void *))
		{
			m_pointer = value;
			m_context = closure;
			m_closure = [](Object * obj)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(obj) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_context) };

					closure((void *)self->m_pointer);
				}
			};
		}

		CapsuleObject(void(*closure)())
		{
			m_pointer = closure;
			m_context = nullptr;
			m_closure = (DelFunc)[](Object * obj)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(obj) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->m_pointer) };

					closure();
				}
			};
		}
	};

	// capsule delete
	template <> struct DefaultDelete<CapsuleObject> : DefaultDelete<Object> {};

	// capsule check
#define OBJECT_CHECK_CAPSULE(o) (isinstance<CapsuleRef>(o))

	// capsule ref
	class CapsuleRef : public Ref<CapsuleObject>
	{
		REF_CLASS(CapsuleRef, OBJECT_CHECK_CAPSULE);

	public:
		template <class T
		> operator T * () const { return get_pointer<T>(); }

		template <class T = void
		> auto get_pointer() const { return static_cast<T *>((void *)VALIDATE(m_ptr)->m_pointer); }

		void set_pointer(void const * value) const { VALIDATE(m_ptr)->m_pointer = value; }
	};
}

#endif // !_PNU_CAPSULE_OBJECT_HPP_
