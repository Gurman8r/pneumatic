#ifndef _PNU_DICT_OBJECT_HPP_
#define _PNU_DICT_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// dict
namespace Pnu
{
	// dict object
	class PNU_API DictObject : public Object
	{
		DEFINE_CLASS(DictObject, Object);

		friend class DictRef;

	public:
		HashMap<ObjectRef, ObjectRef> m_dict{};

		using storage_type		= decltype(m_dict);
		using hasher			= storage_type::hasher;
		using key_equal			= storage_type::key_equal;
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		auto & operator*() const { return (storage_type &)(m_dict); }

		auto * operator->() const { return (storage_type *)(&m_dict); }

		operator storage_type & () const { return (storage_type &)(m_dict); }

		DictObject() noexcept : m_dict{} {}

		DictObject(storage_type const & value) : m_dict{ value } {}

		DictObject(storage_type && value) noexcept : m_dict{ std::move(value) } {}

		DictObject(std::initializer_list<Pair<ObjectRef, ObjectRef>> init) : m_dict{}
		{
			for (auto const & e : init) { m_dict.insert(e); }
		}

		void clear() { m_dict.clear(); }

		void reserve(size_t count) { m_dict.reserve(count); }

		template <class Index = ObjectRef
		> auto del(Index && i) -> Error_ { return m_dict.erase(FWD_OBJ(i)), Error_OK; }

		template <class Index = ObjectRef, class Value = ObjectRef
		> bool insert(Index && i, Value && v) { return m_dict.try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = ObjectRef
		> bool contains(Index && i) const { return find(FWD(i)) != end(); }

		template <class Index = ObjectRef
		> auto find(Index && i) -> iterator { return m_dict.find(FWD_OBJ(i)); }

		template <class Index = ObjectRef
		> auto find(Index && i) const -> const_iterator { return m_dict.find(FWD_OBJ(i)); }

		template <class Index = ObjectRef
		> auto lookup(Index && i) const -> ObjectRef { return this->lookup(FWD_OBJ(i), ObjectRef{}); }

		template <class Index = ObjectRef, class Defval = ObjectRef
		> auto lookup(Index && i, Defval && dv) const -> ObjectRef
		{
			if (auto const ptr{ util::getptr(m_dict, FWD_OBJ(i)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = ObjectRef
		> auto operator[](Index && i) -> ObjectRef & { return m_dict[FWD_OBJ(i)]; }

		bool empty() const { return m_dict.empty(); }

		auto size() const { return m_dict.size(); }

		auto begin() -> iterator { return m_dict.begin(); }

		auto begin() const -> const_iterator { return m_dict.begin(); }

		auto end() -> iterator { return m_dict.end(); }

		auto end() const -> const_iterator { return m_dict.end(); }
	};

	// dict delete
	template <> struct DefaultDelete<DictObject> : DefaultDelete<Object> {};

	// dict check
#define OBJECT_CHECK_DICT(o) (Pnu::typeof(o).has_feature(Pnu::TypeFlags_Dict_Subclass))

	// dict ref
	class DictRef : public Ref<DictObject>
	{
		REF_CLASS(DictRef, OBJECT_CHECK_DICT);

	public:
		using storage_type		= value_type::storage_type;
		using hasher			= value_type::hasher;
		using key_equal			= value_type::key_equal;
		using iterator			= value_type::iterator;
		using const_iterator	= value_type::const_iterator;

		operator storage_type & () const { return VALIDATE(m_ptr)->operator storage_type &(); }

		void clear() const { VALIDATE(m_ptr)->clear(); }

		void reserve(size_t count) const { VALIDATE(m_ptr)->reserve(count); }

		template <class Index = ObjectRef
		> auto del(Index && i) -> Error_ { return VALIDATE(m_ptr)->del(FWD(i)); }

		template <class Index = ObjectRef, class Value = ObjectRef
		> bool insert(Index && i, Value && v) { return VALIDATE(m_ptr)->insert(FWD(i), FWD(v)); }

		template <class Index = ObjectRef
		> bool contains(Index && i) const { return VALIDATE(m_ptr)->contains(FWD(i)); }

		template <class Index = ObjectRef
		> auto find(Index && i) -> iterator { return VALIDATE(m_ptr)->find(FWD(i)); }

		template <class Index = ObjectRef
		> auto find(Index && i) const -> const_iterator { return VALIDATE(m_ptr)->find(FWD(i)); }

		template <class Index = ObjectRef
		> auto lookup(Index && i) const -> ObjectRef { return VALIDATE(m_ptr)->lookup(FWD(i)); }

		template <class Index = ObjectRef, class Defval = ObjectRef
		> auto lookup(Index && i, Defval && dv) const -> ObjectRef { return VALIDATE(m_ptr)->lookup(FWD(i), FWD(dv)); }

		template <class Index = ObjectRef
		> auto operator[](Index && i) const -> ObjectRef & { return VALIDATE(m_ptr)->operator[](FWD(i)); }

		bool empty() const { return VALIDATE(m_ptr)->empty(); }

		auto size() const { return VALIDATE(m_ptr)->size(); }

		auto begin() -> iterator { return VALIDATE(m_ptr)->begin(); }

		auto begin() const -> const_iterator { return VALIDATE(m_ptr)->begin(); }

		auto end() -> iterator { return VALIDATE(m_ptr)->end(); }

		auto end() const -> const_iterator { return VALIDATE(m_ptr)->end(); }
	};
}

#endif // !_PNU_DICT_OBJECT_HPP_
