#include <main/performance.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Performance, t) {}

	Performance * Performance::__singleton{};

	Performance::Performance()
	{
		__singleton = this;
	}

	Performance::~Performance()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}