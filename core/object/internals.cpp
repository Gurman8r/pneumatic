#include <core/object/internals.hpp>
#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Internals * Internals::__singleton{};

	Internals::Internals()
	{
		SINGLETON_CTOR();

		m_modules = DICT::new_();
	}

	Internals::~Internals()
	{
		SINGLETON_DTOR();
		Vector<OBJ>{}.swap(m_loader_stack);
		m_modules = nullptr;
		while (!m_classes.empty()) {
			if (auto const type{ m_classes.back<TypeObject *>() }) { type->cleanup(); }
			m_classes.pop_back();
		}
	}

	void Internals::add_class(TypeObject * type)
	{
		ASSERT(type);
		ASSERT(type->ready());
		auto const id{ type->tp_name.hash_code() };
		ASSERT(!m_classes.contains<size_t>(id));
		m_classes.push_back(id, type->tp_name, type);
	}

	bool Internals::del_class(String const & name)
	{
		auto const index{ m_classes.index_of<0>(name.hash_code()) };
		if (index == m_classes.npos) { return false; }
		if (auto const type{ m_classes.get<TypeObject *>(index) }) { type->cleanup(); }
		m_classes.erase(index);
		return true;
	}

	TypeObject * Internals::get_class(String const & name) const
	{
		if (auto const type{ m_classes.map<size_t, TypeObject *>(name.hash_code()) }) {
			return *type;
		}
		else {
			return nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}