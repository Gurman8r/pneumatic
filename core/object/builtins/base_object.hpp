#ifndef _PNU_BASE_OBJECT_HPP_
#define _PNU_BASE_OBJECT_HPP_

#include <core/object/accessors.hpp>
#include <core/object/ref.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// define class
#define DEFINE_CLASS(m_class, m_inherits)											\
public:																				\
	using base_type = typename m_inherits;											\
																					\
	using typename base_type::Notification_;										\
																					\
private:																			\
	friend class Pnu::Internals;													\
	friend class Pnu::_EmbedClassHelper<m_class>;									\
	friend struct Pnu::DefaultDelete<m_class>;										\
																					\
	static constexpr Pnu::StringView __name_static{ TOSTR(m_class) };				\
																					\
	static Pnu::TypeObject __type_static;											\
																					\
protected:																			\
	static void initialize_class()													\
	{																				\
		static ON_SCOPE_ENTER(&)													\
		{																			\
			get_internals()->add_class(&m_class::__type_static);					\
																					\
			if (m_class::__type_static.tp_bind) {									\
				ASSERT(m_class::__type_static.tp_bind(&m_class::__type_static));	\
			}																		\
		};																			\
	}																				\
																					\
	virtual void _initialize_classv() override										\
	{																				\
		m_class::initialize_class();												\
	}																				\
																					\
	FORCE_INLINE virtual Pnu::StringView _get_class_namev() const noexcept override	\
	{																				\
		return m_class::get_class_name_static();									\
	}																				\
																					\
	FORCE_INLINE virtual Pnu::TypeRef _get_typev() const noexcept override			\
	{																				\
		return m_class::get_type_static();											\
	}																				\
																					\
	FORCE_INLINE void (Object::*_get_notification() const)(Notification_)			\
	{																				\
		return (void (Object::*)(Notification_))&m_class::_notification;			\
	}																				\
																					\
	virtual void _notificationv(Notification_ id, bool reversed) override			\
	{																				\
		if (!reversed) {															\
			m_inherits::_notificationv(id, reversed);								\
		}																			\
		if (m_class::_get_notification() != m_inherits::_get_notification()) {		\
			_notification(id);														\
		}																			\
		if (reversed) {																\
			m_inherits::_notificationv(id, reversed);								\
		}																			\
	}																				\
																					\
public:																				\
	FORCE_INLINE static constexpr Pnu::StringView get_class_name_static() noexcept	\
	{																				\
		return m_class::__name_static;												\
	}																				\
																					\
	FORCE_INLINE static Pnu::TypeRef get_type_static() noexcept						\
	{																				\
		return &m_class::__type_static;												\
	}																				\
																					\
private:

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// class embedding utility
#define EMBED_CLASS(m_class, m_var, ...)											\
																					\
	/* implement embedding helper */												\
	template <> class Pnu::_EmbedClassHelper<m_class> final							\
	{																				\
	public:																			\
		static void do_embed(Pnu::TypeObject & t);									\
																					\
		static void embed(Pnu::TypeObject & t)										\
		{																			\
			/* TODO: can add extra stuff here */									\
																					\
			do_embed(t);															\
		}																			\
	};																				\
																					\
	/* construct the type object using a maker to call the embedder */				\
	Pnu::TypeObject m_class::__type_static = MAKER(									\
		Pnu::TypeObject,															\
		Pnu::mpl::type_tag<m_class>(),												\
		TOSTR(m_class),																\
		##__VA_ARGS__)																\
																					\
	+ Pnu::_EmbedClassHelper<m_class>::embed;										\
																					\
	/* implement binder function body */											\
	void Pnu::_EmbedClassHelper<m_class>::do_embed(Pnu::TypeObject & m_var)			\
																					\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// object
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// base object
	class PNU_API Object : public ObjectAPI<Object>
	{
	public:
		using base_type = typename void;

		using Notification_ = typename i32;
		enum : Notification_ {
			Notification_PostInitialize	= 1,
			Notification_PreDelete		= 2,
		};

	private:
		friend class ObjectRef;
		friend class Internals;
		friend class _EmbedClassHelper<Object>;
		friend struct DefaultDelete<Object>;
		friend PNU_API_FUNC(bool) predelete_handler(Object *);
		friend PNU_API_FUNC(void) postinitialize_handler(Object *);

		static constexpr StringView __name_static{ "Object" };

		static TypeObject __type_static;

		SafeRefCount m_refcount{}, m_refcount_init{}; // ref count

		mutable Ref<TypeObject> m_type{}; // type

	protected:
		static void initialize_class();
		virtual void _initialize_classv();

		bool _predelete();
		void _postinitialize();

		void _initialize_object();
		void _finalize_object();

		FORCE_INLINE virtual StringView _get_class_namev() const noexcept { return get_class_name_static(); }
		FORCE_INLINE virtual TypeRef _get_typev() const noexcept;

		virtual void _notificationv(Notification_ id, bool reversed) {}
		void _notification(Notification_ id) {}
		FORCE_INLINE void (Object::*_get_notification() const)(Notification_) { return &Object::_notification; }

		Object() { _initialize_object(); }

	public:
		virtual ~Object() { _finalize_object(); }

		bool init_ref();
		bool inc_ref();
		bool dec_ref();
		i32 get_ref_count() const { return m_refcount.get(); }
		bool has_references() const { return m_refcount_init.get() != 1; }

		void notification(Notification_ id, bool reversed = false);

		static constexpr StringView get_class_name_static() noexcept { return __name_static; }
		StringView get_class_name() const noexcept { return _get_class_namev(); }

		static TypeRef get_type_static() noexcept;
		TypeRef get_type() const noexcept;
		void set_type(TypeRef const & value) noexcept;

		template <class T> T cast() const &;
		template <class T> T cast() &&;

		Object * ptr() const noexcept { return (Object *)this; }

		// generic_getattr
		static ObjectRef generic_getattr_with_dict(ObjectRef obj, ObjectRef name, ObjectRef dict);
		static ObjectRef generic_getattr(ObjectRef obj, ObjectRef name) noexcept;

		// generic_setattr
		static Error_ generic_setattr_with_dict(ObjectRef obj, ObjectRef name, ObjectRef value, ObjectRef dict);
		static Error_ generic_setattr(ObjectRef obj, ObjectRef name, ObjectRef value) noexcept;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	PNU_API_FUNC(bool) predelete_handler(Object * value);

	PNU_API_FUNC(void) postinitialize_handler(Object * value);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// object delete
	template <> struct DefaultDelete<Object>
	{
		template <class U> void operator()(U * ptr) const
		{
			if (Object * obj{ (Object *)ptr }; !obj)
			{
				return;
			}
			else if (obj->m_type && obj->m_type->tp_del)
			{
				obj->m_type->tp_del(obj);
			}
			else
			{
				memdelete(ptr);
			}
		}
	};

	// no check
#define OBJECT_NO_CHECK(o) (true)

	// object ref
	class ObjectRef : public Ref<Object>
	{
		REF_CLASS(ObjectRef, OBJECT_NO_CHECK);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct Hasher<ObjectRef> : Hasher<Ref<Object>> {};
	template <> struct EqualTo<ObjectRef> : EqualTo<Ref<Object>> {};
	template <> struct NotEqualTo<ObjectRef> : NotEqualTo<Ref<Object>> {};
	template <> struct Less<ObjectRef> : Less<Ref<Object>> {};
	template <> struct Greater<ObjectRef> : Greater<Ref<Object>> {};
	template <> struct LessEqual<ObjectRef> : LessEqual<Ref<Object>> {};
	template <> struct GreaterEqual<ObjectRef> : GreaterEqual<Ref<Object>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> auto object_or_cast(T && o) noexcept -> decltype(FWD(o)) { return FWD(o); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> ObjectRef object_or_cast(T && o);

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> inline ObjectRef object_or_cast(T const * o) { return Ref<T>{ (T *)o }; }

	inline ObjectRef object_or_cast(cstring s) { return object_or_cast(String{ s }); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> TypeRef typeof() noexcept { return T::get_type_static(); }

	template <class T, std::enable_if_t<is_object_api_v<T>, int> = 0
	> TypeRef typeof(T && o) noexcept { return VALIDATE(FWD(o))->get_type(); }

	template <class T, std::enable_if_t<!is_object_api_v<T>, int> = 0
	> TypeRef typeof(T && o) noexcept { return typeof(FWD_OBJ(o)); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, std::enable_if_t<is_base_object_v<T>, int> = 0
	> TypeRef baseof() noexcept
	{
		if constexpr (std::is_void_v<T::base_type>) { return nullptr; }

		else { return typeof<T::base_type>(); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class O = ObjectRef, std::enable_if_t<is_object_api_v<T>, int> = 0
	> bool isinstance(O && obj) noexcept
	{
		return isinstance(FWD_OBJ(obj), typeof<T>());
	}

	template <class A = ObjectRef, class B = ObjectRef
	> bool isinstance(A const & obj, B const & type)
	{
		return typeof(obj).is_subtype(TypeRef::check_(type) ? (TypeRef)type : typeof(type));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getattr
	template <class Index = ObjectRef
	> ObjectRef getattr(ObjectRef const & obj, Index && index)
	{
		if (!obj) { return nullptr; }

		TypeRef type{ typeof(obj) };

		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_getattr) { return type->tp_getattr(obj, FWD(index)); }

			else if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else { return nullptr; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_getattro(obj, FWD_OBJ(index)); }

			else if (type->tp_getattr) { return type->tp_getattr(obj, StringRef(FWD(index)).c_str()); }

			else { return nullptr; }
		}
	}

	// getattr (default)
	template <class Index = ObjectRef, class Value = ObjectRef
	> ObjectRef getattr(ObjectRef const & obj, Index && index, Value && defval)
	{
		if (!obj)
		{
			return FWD_OBJ(defval);
		}
		else if (StringRef str_name{ FWD_OBJ(index) }; hasattr(obj, str_name))
		{
			return getattr(obj, str_name);
		}
		else
		{
			return FWD_OBJ(defval);
		}
	}

	// setattr
	template <class Index = ObjectRef, class Value = ObjectRef
	> Error_ setattr(ObjectRef const & obj, Index && index, Value && value)
	{
		if (!obj) { return Error_Unknown; }

		TypeRef type{ typeof(obj) };
		
		if constexpr (mpl::is_string_v<Index>)
		{
			if (type->tp_setattr) { return type->tp_setattr(obj, FWD(index), FWD_OBJ(value)); }

			else if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else { return Error_Unknown; }
		}
		else
		{
			if (type->tp_getattro) { return type->tp_setattro(obj, FWD_OBJ(index), FWD_OBJ(value)); }

			else if (type->tp_setattr) { return type->tp_setattr(obj, StringRef(FWD(index)).c_str(), FWD_OBJ(value)); }

			else { return Error_Unknown; }
		}
	}

	// hasattr
	template <class Index = ObjectRef
	> bool hasattr(ObjectRef const & obj, Index && index)
	{
		if (!obj) { return false; }

		TypeRef type{ typeof(obj) };

		if (StringRef str_name{ FWD_OBJ(index) }; !str_name)
		{
			return false;
		}
		else if (type->tp_getattro == Object::generic_getattr)
		{
			return Object::generic_getattr(obj, str_name).is_valid();
		}
		else if (type->tp_getattro)
		{
			return (type->tp_getattro)(obj, str_name).is_valid();
		}
		else if (type->tp_getattr)
		{
			return (type->tp_getattr)(obj, str_name.c_str()).is_valid();
		}
		else
		{
			return false;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// getitem
	template <class Index = ObjectRef
	> ObjectRef getitem(ObjectRef const & obj, Index && index)
	{
		STR_IDENTIFIER(__getitem__);

		if (!obj) { return nullptr; }

		else if (DictRef::check_(obj)) { return ((DictRef &)obj)[FWD(index)]; }

		else if (ListRef::check_(obj)) { return ((ListRef &)obj)[FWD(index)]; }

		else if (ObjectRef get{ typeof(obj).lookup(&ID___getitem__) }) { return get(obj, FWD_OBJ(index)); }

		else { return nullptr; }
	}

	// setitem
	template <class Index = ObjectRef, class Value = ObjectRef
	> Error_ setitem(ObjectRef const & obj, Index && index, Value && value)
	{
		STR_IDENTIFIER(__setitem__);

		if (!obj) { return Error_Unknown; }

		else if (DictRef::check_(obj)) { return (((DictRef &)obj)[FWD(index)] = FWD_OBJ(value)), Error_OK; }

		else if (ListRef::check_(obj)) { return (((ListRef &)obj)[FWD(index)] = FWD_OBJ(value)), Error_OK; }

		else if (ObjectRef set{ typeof(obj).lookup(&ID___setitem__) }) { return set(obj, FWD_OBJ(index), FWD_OBJ(value)), Error_OK; }

		else { return Error_Unknown; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_BASE_OBJECT_HPP_
