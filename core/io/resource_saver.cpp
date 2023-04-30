#include <core/io/resource_saver.hpp>

namespace Pnu
{
	ResourceSaver * ResourceSaver::__singleton{};

	ResourceSaver::ResourceSaver() { SINGLETON_CTOR(); }

	ResourceSaver::~ResourceSaver() { SINGLETON_DTOR(); }

	Error_ ResourceSaver::save(RES const & res, String const & path)
	{
		return Error_OK;
	}
}