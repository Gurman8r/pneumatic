#ifndef _PNU_SCRIPT_HPP_
#define _PNU_SCRIPT_HPP_

#include <core/io/resource.hpp>

// script server
namespace Pnu
{
	class Script;
	class ScriptInstance;
	class ScriptLanguage;
	class PlaceholderScriptInstance;

	class PNU_API ScriptServer : public Object
	{
		DEFINE_CLASS(ScriptServer, Object);

		static ScriptServer * __singleton;

		Vector<ScriptLanguage *> m_languages{};
		bool m_scripting_enabled{ true };
		bool m_reload_scripts_on_save{};
		bool m_languages_finalized{};

	public:
		ScriptServer() noexcept { SINGLETON_CTOR(); }
		virtual ~ScriptServer() noexcept override { SINGLETON_DTOR(); }
		FORCE_INLINE static ScriptServer * get_singleton() noexcept { return __singleton; }

		NODISCARD bool is_scripting_enabled() const noexcept { return m_scripting_enabled; }
		void set_scripting_enabled(bool enabled);

		NODISCARD size_t get_language_count() const noexcept { return m_languages.size(); }
		NODISCARD ScriptLanguage * get_language(size_t index) noexcept { return m_languages[index]; }

		Error_ register_language(ScriptLanguage * language);
		Error_ unregister_language(ScriptLanguage const * language);

		void initialize_languages();
		void finalize_languages();
		NODISCARD bool are_languages_finalized() const noexcept { return m_languages_finalized; }
	};

	SINGLETON_WRAPPER(ScriptServer, get_script_server);
}

// script language
namespace Pnu
{
	class PNU_API ScriptLanguage : public Object
	{
		DEFINE_CLASS(ScriptLanguage, Object);

	public:
		ScriptLanguage();
		virtual ~ScriptLanguage() override;

		NODISCARD virtual String get_name() const = 0;

		virtual void initialize() = 0;
		virtual void finalize() = 0;
	};
}

// script
namespace Pnu
{
	class PNU_API Script : public Resource
	{
		DEFINE_CLASS(Script, Resource);

	public:
		Script();
		explicit Script(String const & path);
		virtual ~Script() override;

		virtual Ref<ScriptInstance> instance_create(Object * self) = 0;

		virtual void get_field_names(Vector<String> * out) const = 0;
		virtual void get_method_names(Vector<String> * out) const = 0;
		virtual void get_property_names(Vector<String> * out) const = 0;
		
		virtual bool has_source_code() const = 0;
		virtual String get_source_code() const = 0;
		virtual void set_source_code(String const & code) = 0;
		virtual Error_ reload(bool keep_state = false) = 0;
		
		virtual ScriptLanguage * get_language() const = 0;
	};
}

// script instance
namespace Pnu
{
	class PNU_API ScriptInstance : public Object
	{
		DEFINE_CLASS(ScriptInstance, Object);

	public:
		virtual ~ScriptInstance();

		virtual bool get_constants(HashMap<String, ObjectRef> * out) const = 0;
		virtual bool get_properties(HashMap<String, PropertyRef> * out) const = 0;
		virtual bool get_functions(HashMap<String, FunctionRef> * out) const = 0;

		virtual void notification(i32 notification) = 0;

		virtual bool is_placeholder() const { return false; }
		virtual Ref<Script> get_script() const = 0;
		virtual ScriptLanguage * get_language() = 0;
		virtual Object * get_owner() = 0;
	};
}

// placeholder script instance
namespace Pnu
{
	class PNU_API PlaceholderScriptInstance : public ScriptInstance
	{
		DEFINE_CLASS(PlaceholderScriptInstance, ScriptInstance);

		Object *			m_owner{};
		Ref<Script>			m_script{};
		ScriptLanguage *	m_language{};

	public:
		PlaceholderScriptInstance(ScriptLanguage * language, Ref<Script> script, Object * owner);
		virtual ~PlaceholderScriptInstance() override;

		virtual bool get_constants(HashMap<String, ObjectRef> * out) const;
		virtual bool get_properties(HashMap<String, PropertyRef> * out) const;
		virtual bool get_functions(HashMap<String, FunctionRef> * out) const;

		virtual void notification(i32 notification) override {}
		
		virtual bool is_placeholder() const override { return true; }
		virtual Ref<Script> get_script() const override { return m_script; }
		virtual ScriptLanguage * get_language() { return m_language; }
		virtual Object * get_owner() override { return m_owner; }
	};
}

#endif // !_PNU_SCRIPT_HPP_
