#ifndef _PN_LIST_OBJECT_HPP_
#define _PN_LIST_OBJECT_HPP_

#include <core/object/builtins/type_object.hpp>

// list
namespace pn
{
	// list object
	class PN_API ListObject : public Object
	{
		DEFINE_CLASS(ListObject, Object);

		friend class LIST;

	public:
		Vector<OBJ> m_list{};

		using storage_type		= decltype(m_list);
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		auto & operator*() const { return (storage_type &)(m_list); }

		auto * operator->() const { return (storage_type *)(&m_list); }

		operator storage_type & () const { return (storage_type &)(m_list); }

		template <class Iter
		> ListObject(Iter first, Iter last) : m_list{ first, last } {}

		ListObject() noexcept : m_list{} {}

		ListObject(storage_type const & value) : m_list{ value } {}

		ListObject(storage_type && value) noexcept : m_list{ std::move(value) } {}

		ListObject(std::initializer_list<OBJ> init) : m_list{ init } {}

		ListObject & operator=(std::initializer_list<OBJ> init) { return (m_list = init), (*this); }

		void clear() { m_list.clear(); }

		void reserve(size_t count) { m_list.reserve(count); }

		void resize(size_t count) { m_list.resize(count); }

		template <class Value = OBJ
		> void resize(size_t count, Value && value) { m_list.resize(count, FWD_OBJ(value)); }

		template <class Index = OBJ
		> auto del(Index && i) -> Error_
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return m_list.erase(begin() + static_cast<size_t>(i)), Error_OK;
			}
			else
			{
				return m_list.erase(begin() + FWD_OBJ(i).cast<size_t>()), Error_OK;
			}
		}

		template <class Value = OBJ
		> auto append(Value && v) -> OBJ & { return m_list.emplace_back(FWD_OBJ(v)); }

		template <class Index = OBJ, class Value = OBJ
		> void insert(Index && i, Value && v)
		{
			if constexpr (std::is_integral_v<Index>)
			{
				m_list.insert(begin() + i, FWD_OBJ(v));
			}
			else
			{
				m_list.insert(begin() + i.cast<size_t>(), FWD_OBJ(v));
			}
		}

		template <class Value = OBJ
		> bool contains(Value && v) const { return m_list.contains(FWD_OBJ(v)); }

		template <class Value = OBJ
		> auto find(Value && v) -> iterator { return m_list.find(FWD_OBJ(v)); }

		template <class Value = OBJ
		> auto find(Value && v) const -> const_iterator { return m_list.find(FWD_OBJ(v)); }

		template <class Value = OBJ
		> auto lookup(Value && v) const -> OBJ { return this->lookup(FWD_OBJ(v), OBJ{}); }

		template <class Value = OBJ, class Defval = OBJ
		> auto lookup(Value && v, Defval && dv) const -> OBJ
		{
			if (auto const ptr{ util::getptr(m_list, FWD_OBJ(v)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> auto operator[](Index && i) const -> OBJ
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return m_list[static_cast<size_t>(i)];
			}
			else
			{
				return m_list[FWD_OBJ(i).cast<size_t>()];
			}
		}

		auto data() const { return m_list.data(); }

		bool empty() const { return m_list.empty(); }

		auto size() const { return m_list.size(); }

		auto begin() -> iterator { return m_list.begin(); }

		auto begin() const -> const_iterator { return m_list.begin(); }

		auto end() -> iterator { return m_list.end(); }

		auto end() const -> const_iterator { return m_list.end(); }
	};

	// list delete
	template <> struct DefaultDelete<ListObject> : DefaultDelete<Object> {};

	// list check
#define OBJECT_CHECK_LIST(o) (pn::typeof(o).has_feature(pn::TypeFlags_List_Subclass))

	// list ref
	class LIST : public Ref<ListObject>
	{
		REF_CLASS(LIST, OBJECT_CHECK_LIST);

	public:
		using storage_type		= value_type::storage_type;
		using iterator			= value_type::iterator;
		using const_iterator	= value_type::const_iterator;

		operator storage_type & () const { return VALIDATE(m_ptr)->operator storage_type & (); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error_ { return VALIDATE(m_ptr)->del(FWD(i)); }

		void clear() const { VALIDATE(m_ptr)->clear(); }

		void reserve(size_t count) const { VALIDATE(m_ptr)->reserve(count); }

		void resize(size_t count) const { VALIDATE(m_ptr)->resize(count); }

		template <class Value = OBJ
		> void resize(size_t count, Value && value) const { VALIDATE(m_ptr)->resize(count, FWD(value)); }

		template <class Value = OBJ
		> auto append(Value && v) const -> OBJ & { return VALIDATE(m_ptr)->append(FWD(v)); }

		template <class Index = OBJ, class Value = OBJ
		> void insert(Index && i, Value && v) { return VALIDATE(m_ptr)->insert(FWD(i), FWD(v)); }

		template <class Value = OBJ
		> bool contains(Value && v) const { return VALIDATE(m_ptr)->contains(FWD(v)); }

		template <class Value = OBJ
		> auto find(Value && v) -> iterator { return VALIDATE(m_ptr)->find(FWD(v)); }

		template <class Value = OBJ
		> auto find(Value && v) const -> const_iterator { return VALIDATE(m_ptr)->find(FWD(v)); }

		template <class Value = OBJ
		> auto lookup(Value && v) const -> OBJ { return VALIDATE(m_ptr)->lookup(FWD(v)); }

		template <class Value = OBJ, class Defval = OBJ
		> auto lookup(Value && v, Defval && dv) const -> OBJ { return VALIDATE(m_ptr)->lookup(FWD(v), FWD(dv)); }

		template <class Index = OBJ
		> auto operator[](Index && i) const -> OBJ { return VALIDATE(m_ptr)->operator[](FWD(i)); }

		auto data() const { return VALIDATE(m_ptr)->data(); }

		bool empty() const { return VALIDATE(m_ptr)->empty(); }

		auto size() const { return VALIDATE(m_ptr)->size(); }

		auto begin() -> iterator { return VALIDATE(m_ptr)->begin(); }

		auto begin() const -> const_iterator { return VALIDATE(m_ptr)->begin(); }

		auto end() -> iterator { return VALIDATE(m_ptr)->end(); }

		auto end() const -> const_iterator { return VALIDATE(m_ptr)->end(); }
	};
}

#endif // !_PN_LIST_OBJECT_HPP_
