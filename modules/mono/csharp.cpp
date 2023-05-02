#include <modules/mono/csharp.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpLanguage, t) {}

	CSharpLanguage * CSharpLanguage::__singleton{};
	CSharpLanguage::CSharpLanguage() { SINGLETON_CTOR(); }
	CSharpLanguage::~CSharpLanguage() { SINGLETON_DTOR(); }

	void CSharpLanguage::initialize()
	{
		if (!(m_mono = get_mono())) { m_mono = memnew(Mono); }

		ASSERT(m_mono->initialize());
	}

	void CSharpLanguage::finalize()
	{
		ASSERT(m_mono->finalize());

		if (m_mono) { memdelete(m_mono); m_mono = nullptr; }
	}

	void CSharpLanguage::reload_assemblies(bool soft_reload)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpScript, t) {}

	CSharpScript::CSharpScript() : m_language{ get_csharp() }
	{
	}

	CSharpScript::CSharpScript(String const & path)
	{
	}

	CSharpScript::~CSharpScript()
	{
	}

	Ref<ScriptInstance> CSharpScript::instance_create(Object * self)
	{
		return nullptr;
	}

	void CSharpScript::get_field_names(Vector<String> * out) const
	{
		if (!out) { return; }
	}

	void CSharpScript::get_method_names(Vector<String> * out) const
	{
		if (!out) { return; }
	}

	void CSharpScript::get_property_names(Vector<String> * out) const
	{
		if (!out) { return; }
	}

	Error_ CSharpScript::reload(bool keep_state)
	{
		return Error_OK;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpInstance, t) {}

	CSharpInstance::CSharpInstance(Ref<Script> script, Object * owner)
		: m_script	{ VALIDATE(script) }
		, m_owner	{ owner }
		, m_language{ script->get_language() }
	{
	}

	CSharpInstance::~CSharpInstance()
	{
	}

	void CSharpInstance::notification(i32 notification)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CSharpScriptFormatLoader, t) {}

	RES CSharpScriptFormatLoader::load(String const & path, Error_ * error)
	{
		Ref<CSharpScript> scr{ memnew(CSharpScript) };
		scr->set_path(path);
		scr->reload();
		if (error) { *error = Error_OK; }
		return scr;
	}

	void CSharpScriptFormatLoader::get_recognized_extensions(Vector<String> * out) const
	{
		if (!out) { return; }
		out->push_back(".cs");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}