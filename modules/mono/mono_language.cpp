#include <modules/mono/mono_language.hpp>
#include <core/config/project_settings.hpp>

namespace Pnu
{
	EMBED_CLASS(MonoLanguage, t) {}

	MonoLanguage * MonoLanguage::__singleton{};

	MonoLanguage::MonoLanguage() { SINGLETON_CTOR(); }

	MonoLanguage::~MonoLanguage() { SINGLETON_DTOR(); }

	static void print_all_types(MonoImage * image)
	{
		i32 const rows{ mono_image_get_table_rows(image, MONO_TABLE_TYPEDEF) };
		for (i32 i{ 1 }; i < rows; ++i) {
			MonoClass * klass{ mono_class_get(image, (i + 1) | MONO_TOKEN_TYPE_DEF) };
			printf("Class: %s.%s\n", mono_class_get_namespace(klass), mono_class_get_name(klass));
		}
	}

	void MonoLanguage::initialize()
	{
		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
		m_domain = mono_jit_init("demo-CS");
		m_assembly = VALIDATE(mono_domain_assembly_open(m_domain, ".\\binaries\\demo-CS.dll"));
		m_image = VALIDATE(mono_assembly_get_image(m_assembly));
		print_all_types(m_image);
		MonoClass * klass{ mono_class_from_name(m_image, "Pnu", "Test") };
		MonoMethodDesc * desc{ mono_method_desc_new(".Test:SayHello()", false) };
		MonoMethod * method{ mono_method_desc_search_in_class(desc, klass) };
		mono_method_desc_free(desc);
		mono_runtime_invoke(method, nullptr, nullptr, nullptr);
	}

	void MonoLanguage::finalize()
	{
		mono_jit_cleanup(m_domain);
	}
	
	void MonoLanguage::reload_assemblies(bool soft_reload)
	{
	}
}
