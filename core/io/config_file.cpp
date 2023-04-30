#include <core/io/config_file.hpp>
#include <inih/INIReader.h>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ConfigFile, t) {}

	ConfigFile::~ConfigFile() {}

	ConfigFile::ConfigFile(String const & path)
	{
		if (path.empty()) { return; }
		if (ini_parse(path.c_str(), [](auto user, auto section, auto name, auto value) {
			((ConfigFile *)user)->set_string(section, name, String{ value }.trim([](char c) { return c == ' ' || c == '\'' || c == '\"'; }));
			return 1;
		}, this)) {
			set_path(path);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ConfigFile::get_bool(String const & section, String const & name, bool default_value) const
	{
		if (auto const v{ util::to_bool(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	i32 ConfigFile::get_i32(String const & section, String const & name, i32 default_value) const
	{
		if (auto const v{ util::to_i32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	u32 ConfigFile::get_u32(String const & section, String const & name, u32 default_value) const
	{
		if (auto const v{ util::to_u32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f32 ConfigFile::get_f32(String const & section, String const & name, f32 default_value) const
	{
		if (auto const v{ util::to_f32(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	f64 ConfigFile::get_f64(String const & section, String const & name, f64 default_value) const
	{
		if (auto const v{ util::to_f64(get_string(section, name)) }) { return *v; }
		else { return default_value; }
	}

	String ConfigFile::get_string(String const & section, String const & name, String const & default_value) const
	{
		if (auto const s{ util::getptr(m_data, section) })
		{
			if (auto const v{ util::getptr(*s, name) })
			{
				return *v;
			}
		}
		return default_value;
	}

	bool ConfigFile::set_string(String const & section, String const & name, String const & value)
	{
		return m_data[section].insert({ name, value }).second;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}