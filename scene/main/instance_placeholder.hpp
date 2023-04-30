#ifndef _PN_INSTANCE_PLACEHOLDER_HPP_
#define _PN_INSTANCE_PLACEHOLDER_HPP_

#include <scene/main/node.hpp>

namespace pn
{
	class PN_API InstancePlaceholder : public Node
	{
		DEFINE_CLASS(InstancePlaceholder, Node);

	public:
		InstancePlaceholder();
		virtual ~InstancePlaceholder();
	};
}

#endif // !_PN_INSTANCE_PLACEHOLDER_HPP_
