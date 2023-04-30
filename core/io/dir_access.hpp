#ifndef _PNU_DIR_ACCESS_HPP_
#define _PNU_DIR_ACCESS_HPP_

#include <core/object/class.hpp>

namespace Pnu
{
	// dir access
	class PNU_API DirAccess : public Object
	{
		DEFINE_CLASS(DirAccess, Object);

	protected:
		using CreateFunc = Ref<DirAccess>(*)();
		static CreateFunc __create_func;
		static Ref<DirAccess> create();
		static Ref<DirAccess> create_for_path(String const & path);

	public:
		static Ref<DirAccess> open(String const & path, Error_ * error = nullptr);
	};
}

#endif // !_PNU_DIR_ACCESS_HPP_
