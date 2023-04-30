#ifndef _PN_MESH_LIBRARY_HPP_
#define _PN_MESH_LIBRARY_HPP_

#include <scene/resources/mesh.hpp>

namespace pn
{
	class PN_API MeshLibrary final
	{
	public:
		static Error_ load_mesh(Mesh & mesh, String const & path);
		static Error_ load_mesh(Ref<Mesh> mesh, String const & path);
	};
}

#endif // !_PN_MESH_LIBRARY_HPP_
