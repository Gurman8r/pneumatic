#ifndef _PNU_MISSING_NODE_HPP_
#define _PNU_MISSING_NODE_HPP_

#include <scene/main/node.hpp>

namespace Pnu
{
	class PNU_API MissingNode : public Node
	{
		DEFINE_CLASS(MissingNode, Node);

	public:
		MissingNode();
		virtual ~MissingNode() override;
	};
}

#endif // !_PNU_MISSING_NODE_HPP_
