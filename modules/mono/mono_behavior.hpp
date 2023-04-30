#ifndef _PN_MONO_BEHAVIOR_HPP_
#define _PN_MONO_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace pn
{
	// mono behavior
	class PN_MOD_API MonoBehavior : public Behavior
	{
		DEFINE_CLASS(MonoBehavior, Behavior);

	public:
		MonoBehavior();
		virtual ~MonoBehavior() override;
	};
}

#endif // !_PN_MONO_BEHAVIOR_HPP_
