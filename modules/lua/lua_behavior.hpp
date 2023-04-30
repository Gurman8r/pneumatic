#ifndef _PNU_LUA_BEHAVIOR_HPP_
#define _PNU_LUA_BEHAVIOR_HPP_

#include <scene/component/behavior.hpp>

namespace Pnu
{
	// lua behavior
	class PNU_MOD_API LuaBehavior : public Behavior
	{
		DEFINE_CLASS(LuaBehavior, Behavior);

	public:
		LuaBehavior();
		virtual ~LuaBehavior() override;
	};
}

#endif // !_PNU_LUA_BEHAVIOR_HPP_
