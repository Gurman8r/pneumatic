#ifndef _PN_RESOURCE_HPP_
#define _PN_RESOURCE_HPP_

#include <core/object/class.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OPAQUE_TYPE(RID);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// resource
	class PN_API Resource : public Object
	{
		DEFINE_CLASS(Resource, Object);

		i32 m_subindex{};
		String m_name{};
		String m_path_cache{};

	protected:
		Resource() noexcept {}

	public:
		virtual ~Resource() noexcept override = default;

		virtual RID get_rid() const { return {}; }

		i32 get_subindex() const { return m_subindex; }
		void set_subindex(i32 value) { m_subindex = value; }

		String const & get_name() const { return m_name; }
		void set_name(String const & value) { m_name = value; }

		String const & get_path() const { return m_path_cache; }
		virtual void set_path(String const & value, bool take_over = true);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using RES = Ref<Resource>;
	PN_API_FUNC(RES) load_resource(String const & path);
	PN_API_FUNC(Error_) save_resource(RES const & res, String const & path);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_RESOURCE_HPP_
