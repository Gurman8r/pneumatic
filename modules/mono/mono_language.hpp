#ifndef _PNU_MONO_LANGUAGE_HPP_
#define _PNU_MONO_LANGUAGE_HPP_

#include <core/object/script.hpp>

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
	// mono language
	class PNU_MOD_API MonoLanguage : public ScriptLanguage
	{
		DEFINE_CLASS(MonoLanguage, ScriptLanguage);

		static MonoLanguage * __singleton;

		MonoDomain * m_domain{};
		MonoAssembly * m_assembly{};
		MonoImage * m_image{};

	public:
		MonoLanguage();
		virtual ~MonoLanguage() override;

		virtual String get_name() const override { return "mono"; }

		virtual void initialize() override;
		virtual void finalize() override;

		void reload_assemblies(bool soft_reload);
	};
}

#endif // !_PNU_MONO_LANGUAGE_HPP_
