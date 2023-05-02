#include <core/extension/extension_api_dump.hpp>

namespace Pnu
{
	DictRef ExtensionAPIDump::generate_extension_api()
	{
		DictRef api_dump{};
		return api_dump;
	}

	void ExtensionAPIDump::generate_extension_json_file(String const & path)
	{
		DictRef api_dump{ generate_extension_api() };
	}
}