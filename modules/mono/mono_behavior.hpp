#ifndef _PNU_MONO_BEHAVIOR_HPP_
#define _PNU_MONO_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace Pnu
{
	// mono behavior
	class PNU_MOD_API MonoBehavior : public Behavior
	{
		DEFINE_CLASS(MonoBehavior, Behavior);

	public:
		MonoBehavior();
		virtual ~MonoBehavior() override;
	};
}

#endif // !_PNU_MONO_BEHAVIOR_HPP_
