#ifndef _PN_DIR_ACCESS_HPP_
#define _PN_DIR_ACCESS_HPP_

#include <core/object/class.hpp>

namespace pn
{
	// dir access
	class PN_API DirAccess : public Object
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

#endif // !_PN_DIR_ACCESS_HPP_
