#include <scene/resources/material_library.hpp>

namespace pn
{
	Error_ MaterialLibrary::load_material(Material & material, String const & path)
	{
		if (path.empty()) { return Error_Unknown; }

		return Error_OK;
	}

	Error_ MaterialLibrary::load_material(Ref<Material> material, String const & path)
	{
		return !material ? Error_Unknown : load_material(**material, path);
	}
}