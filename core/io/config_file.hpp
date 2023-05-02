#ifndef _PNU_CONFIG_FILE_HPP_
#define _PNU_CONFIG_FILE_HPP_

#include <core/io/resource.hpp>

namespace Pnu
{
	class PNU_API ConfigFile : public Resource
	{
		DEFINE_CLASS(ConfigFile, Resource);

		using Section = HashMap<String, String>;
		HashMap<String, Section> m_data{};

	public:
		ConfigFile() noexcept {}
		explicit ConfigFile(String const & path);
		virtual ~ConfigFile();
		virtual RID get_rid() const override { return 0; }

	public:
		NODISCARD bool get_bool(String const & section, String const & name, bool default_value = {}) const;
		NODISCARD i32 get_i32(String const & section, String const & name, i32 default_value = {}) const;
		NODISCARD u32 get_u32(String const & section, String const & name, u32 default_value = {}) const;
		NODISCARD f32 get_f32(String const & section, String const & name, f32 default_value = {}) const;
		NODISCARD f64 get_f64(String const & section, String const & name, f64 default_value = {}) const;
		NODISCARD String get_string(String const & section, String const & name, String const & default_value = {}) const;
		bool set_string(String const & section, String const & name, String const & value);
	};
}

#endif // !_PNU_CONFIG_FILE_HPP_
