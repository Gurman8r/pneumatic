#include <drivers/register_driver_types.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

#if SYSTEM_WINDOWS
#include <drivers/windows/file_access_windows.hpp>
#endif

namespace pn
{
	void register_core_driver_types()
	{
#if SYSTEM_WINDOWS
		REGISTER_CLASS(FileAccessWindows);
		FileAccessWindows::initialize();
#endif
	}

	void unregister_core_driver_types()
	{
#if SYSTEM_WINDOWS
		FileAccessWindows::finalize();
#endif
	}

	void register_driver_types()
	{
		REGISTER_CLASS(RENDERING_DEVICE_DEFAULT);
	}

	void unregister_driver_types()
	{
	}
}