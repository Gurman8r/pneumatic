#include <core/io/dir_access.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DirAccess, t, TypeFlags_IsAbstract) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	DirAccess::CreateFunc DirAccess::__create_func{};

	Ref<DirAccess> DirAccess::create()
	{
		return nullptr;
	}

	Ref<DirAccess> DirAccess::create_for_path(String const & path)
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<DirAccess> DirAccess::open(String const & path, Error_ * error)
	{
		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}