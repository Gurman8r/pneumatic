#ifndef _PNU_MONO_HPP_
#define _PNU_MONO_HPP_

#include <core/object/class.hpp>

#include <mono/jit/jit.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/class.h>
#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>
#include <mono/metadata/tokentype.h>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Fn = void(*)(MonoClass *)
	> void for_mono_class_interfaces(MonoClass * klass, Fn fn) { void * it{}; while (MonoClass * e{ mono_class_get_interfaces(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoClass *)
	> void for_mono_class_nested_types(MonoClass * klass, Fn fn) { void * it{}; while (MonoClass * e{ mono_class_get_nested_types(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoClassField *)
	> void for_mono_class_fields(MonoClass * klass, Fn fn) { void * it{}; while (MonoClassField * e{ mono_class_get_fields(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoMethod *)
	> void for_mono_class_methods(MonoClass * klass, Fn fn) { void * it{}; while (MonoMethod * e{ mono_class_get_methods(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoProperty *)
	> void for_mono_class_properties(MonoClass * klass, Fn fn) { void * it{}; while (MonoProperty * e{ mono_class_get_properties(klass, &it) }) { fn(e); } }

	template <class Fn = void(*)(MonoProperty *)
	> void for_mono_class_events(MonoClass * klass, Fn fn) { void * it{}; while (MonoEvent * e{ mono_class_get_events(klass, &it) }) { fn(e); } }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_MOD_API Mono
	{
		static Mono * __singleton;

		friend class CSharpLanguage;
		friend class CSharpScript;
		friend class CSharpInstance;

		MonoDomain *			m_dom{};
		Vector<MonoAssembly *>	m_asm{};
		Vector<MonoImage *>		m_img{};

		MonoClass * m_object_base{};
		MonoClass * m_script_base{};
		HashMap<String, MonoClass *> m_classes{};

	public:
		Mono() noexcept { SINGLETON_CTOR(); }
		~Mono() noexcept { SINGLETON_DTOR(); }
		FORCE_INLINE static Mono * get_singleton() noexcept { return __singleton; }

		bool initialize();
		bool finalize();

		bool load_assemblies();
	};

	SINGLETON_WRAPPER(Mono, get_mono);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_MONO_HPP_
