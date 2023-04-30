#ifndef _PN_TYPE_OBJECT_HPP_
#define _PN_TYPE_OBJECT_HPP_

#include <core/object/internals.hpp>

// type
namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type object
	class PN_API TypeObject : public Object
	{
	public:
		using base_type = typename Object;

		using typename base_type::Notification_;

	private:
		friend class TYPE;
		friend class Internals;
		friend class _EmbedClassHelper<TypeObject>;
		friend struct DefaultDelete<TypeObject>;

		static constexpr StringView __name_static{ "TypeObject" };

		static TypeObject __type_static;

	protected:
		static void initialize_class();

		virtual void _initialize_classv() override;

		virtual StringView _get_classv() const noexcept override { return get_class_static(); }

		virtual TYPE _get_typev() const noexcept override;

		FORCE_INLINE void (Object::*_get_notification() const)(i32) { return (void (Object::*)(i32))&TypeObject::_notification; }

		virtual void _notificationv(Notification_ id, bool reversed) override
		{
			if (!reversed) {
				Object::_notificationv(id, reversed);
			}
			if (TypeObject::_get_notification() != Object::_get_notification()) {
				_notification(id);
			}
			if (reversed) {
				Object::_notificationv(id, reversed);
			}
		}

	public:
		static constexpr StringView get_class_static() noexcept { return __name_static; }

		static TYPE get_type_static() noexcept;

		TypeObject() noexcept {}

		template <class T
		> TypeObject(mpl::type_tag<T>, cstring name, i32 flags = TypeFlags_None) noexcept : TypeObject{}
		{
			tp_name = name;
			tp_size = sizeof(T);
			tp_flags = flags;
			tp_base = baseof<T>();
			tp_del = (DelFunc)memdelete<T>;
			tp_bind = (BindClassFunc)[](TYPE t) -> TYPE { return t; };
			tp_hash = (HashFunc)[](OBJ o) -> size_t { return Hasher<intptr_t>{}((intptr_t)*o); };
			tp_cmp = (CmpFunc)[](OBJ a, OBJ b) -> i32 { return compare((intptr_t)*a, (intptr_t)*b); };

			if constexpr (std::is_default_constructible_v<T>) {
				tp_new = (NewFunc)[](TYPE, OBJ) -> OBJ { return memnew(T); };
			}

			if constexpr (std::is_base_of_v<TypeObject, T>) { tp_flags |= TypeFlags_Type_Subclass; }
			else if constexpr (std::is_base_of_v<IntObject, T>) { tp_flags |= TypeFlags_Int_Subclass; }
			else if constexpr (std::is_base_of_v<FloatObject, T>) { tp_flags |= TypeFlags_Float_Subclass; }
			else if constexpr (std::is_base_of_v<StringObject, T>) { tp_flags |= TypeFlags_Str_Subclass; }
			else if constexpr (std::is_base_of_v<TupleObject, T>) { tp_flags |= TypeFlags_Tuple_Subclass; }
			else if constexpr (std::is_base_of_v<ListObject, T>) { tp_flags |= TypeFlags_List_Subclass; }
			else if constexpr (std::is_base_of_v<DictObject, T>) { tp_flags |= TypeFlags_Dict_Subclass; }
		}

	public:
		String				tp_name				{};
		ssize_t				tp_size				{};
		i32					tp_flags			{};
		BindClassFunc		tp_bind				{};
		ssize_t				tp_dictoffset		{};
		ssize_t				tp_vectorcalloffset	{};

		MemberDef *			tp_members			{};
		MethodDef *			tp_methods			{};
		GetSetDef *			tp_getsets			{};

		GetAttrFunc			tp_getattr			{};
		SetAttrFunc			tp_setattr			{};
		GetAttrOFunc		tp_getattro			{};
		SetAttrOFunc		tp_setattro			{};
		DescrGetFunc		tp_descr_get		{};
		DescrSetFunc		tp_descr_set		{};

		BinaryFunc			tp_call				{};
		CmpFunc				tp_cmp				{};
		DelFunc				tp_del				{};
		HashFunc			tp_hash				{};
		LenFunc				tp_len				{};
		NewFunc				tp_new				{};
		ReprFunc			tp_repr				{};
		ReprFunc			tp_str				{};
		VectorCallFunc		tp_vectorcall		{};
		
		Ref<TypeObject>		tp_base				{ /* TYPE */ };
		OBJ					tp_bases			{ /* LIST */ };
		OBJ					tp_dict				{ /* DICT */ };
		OBJ					tp_mro				{ /* LIST */ };
		OBJ					tp_subclasses		{ /* DICT */ };

	public:
		NODISCARD bool ready();
		NODISCARD OBJ lookup(OBJ const & name) const;
		NODISCARD bool is_subtype(TYPE const & value) const;

	protected:
		void cleanup();
		bool check_consistency() const;
		Error_ add_operators();
		Error_ add_methods(MethodDef * methods);
		Error_ add_members(MemberDef * members);
		Error_ add_getsets(GetSetDef * getsets);
		void inherit_slots(TypeObject * base);
		void modified();
		bool mro_internal(OBJ * old_mro);
		Error_ update_slot(STR const & name);

	protected:
		template <class Slot> bool slot_defined(TypeObject * base, Slot TypeObject:: * slot) const
		{
			return (this->*slot) && (!base || (this->*slot) != (base->*slot));
		}

		template <class Slot> void copy_val(TypeObject * base, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base) { (this->*slot) = (base->*slot); }
		}

		template <class Slot> void copy_slot(TypeObject * base, TypeObject * basebase, Slot TypeObject:: * slot)
		{
			if (!(this->*slot) && base && base->slot_defined(basebase, slot)) { (this->*slot) = (base->*slot); }
		}

	public:
		static OBJ type_getattro(TYPE type, OBJ name);

		static Error_ type_setattro(TYPE type, OBJ name, OBJ value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type delete
	template <> struct DefaultDelete<TypeObject> : DefaultDelete<Object> {};

	// type check
#define OBJECT_CHECK_TYPE(o) (pn::typeof(o).has_feature(pn::TypeFlags_Type_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type ref
	class TYPE : public Ref<TypeObject>
	{
		REF_CLASS(TYPE, OBJECT_CHECK_TYPE);

	public:
		NODISCARD bool ready() const { return m_ptr->ready(); }

		NODISCARD bool has_feature(TypeFlags_ flag) const { return flag_read(m_ptr->tp_flags, flag); }

		NODISCARD bool is_subtype(TYPE const & value) const { return m_ptr->is_subtype(value); }

		NODISCARD OBJ lookup(OBJ const & name) const { return m_ptr->lookup(name); }

		template <class Name = cstring, class Value = OBJ
		> void add_object(Name && name, Value && value) noexcept
		{
			if (!m_ptr || !m_ptr->tp_dict || !m_ptr->ready()) { return; }

			STR str_name{ FWD(name) };

			((DICT &)m_ptr->tp_dict)[str_name] = FWD(value); // modify tp_dict directly

			m_ptr->modified();

			if (is_dunder_name(str_name)) { m_ptr->update_slot(str_name); }

			ASSERT(m_ptr->check_consistency());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> size_t hash(T const & o) noexcept
	{
		if (!o) { return 0; }
		TYPE t{ typeof(o) };
		return t->tp_hash ? t->tp_hash(o) : Hasher<intptr_t>{}((intptr_t)o);
	}

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> ssize_t length(T const & o) noexcept
	{
		if (!o) { return -1; }
		TYPE t{ typeof(o) };
		return t->tp_len ? t->tp_len(o) : -1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline OBJ * get_dict_ptr(TYPE const & t, OBJ const & o)
	{
		if (t && o && 0 < t->tp_dictoffset)
		{
			return reinterpret_cast<OBJ *>(reinterpret_cast<char *>(*o) + t->tp_dictoffset);
		}
		else
		{
			return (OBJ *)nullptr;
		}
	}

	inline OBJ * get_dict_ptr(OBJ const & o) noexcept
	{
		return o ? get_dict_ptr(typeof(o), o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline VectorCallFunc get_vectorcall_func(TYPE const & t, OBJ const & o)
	{
		if (t && o && 0 < t->tp_vectorcalloffset)
		{
			return *reinterpret_cast<VectorCallFunc *>(reinterpret_cast<char *>(*o) + t->tp_vectorcalloffset);
		}
		else
		{
			return (VectorCallFunc)nullptr;
		}
	}

	inline VectorCallFunc get_vectorcall_func(OBJ const & o) noexcept
	{
		return o ? get_vectorcall_func(typeof(o), o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_TYPE_OBJECT_HPP_
