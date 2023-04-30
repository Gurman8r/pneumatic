#ifndef _PN_INPUT_MAP_HPP_
#define _PN_INPUT_MAP_HPP_

// PLACEHOLDER

#include <core/input/input_event.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input map
	class PN_API InputMap : public Object
	{
		DEFINE_CLASS(InputMap, Object);

		friend class Input;

	public:
		InputMap() noexcept;

		virtual ~InputMap() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_INPUT_MAP_HPP_
