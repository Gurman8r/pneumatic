#ifndef _PNU_INPUT_EVENT_HPP_
#define _PNU_INPUT_EVENT_HPP_

// PLACEHOLDER

#include <core/io/resource.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input event
	class PNU_API InputEvent : public Resource
	{
		DEFINE_CLASS(InputEvent, Resource);

		friend class Input;

	public:
		InputEvent() noexcept;

		virtual ~InputEvent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_INPUT_EVENT_HPP_
