#include <scene/resources/mesh_library.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Mesh, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh(Vector<RS::SurfaceData> const & surfaces)
	{
		if (!surfaces.empty()) { m_mesh = get_rendering_server()->mesh_create(surfaces); }
	}

	Mesh::Mesh(String const & path)
	{
		ASSERT(MeshLibrary::load_mesh(*this, path) == Error_OK);
	}

	Mesh::~Mesh()
	{
		if (m_mesh) { get_rendering_server()->mesh_destroy(m_mesh); m_mesh = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	size_t Mesh::get_surface_count() const
	{
		return get_rendering_server()->mesh_get_surface_count(m_mesh);
	}

	RS::Primitive_ Mesh::surface_get_primitive(size_t index) const
	{
		return get_rendering_server()->mesh_surface_get_primitive(m_mesh, index);
	}

	RID Mesh::surface_get_vertex_array(size_t index) const
	{
		return get_rendering_server()->mesh_surface_get_vertex_array(m_mesh, index);
	}

	RID Mesh::surface_get_index_array(size_t index) const
	{
		return get_rendering_server()->mesh_surface_get_index_array(m_mesh, index);
	}

	RID Mesh::surface_get_material(size_t index) const
	{
		return get_rendering_server()->mesh_surface_get_material(m_mesh, index);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}