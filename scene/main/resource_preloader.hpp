#ifndef _PNU_RESOURCE_PRELOADER_HPP_
#define _PNU_RESOURCE_PRELOADER_HPP_

#include <scene/main/node.hpp>

namespace Pnu
{
	class PNU_API ResourcePreloader : public Node
	{
		DEFINE_CLASS(ResourcePreloader, Node);

	public:
		ResourcePreloader();
		virtual ~ResourcePreloader() override;
	};
}

#endif // !_PNU_RESOURCE_PRELOADER_HPP_
