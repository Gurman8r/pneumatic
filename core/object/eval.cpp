#include <core/object/eval.hpp>
#include <core/object/class.hpp>

namespace pn
{
	OBJ evaluate(String text)
	{
		if (text.trim([](char c) { return c == ' ' || c == '\'' || c == '\"'; }).empty()) { return nullptr; }
		else if (auto const o{ util::to_i64(text) }) { return INT(*o); }
		else if (auto const o{ util::to_f64(text) }) { return FLT(*o); }
		else if (auto const o{ util::to_bool(text) }) { return BOOL_OBJ(*o); }
		else { return STR(text); }
	}
}