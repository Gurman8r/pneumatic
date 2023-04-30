#ifndef _PNU_INSTANCE_PLACEHOLDER_HPP_
#define _PNU_INSTANCE_PLACEHOLDER_HPP_

#include <scene/main/node.hpp>

namespace Pnu
{
	class PNU_API InstancePlaceholder : public Node
	{
		DEFINE_CLASS(InstancePlaceholder, Node);

	public:
		InstancePlaceholder();
		virtual ~InstancePlaceholder();
	};
}

#endif // !_PNU_INSTANCE_PLACEHOLDER_HPP_
