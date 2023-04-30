#include <core/object/builtins/type_object.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TypeObject::initialize_class()
	{
		static ON_SCOPE_ENTER(&) {
			get_internals()->add_class(&__type_static);
			ASSERT(__type_static.tp_bind);
			ASSERT(__type_static.tp_bind(&__type_static));
		};
	}

	void TypeObject::_initialize_classv()
	{
		TypeObject::initialize_class();
	}

	TYPE TypeObject::_get_typev() const noexcept
	{
		return get_type_static();
	}

	TYPE TypeObject::get_type_static() noexcept
	{
		return &__type_static;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static MemberDef type_members[]
	{
		{"__dictoffset__", DataType_SSizeT, offsetof(TypeObject, tp_dictoffset) },
		{/*SENTINAL*/}
	};

	static MethodDef type_methods[]
	{
		{ "", BinaryFunc{} },
		{/*SENTINAL*/}
	};

	static GetSetDef type_getsets[]
	{
		{
			"some_property",
			(GetterFunc)[](OBJ obj, auto) -> OBJ
			{
				if (!obj) { return nullptr; }
				return nullptr;
			},
			(SetterFunc)[](OBJ obj, OBJ value, auto) -> Error_
			{
				if (!obj) { return Error_Unknown; }
				return Error_OK;
			}
		},
		{/*SENTINAL*/}
	};

	EMBED_CLASS(TypeObject, t, TypeFlags_HaveVectorCall)
	{
		t.tp_dictoffset = offsetof(TypeObject, tp_dict);

		t.tp_vectorcalloffset = offsetof(TypeObject, tp_vectorcall);

		t.tp_members = type_members;
	
		t.tp_methods = type_methods;

		t.tp_getsets = type_getsets;

		t.tp_getattro = (GetAttrOFunc)&TypeObject::type_getattro;

		t.tp_setattro = (SetAttrOFunc)&TypeObject::type_setattro;

		t.tp_hash = (HashFunc)[](OBJ self) -> size_t { return ((TYPE)self)->tp_name.hash_code(); };

		t.tp_repr = (ReprFunc)[](OBJ self) -> STR { return STR(TYPE(self)->tp_name); };

		t.tp_str = (ReprFunc)[](OBJ self) -> STR { return STR(TYPE(self)->tp_name); };

		t.tp_call = (BinaryFunc)[](OBJ self, OBJ args) -> OBJ
		{
			ASSERT(TYPE::check_(self));
			NewFunc fn{ TYPE(self)->tp_new };
			return fn ? fn(self, args) : nullptr;
		};

		t.tp_bind = BIND_CLASS(TypeObject, t)
		{
			return t
				.def_static("__instancecheck__", [](OBJ const & obj, OBJ const & type) { return isinstance(obj, type); })
				.def("__contains__", [](TypeObject const & self, OBJ const & value) { return DICT(self.tp_dict).contains(value); })
				.def("__subclasscheck__", &TypeObject::is_subtype)
				.def_readonly("__base__", &TypeObject::tp_base)
				.def_readonly("__bases__", &TypeObject::tp_bases)
				.def_readonly("__dict__", &TypeObject::tp_dict)
				.def_readonly("__dictoffset__", &TypeObject::tp_dictoffset)
				.def_readonly("__flags__", &TypeObject::tp_flags)
				.def_readonly("__mro__", &TypeObject::tp_mro)
				.def_readwrite("__name__", &TypeObject::tp_name)
				.def_readonly("__size__", &TypeObject::tp_size)
				.def_readonly("__vectorcalloffset__", &TypeObject::tp_vectorcalloffset)
				.def_property_readonly("__text_signature__", [](TypeObject const & self) { return STR(/* TODO */); })
				.def_property_readonly("__qualname__", [](TypeObject const & self) { return STR(/* TODO */); })
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool TypeObject::ready()
	{
		if (tp_flags & TypeFlags_Ready) { ASSERT(check_consistency()); return true; }
	
		ASSERT(!(tp_flags & TypeFlags_Readying));

		tp_flags |= TypeFlags_Readying;

		if (!tp_base && this != typeof<Object>()) {
			tp_base = typeof<Object>();
		}
	
		if (tp_base && !tp_base->tp_dict) {
			ASSERT(tp_base->ready());
		}
	
		if (!get_type() && tp_base) {
			set_type(tp_base->get_type());
		}
	
		tp_bases = LIST::new_();

		if (tp_base) {
			((LIST &)tp_bases).append(tp_base);
		}

		if (!tp_dict) {
			tp_dict = DICT::new_();
		}

		ASSERT(add_operators() == Error_OK);

		if (tp_methods) {
			ASSERT(add_methods(tp_methods) == Error_OK);
		}

		if (tp_members) {
			ASSERT(add_members(tp_members) == Error_OK);
		}

		if (tp_getsets) {
			ASSERT(add_getsets(tp_getsets) == Error_OK);
		}

		ASSERT(mro_internal(nullptr));
	
		if (tp_base)
		{
			if (!tp_base.is(typeof<Object>()) || !tp_new)
			{
				tp_new = tp_base->tp_new;
			}

			copy_val(*tp_base, &TypeObject::tp_size);

			copy_val(*tp_base, &TypeObject::tp_dictoffset);
		}

		for (TYPE const & base : (LIST &)tp_mro)
		{
			inherit_slots(*base);
		}

		for (TYPE const & base : (LIST &)tp_bases)
		{
			if (!base->tp_subclasses)
			{
				base->tp_subclasses = DICT::new_();
			}

			((DICT &)base->tp_subclasses)[TYPE(this)] = TYPE(this);
		}

		tp_flags = (tp_flags & ~TypeFlags_Readying) | TypeFlags_Ready;

		return true;
	}

	OBJ TypeObject::lookup(OBJ const & name) const
	{
		if (!name || !LIST::check_(tp_mro))
		{
			return nullptr;
		}
		else
		{
			for (TYPE const & base : (LIST &)tp_mro)
			{
				if (OBJ item{ ((DICT &)(base->tp_dict)).lookup(name) })
				{
					return item;
				}
			}
			return nullptr;
		}
	}

	bool TypeObject::is_subtype(TYPE const & value) const
	{
		if (LIST::check_(tp_mro))
		{
			for (TYPE const & base : (LIST &)tp_mro)
			{
				if (value.is(base)) { return true; }
			}
			return false;
		}
		else
		{
			TypeObject const * base{ this };

			do {

				if (base == value) { return true; }

				base = base->tp_base.ptr();

			} while (base);

			return value.is(typeof<Object>());
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void TypeObject::cleanup()
	{
		if (LIST::check_(tp_bases)) {
			for (TYPE const & base : (LIST &)tp_bases) {
				if (DICT::check_(base->tp_subclasses)) {
					((DICT &)base->tp_subclasses).del(this);
				}
			}
		}
		tp_base = nullptr;
		tp_bases = nullptr;
		tp_dict = nullptr;
		tp_mro = nullptr;
		tp_subclasses = nullptr;
	}

	bool TypeObject::check_consistency() const
	{
		// don't check static types before ready()
		if (!(tp_flags & TypeFlags_Ready)) { return true; }

		ASSERT(TYPE::check_(ptr()));

		ASSERT(!(tp_flags & TypeFlags_Readying));

		ASSERT(DICT::check_(tp_dict));

		return true;
	}

	Error_ TypeObject::add_operators()
	{
		return Error_OK;
	}

	Error_ TypeObject::add_members(MemberDef * members)
	{
		return Error_OK;
	}

	Error_ TypeObject::add_methods(MethodDef * methods)
	{
		return Error_OK;
	}

	Error_ TypeObject::add_getsets(GetSetDef * getsets)
	{
		return Error_OK;
	}

	void TypeObject::inherit_slots(TypeObject * base)
	{
		TypeObject * basebase{ *(base->tp_base) };

		copy_slot(base, basebase, &TypeObject::tp_del);

		if (!tp_getattr && !tp_getattro) { tp_getattr = base->tp_getattr; tp_getattro = base->tp_getattro; }

		if (!tp_setattr && !tp_setattro) { tp_setattr = base->tp_setattr; tp_setattro = base->tp_setattro; }

		copy_slot(base, basebase, &TypeObject::tp_len);
	
		copy_slot(base, basebase, &TypeObject::tp_repr);
	
		copy_slot(base, basebase, &TypeObject::tp_str);

		copy_slot(base, basebase, &TypeObject::tp_vectorcalloffset);
	
		if (!tp_call && (base->tp_flags & TypeFlags_HaveVectorCall))
		{
			tp_flags |= TypeFlags_HaveVectorCall;
		}

		copy_slot(base, basebase, &TypeObject::tp_call);

		if (!tp_cmp && !tp_hash)
		{
			STR_IDENTIFIER(__eq__);
			STR_IDENTIFIER(__hash__);
			if (DICT dict{ tp_dict }; dict && !dict.contains(&ID___eq__) && !dict.contains(&ID___hash__))
			{
				tp_cmp = base->tp_cmp;
				tp_hash = base->tp_hash;
			}
		}

		copy_slot(base, basebase, &TypeObject::tp_descr_get);

		if (base->tp_descr_get && (tp_descr_get == base->tp_descr_get) && (base->tp_flags & TypeFlags_MethodDescriptor))
		{
			tp_flags |= TypeFlags_MethodDescriptor;
		}

		copy_slot(base, basebase, &TypeObject::tp_descr_set);

		copy_slot(base, basebase, &TypeObject::tp_dictoffset);
	}

	void TypeObject::modified()
	{
	}

	bool TypeObject::mro_internal(OBJ * in_old_mro)
	{
		OBJ old_mro{ tp_mro };

		OBJ new_mro{ std::invoke([&]() {
			// mro_implementation
			ASSERT(LIST::check_(tp_bases));
			LIST result{ LIST::new_() };
			result.reserve(((LIST &)tp_bases).size() + 1);
			result.append(this);
			for (TYPE const & base : (LIST &)tp_bases) { result.append(base); }
			return result;
		}) };
	
		bool const reent{ tp_mro != old_mro };
	
		if (!new_mro) { return false; }
	
		if (reent) { return true; }
	
		tp_mro = new_mro;
	
		if (in_old_mro) { *in_old_mro = old_mro; }
	
		return true;
	}

	Error_ TypeObject::update_slot(STR const & name)
	{
		if (!name || name.empty()) {
			return Error_Unknown;
		}
		switch (name.hash_code())
		{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		// do nothing
		case "__getattr__"_hash:
		case "__setattr__"_hash:
		case "__delattr__"_hash:
		default: return Error_OK;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		case "__new__"_hash: {
			tp_new = (NewFunc)[](TYPE type, OBJ args) -> OBJ {
				STR_IDENTIFIER(__new__);
				if (OBJ f{ type.lookup(&ID___new__) }) { return call_object(f, args); }
				return nullptr;
			};
		} break;

		case "__del__"_hash: {
			tp_del = (DelFunc)[](Object * obj) -> void {
				STR_IDENTIFIER(__del__);
				if (OBJ f{ typeof(obj).lookup(&ID___del__) }) { /* TODO */ }
			};
		} break;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		case "__call__"_hash: {
			tp_call = (BinaryFunc)[](OBJ self, OBJ args) -> OBJ {
				STR_IDENTIFIER(__call__);
				if (OBJ f{ typeof(self).lookup(&ID___call__) }) { return call_object(f, args); }
				return nullptr;
			};
		} break;

		case "__hash__"_hash: {
			tp_hash = (HashFunc)[](OBJ self) -> size_t {
				STR_IDENTIFIER(__hash__);
				if (OBJ f{ typeof(self).lookup(&ID___hash__) }) { return call_object(f, self).cast<size_t>(); }
				return 0;
			};
		} break;

		case "__len__"_hash: {
			tp_len = (LenFunc)[](OBJ self) -> ssize_t {
				STR_IDENTIFIER(__len__);
				if (OBJ f{ typeof(self).lookup(&ID___len__) }) { return call_object(f, self).cast<ssize_t>(); }
				return -1;
			};
		} break;

		case "__repr__"_hash: {
			tp_repr = (ReprFunc)[](OBJ self) -> STR {
				STR_IDENTIFIER(__repr__);
				if (OBJ f{ typeof(self).lookup(&ID___repr__) }) { return call_object(f, self); }
				return nullptr;
			};
		} break;

		case "__str__"_hash: {
			tp_str = (ReprFunc)[](OBJ self) -> STR {
				STR_IDENTIFIER(__str__);
				if (OBJ f{ typeof(self).lookup(&ID___str__) }) { return call_object(f, self); }
				return nullptr;
			};
		} break;

		case "__get__"_hash: {
			tp_descr_get = (DescrGetFunc)[](OBJ self, OBJ obj, OBJ type) -> OBJ {
				STR_IDENTIFIER(__get__);
				if (OBJ f{ typeof(self).lookup(&ID___get__) }) { /* TODO */ }
				return nullptr;
			};
		} break;

		case "__set__"_hash: {
			tp_descr_set = (DescrSetFunc)[](OBJ self, OBJ obj, OBJ type) -> Error_ {
				STR_IDENTIFIER(__set__);
				if (OBJ f{ typeof(self).lookup(&ID___set__) }) { /* TODO */ }
				return Error_Unknown;
			};
		} break;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		//case "__eq__"_hash: {} break;
		//case "__ne__"_hash: {} break;
		//case "__lt__"_hash: {} break;
		//case "__le__"_hash: {} break;
		//case "__gt__"_hash: {} break;
		//case "__ge__"_hash: {} break;

		//case "__add__"_hash: {} break;
		//case "__sub__"_hash: {} break;
		//case "__div__"_hash: {} break;
		//case "__mul__"_hash: {} break;
		//case "__mod__"_hash: {} break;
		//case "__pow__"_hash: {} break;
		//case "__pos__"_hash: {} break;
		//case "__neg__"_hash: {} break;

		//case "__invert__"_hash: {} break;
		//case "__lshift__"_hash: {} break;
		//case "__rshift__"_hash: {} break;
		//case "__and__"_hash: {} break;
		//case "__or__"_hash: {} break;
		//case "__xor__"_hash: {} break;

		//case "__int__"_hash: {} break;
		//case "__float__"_hash: {} break;
		//case "__bool__"_hash: {} break;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		}
		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJ TypeObject::type_getattro(TYPE type, OBJ name)
	{
		return generic_getattr_with_dict(type, name, nullptr);
	}

	Error_ TypeObject::type_setattro(TYPE type, OBJ name, OBJ value)
	{
		ASSERT(STR::check_(name));

		Error_ err{ generic_setattr_with_dict(type, name, value, nullptr) };

		if (err == Error_OK)
		{
			type->modified();

			if (is_dunder_name(name))
			{
				err = type->update_slot(name);
			}

			ASSERT(type->check_consistency());
		}

		return err;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}