#include <core/extension/extension_api_dump.hpp>

namespace Pnu
{
	DICT ExtensionAPIDump::generate_extension_api()
	{
		DICT api_dump{};
		return api_dump;
	}

	void ExtensionAPIDump::generate_extension_json_file(String const & path)
	{
		DICT api_dump{ generate_extension_api() };
	}
}