#ifndef _PN_BEHAVIOR_HPP_
#define _PN_BEHAVIOR_HPP_

#include <scene/component/component.hpp>

namespace pn
{
	class PN_API Behavior : public Component
	{
		DEFINE_CLASS(Behavior, Component);

		bool m_enabled{ true };

	public:
		Behavior();
		virtual ~Behavior() override;

		NODISCARD bool is_enabled() const noexcept { return m_enabled; }
		void set_enabled(bool enabled);
		virtual void on_enable();
		virtual void on_disable();
	};
}

#endif // !_PN_BEHAVIOR_HPP_
