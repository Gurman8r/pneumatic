#include <core/object/builtins/type_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
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

	TypeRef TypeObject::_get_typev() const noexcept
	{
		return get_type_static();
	}

	TypeRef TypeObject::get_type_static() noexcept
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
			(GetterFunc)[](ObjectRef obj, auto) -> ObjectRef
			{
				if (!obj) { return nullptr; }
				return nullptr;
			},
			(SetterFunc)[](ObjectRef obj, ObjectRef value, auto) -> Error_
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

		t.tp_hash = (HashFunc)[](ObjectRef self) -> size_t { return ((TypeRef)self)->tp_name.hash_code(); };

		t.tp_repr = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(TypeRef(self)->tp_name); };

		t.tp_str = (ReprFunc)[](ObjectRef self) -> StringRef { return StringRef(TypeRef(self)->tp_name); };

		t.tp_call = (BinaryFunc)[](ObjectRef self, ObjectRef args) -> ObjectRef
		{
			ASSERT(TypeRef::check_(self));
			NewFunc fn{ TypeRef(self)->tp_new };
			return fn ? fn(self, args) : nullptr;
		};

		t.tp_bind = BIND_CLASS(TypeObject, t)
		{
			return t
				.def_static("__instancecheck__", [](ObjectRef const & obj, ObjectRef const & type) { return isinstance(obj, type); })
				.def("__contains__", [](TypeObject const & self, ObjectRef const & value) { return DictRef(self.tp_dict).contains(value); })
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
				.def_property_readonly("__text_signature__", [](TypeObject const & self) { return StringRef(/* TODO */); })
				.def_property_readonly("__qualname__", [](TypeObject const & self) { return StringRef(/* TODO */); })
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
	
		tp_bases = ListRef::new_();

		if (tp_base) {
			((ListRef &)tp_bases).append(tp_base);
		}

		if (!tp_dict) {
			tp_dict = DictRef::new_();
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

		for (TypeRef const & base : (ListRef &)tp_mro)
		{
			inherit_slots(*base);
		}

		for (TypeRef const & base : (ListRef &)tp_bases)
		{
			if (!base->tp_subclasses)
			{
				base->tp_subclasses = DictRef::new_();
			}

			((DictRef &)base->tp_subclasses)[TypeRef(this)] = TypeRef(this);
		}

		tp_flags = (tp_flags & ~TypeFlags_Readying) | TypeFlags_Ready;

		return true;
	}

	ObjectRef TypeObject::lookup(ObjectRef const & name) const
	{
		if (!name || !ListRef::check_(tp_mro))
		{
			return nullptr;
		}
		else
		{
			for (TypeRef const & base : (ListRef &)tp_mro)
			{
				if (ObjectRef item{ ((DictRef &)(base->tp_dict)).lookup(name) })
				{
					return item;
				}
			}
			return nullptr;
		}
	}

	bool TypeObject::is_subtype(TypeRef const & value) const
	{
		if (ListRef::check_(tp_mro))
		{
			for (TypeRef const & base : (ListRef &)tp_mro)
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
		if (ListRef::check_(tp_bases)) {
			for (TypeRef const & base : (ListRef &)tp_bases) {
				if (DictRef::check_(base->tp_subclasses)) {
					((DictRef &)base->tp_subclasses).del(this);
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

		ASSERT(TypeRef::check_(ptr()));

		ASSERT(!(tp_flags & TypeFlags_Readying));

		ASSERT(DictRef::check_(tp_dict));

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
			if (DictRef dict{ tp_dict }; dict && !dict.contains(&ID___eq__) && !dict.contains(&ID___hash__))
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

	bool TypeObject::mro_internal(ObjectRef * in_old_mro)
	{
		ObjectRef old_mro{ tp_mro };

		ObjectRef new_mro{ std::invoke([&]() {
			// mro_implementation
			ASSERT(ListRef::check_(tp_bases));
			ListRef result{ ListRef::new_() };
			result.reserve(((ListRef &)tp_bases).size() + 1);
			result.append(this);
			for (TypeRef const & base : (ListRef &)tp_bases) { result.append(base); }
			return result;
		}) };
	
		bool const reent{ tp_mro != old_mro };
	
		if (!new_mro) { return false; }
	
		if (reent) { return true; }
	
		tp_mro = new_mro;
	
		if (in_old_mro) { *in_old_mro = old_mro; }
	
		return true;
	}

	Error_ TypeObject::update_slot(StringRef const & name)
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
			tp_new = (NewFunc)[](TypeRef type, ObjectRef args) -> ObjectRef {
				STR_IDENTIFIER(__new__);
				if (ObjectRef f{ type.lookup(&ID___new__) }) { return call_object(f, args); }
				return nullptr;
			};
		} break;

		case "__del__"_hash: {
			tp_del = (DelFunc)[](Object * obj) -> void {
				STR_IDENTIFIER(__del__);
				if (ObjectRef f{ typeof(obj).lookup(&ID___del__) }) { /* TODO */ }
			};
		} break;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		case "__call__"_hash: {
			tp_call = (BinaryFunc)[](ObjectRef self, ObjectRef args) -> ObjectRef {
				STR_IDENTIFIER(__call__);
				if (ObjectRef f{ typeof(self).lookup(&ID___call__) }) { return call_object(f, args); }
				return nullptr;
			};
		} break;

		case "__hash__"_hash: {
			tp_hash = (HashFunc)[](ObjectRef self) -> size_t {
				STR_IDENTIFIER(__hash__);
				if (ObjectRef f{ typeof(self).lookup(&ID___hash__) }) { return call_object(f, self).cast<size_t>(); }
				return 0;
			};
		} break;

		case "__len__"_hash: {
			tp_len = (LenFunc)[](ObjectRef self) -> ssize_t {
				STR_IDENTIFIER(__len__);
				if (ObjectRef f{ typeof(self).lookup(&ID___len__) }) { return call_object(f, self).cast<ssize_t>(); }
				return -1;
			};
		} break;

		case "__repr__"_hash: {
			tp_repr = (ReprFunc)[](ObjectRef self) -> StringRef {
				STR_IDENTIFIER(__repr__);
				if (ObjectRef f{ typeof(self).lookup(&ID___repr__) }) { return call_object(f, self); }
				return nullptr;
			};
		} break;

		case "__str__"_hash: {
			tp_str = (ReprFunc)[](ObjectRef self) -> StringRef {
				STR_IDENTIFIER(__str__);
				if (ObjectRef f{ typeof(self).lookup(&ID___str__) }) { return call_object(f, self); }
				return nullptr;
			};
		} break;

		case "__get__"_hash: {
			tp_descr_get = (DescrGetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef type) -> ObjectRef {
				STR_IDENTIFIER(__get__);
				if (ObjectRef f{ typeof(self).lookup(&ID___get__) }) { /* TODO */ }
				return nullptr;
			};
		} break;

		case "__set__"_hash: {
			tp_descr_set = (DescrSetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef type) -> Error_ {
				STR_IDENTIFIER(__set__);
				if (ObjectRef f{ typeof(self).lookup(&ID___set__) }) { /* TODO */ }
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

	ObjectRef TypeObject::type_getattro(TypeRef type, ObjectRef name)
	{
		return generic_getattr_with_dict(type, name, nullptr);
	}

	Error_ TypeObject::type_setattro(TypeRef type, ObjectRef name, ObjectRef value)
	{
		ASSERT(StringRef::check_(name));

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