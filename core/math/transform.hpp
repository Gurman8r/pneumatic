#ifndef _PN_TRANSFORM_HPP_
#define _PN_TRANSFORM_HPP_

#include <core/math/quat.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Mat4 perspective(f32 fov, f32 aspect, f32 znear, f32 zfar) noexcept
	{
		return bit_cast<Mat4>(glm::perspective(fov, aspect, znear, zfar));
	}

	inline Mat4 frustum(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar) noexcept
	{
		return bit_cast<Mat4>(glm::frustum(left, right, bottom, top, znear, zfar));
	}

	inline Mat4 orthographic(f32 left, f32 right, f32 top, f32 bottom, f32 znear, f32 zfar) noexcept
	{
		return bit_cast<Mat4>(glm::ortho(left, right, bottom, top, znear, zfar));
	}

	inline Mat4 translate(Mat4 const & m, Vec3 const & v)
	{
		return bit_cast<Mat4>(glm::translate((glm::mat4 const &)m, (glm::vec3 const &)v));
	}

	inline Mat4 rotate(Mat4 const & m, f32 const angle, Vec3 const & axis) noexcept
	{
		return bit_cast<Mat4>(glm::rotate((glm::mat4 const &)m, angle, (glm::vec3 const &)axis));
	}

	inline Mat4 scale(Mat4 const & m, Vec3 const & v) noexcept
	{
		return bit_cast<Mat4>(glm::scale((glm::mat4 const &)m, (glm::vec3 const &)v));
	}

	inline Mat4 look_at(Vec3 const & eye, Vec3 const & target, Vec3 const & up = up_v<Vec3>) noexcept
	{
		return bit_cast<Mat4>(glm::lookAt((glm::vec3 const &)eye, (glm::vec3 const &)target, (glm::vec3 const &)up));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Transform
	{
	private:
		mutable Mat4 m_matrix;
		mutable bool m_changed;

		Vec3 m_position;
		Quat m_rotation;
		Vec3 m_scale;

	public:
		Transform() noexcept
			: m_matrix	{ identity_v<Mat4> }
			, m_changed	{ false }
			, m_position{}
			, m_rotation{}
			, m_scale	{ one_v<Vec3> }
		{
		}

		Transform(Vec3 const & position, Quat const & rotation = {}, Vec3 const & scale = one_v<Vec3>) noexcept
			: m_matrix	{ identity_v<Mat4> }
			, m_changed	{ true }
			, m_position{ position }
			, m_rotation{ rotation }
			, m_scale	{ scale }
		{
		}

		Transform(Transform const & value)
			: m_matrix	{ value.m_matrix }
			, m_changed	{ value.m_changed }
			, m_position{ value.m_position }
			, m_rotation{ value.m_rotation }
			, m_scale	{ value.m_scale }
		{
		}

		Transform(Transform && value) noexcept
			: m_matrix	{}
			, m_changed	{}
			, m_position{}
			, m_rotation{}
			, m_scale	{}
		{
			swap(std::move(value));
		}

		Transform & operator=(Transform const & value) { Transform temp{ value }; return swap(temp); }

		Transform & operator=(Transform && value) noexcept { return swap(std::move(value)); }

		Transform & swap(Transform & value) noexcept
		{
			if (this != std::addressof(value)) {
				util::swap(m_matrix, value.m_matrix);
				util::swap(m_changed, value.m_changed);
				util::swap(m_position, value.m_position);
				util::swap(m_rotation, value.m_rotation);
				util::swap(m_scale, value.m_scale);
			}
			return (*this);
		}

	public:
		/* POSITION */

		NODISCARD Vec3 const & get_position() const noexcept
		{
			return m_position;
		}

		Transform & set_position(Vec3 const & xyz) noexcept
		{
			if (m_position != xyz) {
				m_position = xyz;
				m_changed = true;
			}
			return (*this);
		}

		Transform & set_position(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (m_position[0] != x || m_position[1] != y || m_position[2] != z) {
				m_position[0] = x; m_position[1] = y; m_position[2] = z;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate(Vec3 const & xyz) noexcept
		{
			if (xyz != Vec3{}) {
				m_position += xyz;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (x != 0.f || y != 0.f || z != 0.f) {
				m_position[0] += x; m_position[1] += y; m_position[2] += z;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate_x(f32 const x) noexcept
		{
			if (x != 0.f) {
				m_position[0] += x;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate_y(f32 const y) noexcept
		{
			if (y != 0.f) {
				m_position[1] += y;
				m_changed = true;
			}
			return (*this);
		}

		Transform & translate_z(f32 const z) noexcept
		{
			if (z != 0.f) {
				m_position[2] += z;
				m_changed = true;
			}
			return (*this);
		}

	public:
		/* ROTATION */

		NODISCARD Quat const & get_rotation() const noexcept
		{
			return m_rotation;
		}

		Transform & set_rotation(Quat const & q) noexcept
		{
			if (m_rotation != q) {
				m_rotation = q;
				m_changed = true;
			}
			return (*this);
		}

		Transform & set_rotation(f32 const angle, Vec3 const & axis) noexcept
		{
			return set_rotation(Quat::angle_axis(angle, axis));
		}

		Transform & set_rotation(f32 const x, f32 const y, f32 const z) noexcept
		{
			return set_rotation(Quat{ Vec3{ x, y, z } });
		}

		Transform & set_rotation(Vec3 const & xyz) noexcept
		{
			return set_rotation(Quat{ xyz });
		}

		Transform & rotate(Quat const & q) noexcept
		{
			if (q != Quat{}) {
				m_rotation *= q;
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate(f32 const angle, Vec3 const & axis) noexcept
		{
			if (angle != 0.f && axis != Vec3{}) {
				m_rotation.rotate(angle, axis);
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate(Vec3 const & xyz) noexcept
		{
			if (xyz != Vec3{}) {
				m_rotation.rotate(xyz);
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (x != 0.f && y != 0.f && z != 0.f) {
				m_rotation.rotate(x, y, z);
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate_x(f32 const x) noexcept
		{
			if (x != 0.f) {
				m_rotation.rotate_x(x);
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate_y(f32 const y) noexcept
		{
			if (y != 0.f) {
				m_rotation.rotate_y(y);
				m_changed = true;
			}
			return (*this);
		}

		Transform & rotate_z(f32 const z) noexcept
		{
			if (z != 0.f) {
				m_rotation.rotate_z(z);
				m_changed = true;
			}
			return (*this);
		}

	public:
		/* SCALE */

		NODISCARD Vec3 const & get_scale() const noexcept
		{
			return m_scale;
		}

		Transform & set_scale(Vec3 const & xyz) noexcept
		{
			if (m_scale != xyz) {
				m_scale = xyz;
				m_changed = true;
			}
			return (*this);
		}

		Transform & set_scale(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (m_scale[0] != x || m_scale[1] != y || m_scale[2] != z) {
				m_scale[0] = x; m_scale[1] = y; m_scale[2] = z;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale(Vec3 const & xyz) noexcept
		{
			if (xyz != Vec3{}) {
				m_scale += xyz;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale(f32 const x, f32 const y, f32 const z) noexcept
		{
			if (x != 0.f || y != 0.f || z != 0.f) {
				m_scale[0] += x; m_scale[1] += y; m_scale[2] += z;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale_x(f32 const x) noexcept
		{
			if (x != 0.f) {
				m_scale[0] += x;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale_y(f32 const y) noexcept
		{
			if (y != 0.f) {
				m_scale[1] += y;
				m_changed = true;
			}
			return (*this);
		}

		Transform & scale_z(f32 const z) noexcept
		{
			if (z != 0.f) {
				m_scale[2] += z;
				m_changed = true;
			}
			return (*this);
		}

	public:
		/* MATRIX */

		NODISCARD Mat4 get_translation() const noexcept
		{
			return pn::translate(identity_v<Mat4>, m_position);
		}

		NODISCARD Mat4 get_orientation() const noexcept
		{
			return m_rotation;
		}

		NODISCARD Mat4 get_scaling() const noexcept
		{
			return pn::scale(identity_v<Mat4>, m_scale);
		}

		NODISCARD Mat4 const & get_matrix() const noexcept
		{
			if (m_changed) {
				m_matrix = get_scaling() * get_orientation() * get_translation();
				m_changed = false;
			}
			return m_matrix;
		}

		NODISCARD operator Mat4 const & () const noexcept
		{
			return get_matrix();
		}

		NODISCARD operator f32 const * () const noexcept
		{
			return get_matrix();
		}

		NODISCARD Vec3 front() const noexcept { return normalize((Vec3)(get_matrix().col(2))); }

		NODISCARD Vec3 right() const noexcept { return normalize(cross(front(), up_v<Vec3>)); }

		NODISCARD Vec3 up() const noexcept { return normalize(cross(right(), front())); }

		NODISCARD Vec3 back() const { return -front(); }

		NODISCARD Vec3 left() const { return -right(); }

		NODISCARD Vec3 down() const { return -up(); }

		Transform & look_at(Vec3 const & target, Vec3 const & up = up_v<Vec3>) noexcept
		{
			m_rotation = Quat::look_at(normalize(m_position - target), up);
			m_changed = true;
			return (*this);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_TRANSFORM_HPP_
