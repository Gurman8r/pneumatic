#include <core/object/builtins/cppfunction_object.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(CppFunctionObject, t)
	{
		t.tp_dictoffset = offsetof(CppFunctionObject, m_dict);

		t.tp_vectorcalloffset = offsetof(CppFunctionObject, m_vectorcall);

		t.tp_descr_get = (DescrGetFunc)[](ObjectRef self, ObjectRef obj, ObjectRef cls) -> ObjectRef
		{
			return !obj ? self : (ObjectRef)MethodRef({ self, obj });
		};

		t.tp_bind = BIND_CLASS(CppFunctionObject, t)
		{
			// manually add this first because it's used by CLASS_
			t.add_object("__name__", PropertyRef({
				CppFunctionRef({ [](CppFunctionObject const & self) -> String const & { return self->name; }, attr::is_method(t) }),
				CppFunctionRef({ [](CppFunctionObject & self, String const & value) { self->name = value; }, attr::is_method(t) }),
				}));

			return t
				.def_property("__text_signature__",
					[](CppFunctionObject const & self) { return self->signature; },
					[](CppFunctionObject & self, String const & value) { self->signature = value; })
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CppFunctionObject::~CppFunctionObject()
	{
		while (m_record) {
			FunctionRecord * const it{ m_record->next };
			memdelete(m_record);
			m_record = it;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void CppFunctionObject::initialize_generic(FunctionRecord * rec, std::type_info const * const * info_in, size_t argc_in, bool prepend)
	{
		ASSERT("BAD FunctionRef RECORD" && (rec && !rec->next));
		m_record = rec;

		// argument info
		rec->argument_count = (u16)argc_in;
		rec->args.reserve(argc_in);
		for (size_t i{}; i < argc_in; ++i)
		{
			rec->args.push_back(MAKE(ArgumentRecord, arg)
			{
				// TODO: generate better argument info
				arg.name = "arg"_s + util::to_string(i);
			});
		}

		// overload chaining
		if (CppFunctionRef::check_(rec->sibling))
		{
			if (CppFunctionObject & sibling{ *VALIDATE((CppFunctionObject *)rec->sibling) }
			; sibling.m_record->scope == rec->scope)
			{
				if (prepend)
				{
					rec->next = sibling.m_record;
					sibling.m_record = nullptr;
				}
				else
				{
					FunctionRecord * it{ sibling.m_record };
					while (it->next) { it = it->next; }
					it->next = rec;
					m_record = sibling.m_record;
					sibling.m_record = nullptr;
				}
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ObjectRef CppFunctionObject::cppfunction_vectorcall(ObjectRef callable, ObjectRef const * argv, size_t argc)
	{
		ASSERT(argc < MAX_ARGUMENTS);

		if (!CppFunctionRef::check_(callable)) { return nullptr; }

		ObjectRef parent{ 0 < argc ? argv[0] : nullptr };

		FunctionRecord const * overloads{ ((CppFunctionRef)callable)->get_function_record() }, * it{ overloads };

		for (; it; it = it->next)
		{
			bool const overloaded{ it && it->next };

			FunctionRecord const & record{ *it };

			FunctionCall call{ record, parent };

			size_t
				num_args	{ record.argument_count },
				num_to_copy	{ MIN(num_args, argc) },
				num_copied	{};

			// copy positional arguments
			for (; num_copied < num_to_copy; ++num_copied)
			{
				ArgumentRecord const * arg{};
				if (num_copied < record.args.size()) {
					arg = &record.args[num_copied];
				}
				call.args.push_back(argv[num_copied], arg && arg->convert);
			}

			// fill in missing arguments
			if (num_copied < num_args)
			{
				for (; num_copied < num_args; ++num_copied)
				{
					ArgumentRecord const & arg{ record.args[num_copied] };
					if (!arg.value) {
						break;
					}
					call.args.push_back(arg.value, arg.convert);
				}

				if (num_copied < num_args)
				{
					if (overloaded) {
						continue;
					}

					CRASH("NOT ENOUGH ARGUMENTS");
				}
			}

			// execute call
			if (ObjectRef result{ call() }; !call.try_next_overload)
			{
				return result;
			}
		}

		return nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}