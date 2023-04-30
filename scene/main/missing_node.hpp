#ifndef _PN_MISSING_NODE_HPP_
#define _PN_MISSING_NODE_HPP_

#include <scene/main/node.hpp>

namespace pn
{
	class PN_API MissingNode : public Node
	{
		DEFINE_CLASS(MissingNode, Node);

	public:
		MissingNode();
		virtual ~MissingNode() override;
	};
}

#endif // !_PN_MISSING_NODE_HPP_
