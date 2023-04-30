#ifndef _PN_VIEWPORT_HPP_
#define _PN_VIEWPORT_HPP_

#include <scene/main/node.hpp>
#include <scene/resources/texture.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport texture
	class ViewportTexture : public Texture2D
	{
		DEFINE_CLASS(ViewportTexture, Texture2D);

	public:
		ViewportTexture();
		virtual ~ViewportTexture() override;
		virtual RID get_rid() const override;
		virtual i32 get_width() const override;
		virtual i32 get_height() const override;
		virtual Vec2i get_size() const override;
		virtual bool has_alpha() const override;
		virtual Ref<Image> get_data() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// viewport
	class PN_API Viewport : public Node
	{
		DEFINE_CLASS(Viewport, Node);

		friend class Window;

		RID m_viewport{};

	public:
		Viewport();
		virtual ~Viewport() override;

	protected:
		virtual Vec2i _get_size() const;
		virtual void _set_size(Vec2i const & size);

	public:
		Ref<ViewportTexture> get_texture() const;
		virtual DS::WindowID get_window_id() const = 0;
		virtual RID get_viewport_rid() const { return m_viewport; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_VIEWPORT_HPP_
