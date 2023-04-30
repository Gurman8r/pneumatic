#ifndef _PNU_MESH_HPP_
#define _PNU_MESH_HPP_

#include <servers/rendering_server.hpp>

namespace Pnu
{
	class PNU_API Mesh : public Resource
	{
		DEFINE_CLASS(Mesh, Resource);

		friend class MeshLibrary;

		RID m_mesh{};

	public:
		Mesh() noexcept {}

		explicit Mesh(Vector<RS::SurfaceData> const & surfaces);

		explicit Mesh(RS::SurfaceData const & surface) : Mesh{ Vector<RS::SurfaceData>{ surface } } {}

		explicit Mesh(String const & path);

		virtual ~Mesh() override;

		virtual RID get_rid() const override { return m_mesh; }

	public:
		virtual size_t get_surface_count() const;
		virtual RS::Primitive_ surface_get_primitive(size_t index) const;
		virtual RID surface_get_vertex_array(size_t index) const;
		virtual RID surface_get_index_array(size_t index) const;
		virtual RID surface_get_material(size_t index) const;
	};
}

#endif // !_PNU_MESH_HPP_
