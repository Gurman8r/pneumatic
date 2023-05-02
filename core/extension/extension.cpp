#include <core/extension/extension.hpp>
#include <core/extension/extension_manager.hpp>
#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>
#include <core/io/config_file.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static ExtensionInterface extension_interface{};

	EMBED_CLASS(Extension, t) {}

	Extension::Extension()
	{
	}

	Extension::~Extension()
	{
		close_library();
	}

	Error_ Extension::open_library(String const & path, String const & entry_symbol)
	{
		if (Error_ const err{ get_os()->open_dynamic_library(path, m_library) }) {
			return err;
		}
		
		void * entry_func{};
		if (Error_ const err{ get_os()->get_dynamic_library_symbol(m_library, entry_symbol, entry_func, false) }) {
			get_os()->close_dynamic_library(m_library);
			return err;
		}

		ExtensionInitializationFunc initialization_function{ (ExtensionInitializationFunc)entry_func };
		if (initialization_function(&extension_interface, this, &m_initialization)) {
			m_level_initialized = -1;
			return Error_OK;
		}
		else {
			return Error_Unknown;
		}
	}

	void Extension::close_library()
	{
		if (m_library) {
			get_os()->close_dynamic_library(m_library);
			m_library = nullptr;
		}
	}

	bool Extension::is_library_open() const
	{
		return m_library;
	}

	ExtensionInitializationLevel_ Extension::get_minimum_level() const
	{
		return m_initialization.minimum_level;
	}

	void Extension::initialize_library(ExtensionInitializationLevel_ level)
	{
		ASSERT(m_library);
		ASSERT(level > m_level_initialized);
		m_level_initialized = level;
		ASSERT(m_initialization.initialize);
		m_initialization.initialize(m_initialization.user, level);
	}

	void Extension::finalize_library(ExtensionInitializationLevel_ level)
	{
		ASSERT(m_library);
		ASSERT(level <= m_level_initialized);
		m_level_initialized = level - 1;
		m_initialization.finalize(m_initialization.user, level);
	}

	void Extension::initialize_interface()
	{
	}

	String Extension::get_extension_list_config_file()
	{
		return get_project_settings()->get_config_path() + "extensions.cfg";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(ExtensionFormatLoader, t) {}

	RES ExtensionFormatLoader::load(String const & path, Error_ * error)
	{
		String const stem{ path.stem() };
		String const ini_path{ get_project_settings()->get_config_path() + stem };
		ConfigFile const ini{ ini_path };
		String const library_name{ ini.get_string("configuration", "library_name", stem) };
		String const entry_symbol{ ini.get_string("configuration", "entry_symbol", String::format("open_%s_library", library_name.c_str())) };
		String const dll_path{ get_project_settings()->get_bin_path() + library_name };

		Ref<Extension> extension;
		extension.instance();
		if (extension->open_library(dll_path, entry_symbol)) {
			extension = nullptr;
		}
		
		if (error) { *error = Error_OK; }
		return extension;
	}

	void ExtensionFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back("");
		out->push_back(".dll");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}