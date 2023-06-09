#ifndef _PNU_JSON_HPP_
#define _PNU_JSON_HPP_

#include <core/object/builtins/dict_object.hpp>

#include <nlohmann/json.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using Json = nlohmann::basic_json
	<
		std::map,
		std::vector,
		std::string,
		bool, i64, u64, f64,
		std::allocator,
		nlohmann::adl_serializer,
		std::vector<u8>
	>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API JsonUtility final
	{
	public:
		template <class T
		> static DictRef to_json(T const & value)
		{
			return {};
		}

		template <class T
		> static Optional<T> from_json(DictRef const & json)
		{
			return nullopt;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_JSON_HPP_
