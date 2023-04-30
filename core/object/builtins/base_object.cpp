#include <core/object/builtins/base_object.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Object, t, TypeFlags_IsAbstract)
	{
		t.tp_getattro = (GetAttrOFunc)&Object::generic_getattr;

		t.tp_setattro = (SetAttrOFunc)&Object::generic_setattr;

		t.tp_bind = BIND_CLASS(Object, t)
		{
			return t
				.def("init_ref", &Object::init_ref)
				.def("inc_ref", &Object::inc_ref)
				.def("dec_ref", &Object::dec_ref)
				.def("get_ref_count", &Object::get_ref_count)
				.def("has_references", &Object::has_references)
				.def("notification", &Object::notification)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Object::initialize_class()
	{
		static ON_SCOPE_ENTER(&) {
			get_internals()->add_class(&__type_static);
			ASSERT(__type_static.tp_bind);
			ASSERT(__type_static.tp_bind(&__type_static));
		};
	}

	void Object::_initialize_classv()
	{
		initialize_class();
	}

	bool Object::_predelete()
	{
		notification(Notification_PreDelete, true);
		return true;
	}

	void Object::_postinitialize()
	{
		notification(Notification_PostInitialize);
	}

	void Object::_initialize_object()
	{
		m_refcount.init();
		m_refcount_init.init();
	}

	void Object::_finalize_object()
	{
		m_type = nullptr;
	}

	TYPE Object::_get_typev() const noexcept
	{
		return get_type_static();
	}

	TYPE Object::get_type_static() noexcept
	{
		return &__type_static;
	}

	TYPE Object::get_type() const noexcept
	{
		return BRANCHLESS_IF(!m_type, m_type = _get_typev()), m_type;
	}

	void Object::set_type(TYPE const & value) noexcept
	{
		m_type = value;
	}

	bool Object::init_ref()
	{
		if (inc_ref())
		{
			if (!has_references() && m_refcount_init.dec_ref())
			{
				// first referencing is already 1, so compensate for the ref above
				dec_ref();
			}
			return true;
		}
		return false;
	}

	bool Object::inc_ref()
	{
		u32 const rc_val{ m_refcount.refval() };
		bool const ok{ rc_val != 0 };
		if (ok && rc_val <= 2 /* higher is not relevant */)
		{
			/* nothing to do */
		}
		return ok;
	}

	bool Object::dec_ref()
	{
		u32 const rc_val{ m_refcount.unrefval() };
		bool const die{ rc_val == 0 };
		if (rc_val <= 1 /* higher is not relevant */)
		{
			/* nothing to do */
		}
		return die;
	}

	void Object::notification(Notification_ id, bool reversed)
	{
		_notificationv(id, reversed);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJ Object::generic_getattr_with_dict(OBJ obj, OBJ name, OBJ dict)
	{
		TYPE type{ typeof(obj) };

		if (!type->tp_dict && !type->ready())
		{
			return nullptr;
		}

		OBJ descr{ type.lookup(name) };

		DescrGetFunc get{};

		if (descr)
		{
			TYPE descr_type{ typeof(descr) };

			get = descr_type->tp_descr_get;

			if (get && descr_type->tp_descr_set)
			{
				return get(descr, obj, type);
			}
		}

		if (!dict)
		{
			if (OBJ * dictptr{ get_dict_ptr(type, obj) })
			{
				dict = *dictptr;
			}
		}

		if (dict)
		{
			return ((DICT &)dict).lookup(name);
		}

		if (get)
		{
			return get(descr, obj, type);
		}

		if (descr)
		{
			return descr;
		}

		return nullptr;
	}

	OBJ Object::generic_getattr(OBJ obj, OBJ name) noexcept
	{
		return generic_getattr_with_dict(obj, name, nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ Object::generic_setattr_with_dict(OBJ obj, OBJ name, OBJ value, OBJ dict)
	{
		TYPE type{ typeof(obj) };

		if (!type->tp_dict && !type->ready())
		{
			return Error_Unknown;
		}

		OBJ descr{ type.lookup(name) };

		DescrSetFunc set{};

		if (descr)
		{
			set = typeof(descr)->tp_descr_set;

			if (set)
			{
				return set(descr, obj, value);
			}
		}

		if (!dict)
		{
			if (OBJ * dictptr{ get_dict_ptr(type, obj) })
			{
				dict = *dictptr;

				if (!dict)
				{
					dict = DICT::new_();
				}

				return (((DICT &)dict)[name] = value), Error_OK;
			}
			else
			{
				return Error_Unknown;
			}
		}
		else
		{
			return (((DICT &)dict)[name] = value), Error_OK;
		}
	}

	Error_ Object::generic_setattr(OBJ obj, OBJ name, OBJ value) noexcept
	{
		return generic_setattr_with_dict(obj, name, value, nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool predelete_handler(Object * value)
	{
		return value->_predelete();
	}

	void postinitialize_handler(Object * value)
	{
		value->_postinitialize();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}