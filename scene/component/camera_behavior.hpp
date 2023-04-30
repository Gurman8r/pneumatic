#ifndef _PN_CAMERA_BEHAVIOR_HPP_
#define _PN_CAMERA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace pn
{
	class PN_API CameraBehavior : public Behavior
	{
		DEFINE_CLASS(CameraBehavior, Behavior);

	public:
		CameraBehavior();
		virtual ~CameraBehavior() override;
	};
}

#endif // !_PN_CAMERA_BEHAVIOR_HPP_
