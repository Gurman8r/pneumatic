#include <modules/mono/mono.hpp>
#include <core/config/project_settings.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline void CS_Pneumatic_Object_print(MonoString * message) {
		PRINT_LINE(mono_string_to_utf8(message));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Mono * Mono::__singleton{};

	bool Mono::initialize()
	{
		String const lib_dir{ "C:\\Program Files\\Mono\\lib" };
		String const etc_dir{ "C:\\Program Files\\Mono\\etc" };
		mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

		if (!(m_dom = mono_jit_init("pneumatic"))) {
			return false;
		}

		if (!load_assemblies()) {
			return false;
		}

		return true;
	}

	bool Mono::finalize()
	{
		if (m_dom) { mono_jit_cleanup(m_dom); m_dom = nullptr; }

		return true;
	}

	bool Mono::load_assemblies()
	{
		m_img.push_back(mono_assembly_get_image(m_asm.emplace_back(mono_domain_assembly_open(m_dom, "./binaries/pneumatic-CS.dll"))));
		mono_add_internal_call("Pnu.Object::print", &CS_Pneumatic_Object_print);
		if (!(m_object_base = mono_class_from_name(m_img[0], "Pnu", "Object"))) { return false; }
		if (!(m_script_base = mono_class_from_name(m_img[0], "Pnu", "Script"))) { return false; }

		m_img.push_back(mono_assembly_get_image(m_asm.emplace_back(mono_domain_assembly_open(m_dom, "./binaries/demo-CS.dll"))));
		i32 const rows{ mono_image_get_table_rows(m_img[1], MONO_TABLE_TYPEDEF) };
		for (i32 i{ 1 }; i < rows; ++i)
		{
			MonoClass * klass{ mono_class_get(m_img[1], (i + 1) | MONO_TOKEN_TYPE_DEF) };
			if (!mono_class_is_subclass_of(klass, m_script_base, false)) { continue; }
			cstring const space{ mono_class_get_namespace(klass) };
			cstring const name{ mono_class_get_name(klass) };
			char signature[128]; sprintf(signature, "%s.%s", space, name);
			if (!m_classes.insert({ signature, klass }).second) { continue; }
			printf("Class: %s\n", signature);

			for_mono_class_interfaces(klass, [&](MonoClass * t) {
				printf("Implements: %s\n", mono_class_get_name(t));
			});

			for_mono_class_nested_types(klass, [&](MonoClass * t) {
				printf("Nested: %s\n", mono_class_get_name(t));
			});

			for_mono_class_fields(klass, [&](MonoClassField * f) {
				printf("Field: %s, flags 0x%x\n", mono_field_get_name(f), mono_field_get_flags(f));
			});

			for_mono_class_methods(klass, [&](MonoMethod * m) {
				u32 iflags{}, flags{ mono_method_get_flags(m, &iflags) };
				printf("Method: %s, flags 0x%x, iflags 0x%x\n", mono_method_get_name(m), flags, iflags);
			});

			for_mono_class_properties(klass, [&](MonoProperty * p) {
				printf("Property: %s, flags 0x%x\n", mono_property_get_name(p), mono_property_get_flags(p));
			});

			for_mono_class_events(klass, [&](MonoEvent * e) {
				printf("Event: %s, flags: 0x%x\n", mono_event_get_name(e), mono_event_get_flags(e));
			});

			printf("\n");

			auto get_method = [klass](cstring t, cstring f, cstring a = "") {
				char signature[128]; sprintf(signature, ".%s:%s(%s)", t, f, a);
				auto const d{ mono_method_desc_new(signature, false) };
				auto const m{ mono_method_desc_search_in_class(d, klass) };
				mono_method_desc_free(d);
				return m;
			};
			if (MonoMethod * method{ get_method(name, "SayHello") }) {
				mono_runtime_invoke(method, nullptr, nullptr, nullptr);
			}
		}

		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}