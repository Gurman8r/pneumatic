#ifndef _PNU_MONO_SCRIPT_HPP_
#define _PNU_MONO_SCRIPT_HPP_

#include <modules/mono/mono_language.hpp>
#include <core/io/resource_loader.hpp>
#include <core/io/resource_saver.hpp>

namespace Pnu
{
	// mono script
	class PNU_MOD_API MonoScript : public Script
	{
		DEFINE_CLASS(MonoScript, Script);

	public:
		MonoScript();
		explicit MonoScript(String const & path);
		virtual ~MonoScript() override;
		
		virtual bool has_source_code() const override;
		virtual String get_source_code() const override;
		virtual void set_source_code(String const & code) override;
		virtual Error_ reload(bool keep_state = false) override;
	};
}

namespace Pnu
{
	// mono script loader
	class PNU_MOD_API MonoScriptFormatLoader : public ResourceFormatLoader {
		DEFINE_CLASS(MonoScriptFormatLoader, ResourceFormatLoader);
	public:
		virtual RES load(String const & path, Error_ * error = nullptr) override;
		virtual void get_recognized_extensions(Vector<String> * out) const override;
		virtual TYPE get_resource_type(String const & path) const override;
	};
}

namespace Pnu
{
	// mono instance
	class PNU_MOD_API MonoInstance : public ScriptInstance
	{
		DEFINE_CLASS(MonoInstance, ScriptInstance);

		Object * m_owner{};
		Ref<Script> m_script{};
		ScriptLanguage * m_language{};

	public:
		MonoInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~MonoInstance() override;

		virtual bool get_constants(HashMap<String, OBJ> * out) const override;
		virtual bool get_properties(HashMap<String, PROPERTY> * out) const override;
		virtual bool get_functions(HashMap<String, FUNCTION> * out) const override;

		virtual void notification(i32 notification) override;

		virtual bool is_placeholder() const override { return false; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }
	};
}

#endif // !_PNU_MONO_SCRIPT_HPP_
