#include <core/extension/extension_manager.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/file_access.hpp>
#include <core/io/resource_loader.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ExtensionManager, t) {}

	ExtensionManager * ExtensionManager::__singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ExtensionManager::ExtensionManager()
	{
		SINGLETON_CTOR();
	}

	ExtensionManager::~ExtensionManager()
	{
		SINGLETON_DTOR();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ExtensionManager::LoadStatus_ ExtensionManager::load_extension(String const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it != m_extensions.end()) { return LoadStatus_AlreadyLoaded; }

		
		Ref<Extension> extension{ get_resource_loader()->load(path) };
		if (!extension) { return LoadStatus_Failure; }

		if (m_level >= 0) {
			i32 const minimum_level{ extension->get_minimum_level() };
			if (minimum_level < MIN(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i{ minimum_level }; i <= m_level; ++i) {
				extension->initialize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions[path] = extension;
		return LoadStatus_Success;
	}

	ExtensionManager::LoadStatus_ ExtensionManager::unload_extension(String const & path)
	{
		auto const it{ m_extensions.find(path) };
		if (it == m_extensions.end()) { return LoadStatus_NotLoaded; }

		Ref<Extension> extension{ it->second };

		if (m_level >= 0) {
			i32 const minimum_level{ extension->get_minimum_level() };
			if (minimum_level < MIN(m_level, ExtensionInitializationLevel_Scene)) {
				return LoadStatus_NeedsRestart;
			}
			for (i32 i{ minimum_level }; i <= m_level; ++i) {
				extension->finalize_library((ExtensionInitializationLevel_)i);
			}
		}

		m_extensions.erase(it);
		return LoadStatus_Success;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ExtensionManager::is_extension_loaded(String const & path)
	{
		return m_extensions.contains(path);
	}

	Vector<String> ExtensionManager::get_loaded_extensions() const
	{
		Vector<String> ext{};
		ext.reserve(m_extensions.size());
		for (auto const & [k, v] : m_extensions) {
			ext.push_back(k);
		}
		return ext;
	}

	Ref<Extension> ExtensionManager::get_extension(String const & path)
	{
		if (auto const it{ m_extensions.find(path) }; it != m_extensions.end()) {
			return it->second;
		}
		return nullptr;
	}

	String ExtensionManager::get_extension_list_config_file() const
	{
		return get_project_settings()->get_config_path() + "extensions.cfg";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ExtensionManager::load_extensions()
	{
		auto f{ FileAccess::open(Extension::get_extension_list_config_file(), FileMode_Read) };
		if (!f) { return; }
		while (String line{ f->read_line().trim() }) {
			load_extension(line);
		}
	}

	void ExtensionManager::initialize_extensions(ExtensionInitializationLevel_ level)
	{
		for (auto & [k, v] : m_extensions) {
			v->initialize_library(level);
		}
		m_level = level;
	}

	void ExtensionManager::finalize_extensions(ExtensionInitializationLevel_ level)
	{
		for (auto & [k, v] : m_extensions) {
			v->finalize_library(level);
		}
		m_level = (i32)level - 1;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}