#include <core/io/resource.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Resource, t, TypeFlags_IsAbstract)
	{
	}

	void Resource::set_path(String const & value, bool take_over)
	{
		if (take_over || m_path_cache != value) {
			m_path_cache = value;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RES load_resource(String const & path)
	{
		return get_resource_loader()->load(path);
	}

	Error_ save_resource(RES const & res, String const & path)
	{
		return get_resource_saver()->save(res, path);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}