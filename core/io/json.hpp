#ifndef _PN_JSON_HPP_
#define _PN_JSON_HPP_

#include <core/object/builtins/dict_object.hpp>

#include <nlohmann/json.hpp>

namespace pn
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

	class PN_API JsonUtility final
	{
	public:
		template <class T
		> static DICT to_json(T const & value)
		{
			return {};
		}

		template <class T
		> static Optional<T> from_json(DICT const & json)
		{
			return nullopt;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_JSON_HPP_
