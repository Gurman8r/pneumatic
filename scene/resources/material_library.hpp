#ifndef _PNU_MATERIAL_LIBRARY_HPP_
#define _PNU_MATERIAL_LIBRARY_HPP_

#include <scene/resources/material.hpp>

namespace Pnu
{
	class PNU_API MaterialLibrary final
	{
	public:
		static Error_ load_material(Material & material, String const & path);
		static Error_ load_material(Ref<Material> material, String const & path);
	};
}

#endif // !_PNU_MATERIAL_LIBRARY_HPP_
