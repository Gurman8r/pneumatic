#ifndef _PN_INPUT_EVENT_HPP_
#define _PN_INPUT_EVENT_HPP_

// PLACEHOLDER

#include <core/io/resource.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// input event
	class PN_API InputEvent : public Resource
	{
		DEFINE_CLASS(InputEvent, Resource);

		friend class Input;

	public:
		InputEvent() noexcept;

		virtual ~InputEvent() noexcept override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_INPUT_EVENT_HPP_
