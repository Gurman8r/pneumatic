#ifndef _PNU_TYPE_OBJECT_HPP_
#define _PNU_TYPE_OBJECT_HPP_

#include <core/object/internals.hpp>

// type
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type object
	class PNU_API TypeObject : public Object
	{
	public:
		using base_type = typename Object;

		using typename base_type::Notification_;

	private:
		friend class TypeRef;
		friend class Internals;
		friend class _EmbedClassHelper<TypeObject>;
		friend struct DefaultDelete<TypeObject>;

		static constexpr StringView __name_static{ "TypeObject" };

		static TypeObject __type_static;

	protected:
		static void initialize_class();

		virtual void _initialize_classv() override;

		virtual StringView _get_class_namev() const noexcept override { return get_class_name_static(); }

		virtual TypeRef _get_typev() const noexcept override;

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
		static constexpr StringView get_class_name_static() noexcept { return __name_static; }

		static TypeRef get_type_static() noexcept;

		TypeObject() noexcept {}

		template <class T
		> TypeObject(mpl::type_tag<T>, cstring name, i32 flags = TypeFlags_None) noexcept : TypeObject{}
		{
			tp_name = name;
			tp_size = sizeof(T);
			tp_flags = flags;
			tp_base = baseof<T>();
			tp_del = (DelFunc)memdelete<T>;
			tp_bind = (BindClassFunc)[](TypeRef t) -> TypeRef { return t; };
			tp_hash = (HashFunc)[](ObjectRef o) -> size_t { return Hasher<intptr_t>{}((intptr_t)*o); };
			tp_cmp = (CmpFunc)[](ObjectRef a, ObjectRef b) -> i32 { return compare((intptr_t)*a, (intptr_t)*b); };

			if constexpr (std::is_default_constructible_v<T>) {
				tp_new = (NewFunc)[](TypeRef, ObjectRef) -> ObjectRef { return memnew(T); };
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
		
		Ref<TypeObject>		tp_base				{ /* TypeRef */ };
		ObjectRef					tp_bases			{ /* ListRef */ };
		ObjectRef					tp_dict				{ /* DictRef */ };
		ObjectRef					tp_mro				{ /* ListRef */ };
		ObjectRef					tp_subclasses		{ /* DictRef */ };

	public:
		NODISCARD bool ready();
		NODISCARD ObjectRef lookup(ObjectRef const & name) const;
		NODISCARD bool is_subtype(TypeRef const & value) const;

	protected:
		void cleanup();
		bool check_consistency() const;
		Error_ add_operators();
		Error_ add_methods(MethodDef * methods);
		Error_ add_members(MemberDef * members);
		Error_ add_getsets(GetSetDef * getsets);
		void inherit_slots(TypeObject * base);
		void modified();
		bool mro_internal(ObjectRef * old_mro);
		Error_ update_slot(StringRef const & name);

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
		static ObjectRef type_getattro(TypeRef type, ObjectRef name);

		static Error_ type_setattro(TypeRef type, ObjectRef name, ObjectRef value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type delete
	template <> struct DefaultDelete<TypeObject> : DefaultDelete<Object> {};

	// type check
#define OBJECT_CHECK_TYPE(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Type_Subclass))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// type ref
	class TypeRef : public Ref<TypeObject>
	{
		REF_CLASS(TypeRef, OBJECT_CHECK_TYPE);

	public:
		NODISCARD bool ready() const { return m_ptr->ready(); }

		NODISCARD bool has_feature(TypeFlags_ flag) const { return flag_read(m_ptr->tp_flags, flag); }

		NODISCARD bool is_subtype(TypeRef const & value) const { return m_ptr->is_subtype(value); }

		NODISCARD ObjectRef lookup(ObjectRef const & name) const { return m_ptr->lookup(name); }

		template <class Name = cstring, class Value = ObjectRef
		> void add_object(Name && name, Value && value) noexcept
		{
			if (!m_ptr || !m_ptr->tp_dict || !m_ptr->ready()) { return; }

			StringRef str_name{ FWD(name) };

			((DictRef &)m_ptr->tp_dict)[str_name] = FWD(value); // modify tp_dict directly

			m_ptr->modified();

			if (is_dunder_name(str_name)) { m_ptr->update_slot(str_name); }

			ASSERT(m_ptr->check_consistency());
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> size_t hash(T const & o) noexcept
	{
		if (!o) { return 0; }
		TypeRef t{ typeof(o) };
		return t->tp_hash ? t->tp_hash(o) : Hasher<intptr_t>{}((intptr_t)o);
	}

	template <class T, class = std::enable_if_t<is_ref_v<T>>
	> ssize_t length(T const & o) noexcept
	{
		if (!o) { return -1; }
		TypeRef t{ typeof(o) };
		return t->tp_len ? t->tp_len(o) : -1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline ObjectRef * get_dict_ptr(TypeRef const & t, ObjectRef const & o)
	{
		if (t && o && 0 < t->tp_dictoffset)
		{
			return reinterpret_cast<ObjectRef *>(reinterpret_cast<char *>(*o) + t->tp_dictoffset);
		}
		else
		{
			return (ObjectRef *)nullptr;
		}
	}

	inline ObjectRef * get_dict_ptr(ObjectRef const & o) noexcept
	{
		return o ? get_dict_ptr(typeof(o), o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline VectorCallFunc get_vectorcall_func(TypeRef const & t, ObjectRef const & o)
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

	inline VectorCallFunc get_vectorcall_func(ObjectRef const & o) noexcept
	{
		return o ? get_vectorcall_func(typeof(o), o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_TYPE_OBJECT_HPP_
