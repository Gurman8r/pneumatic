#ifndef _PN_MATERIAL_LIBRARY_HPP_
#define _PN_MATERIAL_LIBRARY_HPP_

#include <scene/resources/material.hpp>

namespace pn
{
	class PN_API MaterialLibrary final
	{
	public:
		static Error_ load_material(Material & material, String const & path);
		static Error_ load_material(Ref<Material> material, String const & path);
	};
}

#endif // !_PN_MATERIAL_LIBRARY_HPP_
