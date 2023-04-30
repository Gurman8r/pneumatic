#ifndef _PN_LUA_BEHAVIOR_HPP_
#define _PN_LUA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace pn
{
	// lua behavior
	class PN_MOD_API LuaBehavior : public Behavior
	{
		DEFINE_CLASS(LuaBehavior, Behavior);

	public:
		LuaBehavior();
		virtual ~LuaBehavior() override;
	};
}

#endif // !_PN_LUA_BEHAVIOR_HPP_
