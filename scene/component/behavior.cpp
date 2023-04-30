#include <scene/component/behavior.hpp>

namespace Pnu
{
	EMBED_CLASS(Behavior, t) {}

	Behavior::Behavior()
	{
	}

	Behavior::~Behavior()
	{
	}

	void Behavior::set_enabled(bool enabled)
	{
		if (m_enabled != enabled) {
			m_enabled = enabled;
			std::invoke(m_enabled ? &Behavior::on_enable : &Behavior::on_disable, this);
		}
	}

	void Behavior::on_enable()
	{
	}

	void Behavior::on_disable()
	{
	}
}