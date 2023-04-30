#ifndef _PN_RESOURCE_PRELOADER_HPP_
#define _PN_RESOURCE_PRELOADER_HPP_

#include <scene/main/node.hpp>

namespace pn
{
	class PN_API ResourcePreloader : public Node
	{
		DEFINE_CLASS(ResourcePreloader, Node);

	public:
		ResourcePreloader();
		virtual ~ResourcePreloader() override;
	};
}

#endif // !_PN_RESOURCE_PRELOADER_HPP_
