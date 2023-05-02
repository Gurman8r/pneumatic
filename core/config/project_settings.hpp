#ifndef _PNU_PROJECT_SETTINGS_HPP_
#define _PNU_PROJECT_SETTINGS_HPP_

#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API ProjectSettings : public Object
	{
		DEFINE_CLASS(ProjectSettings, Object);

		static ProjectSettings * __singleton;

		String
			m_bin_path		{},
			m_cache_path	{},
			m_config_path	{},
			m_data_path		{},
			m_resource_path	{},
			m_user_path		{};

		HashMap<String, HashMap<String, ObjectRef>> m_data{};

	public:
		ProjectSettings() noexcept { SINGLETON_CTOR(); }
		virtual ~ProjectSettings() noexcept override { SINGLETON_DTOR(); }
		FORCE_INLINE static ProjectSettings * get_singleton() noexcept { return __singleton; }

	public:
		Error_ setup(String const & exepath, String const & main_pack = {});

		NODISCARD String globalize_path(String const & path) const;
		NODISCARD String get_bin_path() const;
		NODISCARD String get_cache_path() const;
		NODISCARD String get_config_path() const;
		NODISCARD String get_data_path() const;
		NODISCARD String get_resource_path() const;
		NODISCARD String get_user_path() const;

		NODISCARD ObjectRef get(String const & section, String const & name) const;
		Error_ set(String const & section, String const & name, ObjectRef const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_WRAPPER(ProjectSettings, get_project_settings);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_PROJECT_SETTINGS_HPP_
