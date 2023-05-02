#include <core/config/project_settings.hpp>
#include <core/object/eval.hpp>
#include <inih/INIReader.h>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ProjectSettings, t) {}

	ProjectSettings * ProjectSettings::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ ProjectSettings::setup(String const & exepath, String const & main_pack)
	{
		if (exepath.empty()) {
			return Error_Unknown;
		}

		m_bin_path = "./binaries/";
		m_cache_path = "./cache/";
		m_config_path = "./config/";
		m_data_path = "./data/";
		m_resource_path = "./resources/";
		m_user_path = "./user/";

		// engine settings
		String const engine_ini{ get_config_path() + "engine.ini" };
		ini_parse(engine_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);

		// editor settings
#if TOOLS_ENABLED
		String const editor_ini{ get_config_path() + "editor.ini" };
		ini_parse(editor_ini.c_str(), [](auto user, auto section, auto name, auto value) {
			((ProjectSettings *)user)->set(section, name, evaluate(value));
			return 1;
		}, this);
#endif

		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String ProjectSettings::globalize_path(String const & path) const
	{
		return path;
	}

	String ProjectSettings::get_bin_path() const
	{
		return m_bin_path;
	}

	String ProjectSettings::get_config_path() const
	{
		return m_config_path;
	}

	String ProjectSettings::get_cache_path() const
	{
		return m_cache_path;
	}

	String ProjectSettings::get_data_path() const
	{
		return m_data_path;
	}

	String ProjectSettings::get_resource_path() const
	{
		return m_resource_path;
	}

	String ProjectSettings::get_user_path() const
	{
		return m_user_path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ObjectRef ProjectSettings::get(String const & section, String const & name) const
	{
		if (auto const s{ util::getptr(m_data, section) })
		{
			if (auto const v{ util::getptr(*s, name) })
			{
				return *v;
			}
		}
		return nullptr;
	}

	Error_ ProjectSettings::set(String const & section, String const & name, ObjectRef const & value)
	{
		// add
		if (value) {
			m_data[section][name] = value;
			return Error_OK;
		}
		// remove
		else if (auto const s{ m_data.find(section) }; s != m_data.end()) {
			if (auto const v{ s->second.find(name) }; v != s->second.end()) { s->second.erase(v); }
			else { return Error_Unknown; } // value not found
			if (s->second.empty()) { m_data.erase(s); }
			return Error_OK;
		}
		else {
			return Error_Unknown; // section not found
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}