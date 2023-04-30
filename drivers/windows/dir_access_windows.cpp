#include <drivers/windows/dir_access_windows.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <filesystem>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(DirAccessWindows, t) {};

	DirAccessWindows::DirAccessWindows() {}

	DirAccessWindows::~DirAccessWindows() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void DirAccessWindows::initialize()
	{
		DirAccess::__create_func = []() -> Ref<DirAccess> { return memnew(DirAccessWindows); };
	}

	void DirAccessWindows::finalize()
	{
		DirAccess::__create_func = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}