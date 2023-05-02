#include <core/object/eval.hpp>
#include <core/object/class.hpp>

namespace Pnu
{
	ObjectRef evaluate(String text)
	{
		if (text.trim([](char c) { return c == ' ' || c == '\'' || c == '\"'; }).empty()) { return nullptr; }
		else if (auto const o{ util::to_i64(text) }) { return IntRef(*o); }
		else if (auto const o{ util::to_f64(text) }) { return FloatRef(*o); }
		else if (auto const o{ util::to_bool(text) }) { return BoolRef(*o); }
		else { return StringRef(text); }
	}
}