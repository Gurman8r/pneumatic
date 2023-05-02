#ifndef _PNU_EXTENSION_API_DUMP_HPP_
#define _PNU_EXTENSION_API_DUMP_HPP_

// PLACEHOLDER

#include <core/extension/extension.hpp>

namespace Pnu
{
	class PNU_API ExtensionAPIDump final
	{
	public:
		static DictRef generate_extension_api();
		static void generate_extension_json_file(String const & path);
	};
}

#endif // !_PNU_EXTENSION_API_DUMP_HPP_
