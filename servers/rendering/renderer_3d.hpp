#ifndef _PNU_RENDERER_SCENE_RENDERER_HPP_
#define _PNU_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class PNU_API Renderer3D : public Object
	{
		DEFINE_CLASS(Renderer3D, Object);

		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		Renderer3D(RenderingDevice * device, RendererStorage * storage);

		virtual ~Renderer3D() override;

	public:
		struct SceneState
		{
			struct UBO
			{
				Mat4 projection_matrix{};
				Mat4 inv_projection_matrix{};

				Mat4 camera_matrix{};
				Mat4 inv_camera_matrix{};

				Vec2 viewport_size{};
				Vec2 screen_pixel_size{};

				f32 znear{};
				f32 zfar{};
				
				f32 time{};

				// etc...
			}
			ubo{};
		}
		m_scene_state{};

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_RENDERER_SCENE_RENDERER_HPP_
