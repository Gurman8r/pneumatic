#include <scene/resources/mesh_library.hpp>
#include <scene/resources/texture.hpp>
#include <servers/rendering_server.hpp>
#include <servers/rendering/renderer_storage.hpp>

using namespace Pnu;

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t process_vertices(aiMesh const * mesh, DynamicBuffer & data)
{
	for (size_t i{}; i < (size_t)mesh->mNumVertices; ++i)
	{
		// position
		data << Vec4f{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.f };

		// normals
		if (mesh->HasNormals())
		{
			data << Vec4f{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1.f };
		}
		else
		{
			data << Vec4f{};
		}

		// texcoords
		if (mesh->HasTextureCoords(0))
		{
			data << Vec4f{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, mesh->mTextureCoords[0][i].z, 1.f };
		}
		else
		{
			data << Vec4f{};
		}

		// tangents & bitantents
		if (mesh->HasTangentsAndBitangents())
		{
			data << Vec4f{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z, 1.f };
			data << Vec4f{ mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z, 1.f };
		}
		else
		{
			data << Vec4f{} << Vec4f{};
		}
	}

	// TODO: properly deduce type
	return data.size() / sizeof(f32);
}

size_t process_indices(aiMesh const * mesh, DynamicBuffer & data)
{
	for (size_t i{}; i < (size_t)mesh->mNumFaces; ++i)
	{
		aiFace face{ mesh->mFaces[i] };

		for (size_t j{}; j < (size_t)face.mNumIndices; ++j)
		{
			data << face.mIndices[j];
		}
	}

	// TODO: properly deduce type
	return data.size() / sizeof(u32);
}

void process_aimaterial(aiMaterial const * material, Vector<Ref<Texture>> & textures)
{
	if (!material) { return; }

	auto _load_material_texture = [&](aiTextureType type, cstring type_name)
	{
		for (u32 i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString path;
			if (material->GetTexture(type, i, &path) != aiReturn_SUCCESS) { continue; }
			Ref<ImageTexture> texture{ String{ path.C_Str() } };
			textures.push_back(texture);
		}
	};
	_load_material_texture(aiTextureType_DIFFUSE, "dm"); // diffuse
	_load_material_texture(aiTextureType_AMBIENT, "hm"); // height
	_load_material_texture(aiTextureType_LIGHTMAP, "lm"); // light
	_load_material_texture(aiTextureType_HEIGHT, "nm"); // normal
	_load_material_texture(aiTextureType_SPECULAR, "sm"); // specular
}

void process_ainode(aiScene const * scene, aiNode const * node, Vector<RS::SurfaceData> & spec)
{
	for (size_t i{}; i < (size_t)node->mNumMeshes; ++i)
	{
		aiMesh const * m{ scene->mMeshes[node->mMeshes[i]] };

		RS::SurfaceData & s{ spec.emplace_back(RS::SurfaceData{}) };
		s.primitive = RS::Primitive_Triangles;
		s.vertex_count = process_vertices(m, s.vertex_data);
		s.index_count = process_indices(m, s.index_data);

		//Vector<Ref<Texture>> textures;
		//process_aimaterial(scene->mMaterials[m->mMaterialIndex], textures);
	}

	for (size_t i{}; i < (size_t)node->mNumChildren; ++i)
	{
		process_ainode(scene, node->mChildren[i], spec);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ MeshLibrary::load_mesh(Mesh & mesh, String const & path)
{
	if (path.empty()) { return Error_Unknown; }
	if (mesh.m_mesh) { get_rendering_server()->mesh_destroy(mesh.m_mesh); }

	Assimp::Importer ai;
	aiScene const * scene{ ai.ReadFile(path.c_str(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_GenSmoothNormals |
		aiProcess_GenUVCoords) };
	ON_SCOPE_EXIT(&) { ai.FreeScene(); };

	Vector<RS::SurfaceData> spec;
	process_ainode(scene, scene->mRootNode, spec);
	mesh.m_mesh = get_rendering_server()->mesh_create(spec);
	if (!mesh.m_mesh) { return Error_Unknown; }
	return Error_OK;
}

Error_ MeshLibrary::load_mesh(Ref<Mesh> mesh, String const & path)
{
	return !mesh ? Error_Unknown : load_mesh(**mesh, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */