#ifndef _PNU_CSHARP_HPP_
#define _PNU_CSHARP_HPP_

#include <modules/mono/mono.hpp>
#include <core/object/script.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class CSharpScript;
	class CSharpInstance;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp language
	class PNU_MOD_API CSharpLanguage : public ScriptLanguage
	{
		DEFINE_CLASS(CSharpLanguage, ScriptLanguage);

		static CSharpLanguage * __singleton;

		friend class CSharpScript;
		friend class CSharpInstance;

		Mono * m_mono{};

	public:
		CSharpLanguage();
		virtual ~CSharpLanguage() override;
		FORCE_INLINE static CSharpLanguage * get_singleton() noexcept { return __singleton; }

		virtual String get_name() const override { return "mono"; }

		virtual void initialize() override;
		virtual void finalize() override;

		void reload_assemblies(bool soft_reload);
	};

	SINGLETON_WRAPPER(CSharpLanguage, get_csharp);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script
	class PNU_MOD_API CSharpScript : public Script
	{
		DEFINE_CLASS(CSharpScript, Script);

		ScriptLanguage * const m_language{};
		String m_source{};
		mutable bool m_changed{};

	public:
		CSharpScript();
		explicit CSharpScript(String const & path);
		virtual ~CSharpScript() override;

		virtual Ref<ScriptInstance> instance_create(Object * self) override;

		virtual void get_field_names(Vector<String> * out) const override;
		virtual void get_method_names(Vector<String> * out) const override;
		virtual void get_property_names(Vector<String> * out) const override;
		
		virtual bool has_source_code() const override { return !m_source.empty(); }
		virtual String get_source_code() const override { return m_source; }
		virtual void set_source_code(String const & value) override { if (m_source != value) { m_source = value; m_changed = true; } }
		virtual Error_ reload(bool keep_state = false) override;

		virtual ScriptLanguage * get_language() const override { return m_language; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp instance
	class PNU_MOD_API CSharpInstance : public ScriptInstance
	{
		DEFINE_CLASS(CSharpInstance, ScriptInstance);

		ScriptLanguage * const m_language{};
		Ref<Script> m_script{};
		Object * m_owner{};

		MonoObject * m_handle{};

	public:
		CSharpInstance(Ref<Script> script, Object * owner);
		virtual ~CSharpInstance() override;

		virtual void notification(i32 notification) override;

		virtual bool is_placeholder() const override { return false; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// csharp script loader
	class PNU_MOD_API CSharpScriptFormatLoader : public ResourceFormatLoader {
		DEFINE_CLASS(CSharpScriptFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_CSHARP_HPP_
