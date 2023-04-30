#ifndef _PN_DIR_ACCESS_WINDOWS_HPP_
#define _PN_DIR_ACCESS_WINDOWS_HPP_

#include <core/io/dir_access.hpp>

namespace pn
{
	// directory
	class PN_API DirAccessWindows : public DirAccess
	{
		DEFINE_CLASS(DirAccessWindows, DirAccess);

	public:
		DirAccessWindows();
		virtual ~DirAccessWindows() override;

		static void initialize();
		static void finalize();
	};
}

#endif // !_PN_DIR_ACCESS_WINDOWS_HPP_
