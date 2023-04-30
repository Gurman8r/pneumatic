#ifndef _PNU_INPUT_MAP_HPP_
#define _PNU_INPUT_MAP_HPP_

// PLACEHOLDER

#include <core/input/input_event.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input map
	class PNU_API InputMap : public Object
	{
		DEFINE_CLASS(InputMap, Object);

		friend class Input;

	public:
		InputMap() noexcept;

		virtual ~InputMap() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_INPUT_MAP_HPP_
