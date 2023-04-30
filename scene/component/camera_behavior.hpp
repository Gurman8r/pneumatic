#ifndef _PNU_CAMERA_BEHAVIOR_HPP_
#define _PNU_CAMERA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace Pnu
{
	class PNU_API CameraBehavior : public Behavior
	{
		DEFINE_CLASS(CameraBehavior, Behavior);

	public:
		CameraBehavior();
		virtual ~CameraBehavior() override;
	};
}

#endif // !_PNU_CAMERA_BEHAVIOR_HPP_
