#include <scene/resources/shader_library.hpp>
#include <servers/rendering_server.hpp>
#include <fstream>

using namespace pn;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ShaderBuilder
{
public:
	ShaderBuilder()
	{
	}

	~ShaderBuilder()
	{
	}

public:
	ShaderBuilder & version(String const & s)
	{
		return (*this);
	}

	ShaderBuilder & include(String const & s)
	{
		return (*this);
	}
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ load_shader_default(std::ifstream & file, RD::ShaderStageData (&spec)[RD::ShaderStage_MAX])
{
	RD::ShaderStage_ stage_index{ RD::ShaderStage_MAX };

	String line;
	while (std::getline(file, line))
	{
		bool should_write{ true };
		if (line.erase_duplicates(' ').empty()) { continue; }
		else if (line.front() == '#') {
			switch (line.hash_code()) {
			case "#pragma shader vertex"_hash: { stage_index = RD::ShaderStage_Vertex; should_write = false; } break;
			case "#pragma shader pixel"_hash: { stage_index = RD::ShaderStage_Fragment; should_write = false; } break;
			case "#pragma shader geometry"_hash: { stage_index = RD::ShaderStage_Geometry; should_write = false; } break;
			case "#pragma shader tess_ctrl"_hash: { stage_index = RD::ShaderStage_TesselationControl; should_write = false; } break;
			case "#pragma shader tess_eval"_hash: { stage_index = RD::ShaderStage_TesselationEvaluation; should_write = false; } break;
			case "#pragma shader compute"_hash: { stage_index = RD::ShaderStage_Compute; should_write = false; } break;
			}
		}
		if (should_write && stage_index != RD::ShaderStage_MAX)
		{
			spec[stage_index].code.printf("%.*s\n", line.size(), line.data());
		}
	}

	for (i32 i{}; i < RD::ShaderStage_MAX; ++i) {
		spec[i].shader_stage = (RD::ShaderStage_)i;
		if (!spec[i].code.empty() && spec[i].code.back() != '\0') { spec[i].code << '\0'; }
		//get_os()->printf("%.*s\n", spec[i].code.size(), spec[i].code.data());
	}

	return Error_OK;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Error_ ShaderLibrary::load_shader(Shader & shader, String const & path)
{
	if (path.empty()) { return Error_Unknown; }
	std::ifstream file{ path.c_str() };
	ON_SCOPE_EXIT(&) { file.close(); };
	if (!file) { return Error_Unknown; }

	switch (path.extension().hash_code())
	{
	case ".shader"_hash: {
		if (shader.m_shader) { RENDERING_DEVICE->shader_destroy(shader.m_shader); }
		RD::ShaderStageData spec[RD::ShaderStage_MAX]{};
		if (auto const err{ load_shader_default(file, spec) }; err != Error_OK) { return err; }
		shader.m_shader = RENDERING_DEVICE->shader_create(spec);
		if (!shader.m_shader) { return Error_Unknown; }
		return Error_OK;
	} break;
	}

	return Error_Unknown;
}

Error_ ShaderLibrary::load_shader(Ref<Shader> shader, String const & path)
{
	return !shader ? Error_Unknown : load_shader(**shader, path);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */