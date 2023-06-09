#ifndef _PNU_EXTENSION_HPP_
#define _PNU_EXTENSION_HPP_

#include <core/extension/extension_interface.hpp>

#include <core/io/resource_loader.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API Extension : public Resource
	{
		DEFINE_CLASS(Extension, Resource);

		String m_library_path{};
		void * m_library{};
		i32 m_level_initialized{ -1 };
		ExtensionInitialization m_initialization{};

	public:
		Extension();
		virtual ~Extension() override;

		Error_ open_library(String const & path, String const & entry_symbol);
		void close_library();

		NODISCARD bool is_library_open() const;
		NODISCARD ExtensionInitializationLevel_ get_minimum_level() const;
		
		void initialize_library(ExtensionInitializationLevel_ level);
		void finalize_library(ExtensionInitializationLevel_ level);

	public:
		static void initialize_interface();
		static String get_extension_list_config_file();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API ExtensionFormatLoader : public ResourceFormatLoader {
		DEFINE_CLASS(ExtensionFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_EXTENSION_HPP_
