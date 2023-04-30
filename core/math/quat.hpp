#ifndef _PNU_QUAT_HPP_
#define _PNU_QUAT_HPP_

#include <core/math/matrix.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Quat
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= Quat;
		using storage_type		= Vec4;
		using value_type		= storage_type::value_type;
		using size_type			= storage_type::size_type;
		using difference_type	= storage_type::difference_type;
		using pointer			= storage_type::pointer;
		using reference			= storage_type::reference;
		using const_pointer		= storage_type::const_pointer;
		using const_reference	= storage_type::const_reference;
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		storage_type m_data{ 1, 0, 0, 0 };

		Quat(self_type const &) = default;
		Quat(self_type &&) noexcept = default;
		self_type & operator=(self_type const &) = default;
		self_type & operator=(self_type &&) noexcept = default;
		NODISCARD operator pointer() noexcept { return m_data; }
		NODISCARD operator const_pointer() const noexcept { return m_data; }
		NODISCARD operator storage_type & () & noexcept { return m_data; }
		NODISCARD operator storage_type const & () const & noexcept { return m_data; }
		NODISCARD operator storage_type && () && noexcept { return std::move(m_data); }
		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		Quat(glm::quat const & q) noexcept
		{
			::new (m_data) glm::quat{ q };
		}

		Quat(f32 const w = 1, f32 const x = 0, f32 const y = 0, f32 const z = 0) noexcept
		{
			::new (m_data) glm::quat{ w, x, y, z };
		}

		Quat(f32 const real, Vec3 const & complex) noexcept
		{
			::new (m_data) glm::quat{ real, (glm::vec3 const &)complex };
		}

		Quat(Vec3 const & u, Vec3 const & v) noexcept
		{
			::new(m_data) glm::quat{ (glm::vec3 const &)u, (glm::vec3 const &)v };
		}

		Quat(Vec3 const & xyz) noexcept : Quat{}
		{
			rotate(xyz);
		}

		Quat(Mat3 const & m9) noexcept
		{
			::new (m_data) glm::quat{ (glm::mat3 const &)m9 };
		}

		Quat(Mat4 const & m16) noexcept
		{
			::new (m_data) glm::quat{ (glm::mat4 const &)m16 };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD Vec3 const & complex() const noexcept { return *(Vec3 const *)&m_data[1]; }

		NODISCARD f32 const & real() const noexcept { return m_data[0]; }

		NODISCARD Vec3 & complex() noexcept { return *(Vec3 *)&m_data[1]; }
		
		NODISCARD f32 & real() noexcept { return m_data[0]; }

		NODISCARD f32 angle() const noexcept { return glm::angle((glm::quat const &)*this); }
		
		NODISCARD Vec3 axis() const noexcept { return bit_cast<Vec3>(glm::axis((glm::quat const &)*this)); }

		NODISCARD Vec3 euler() const noexcept { return { pitch(), yaw(), roll() }; }
		
		NODISCARD f32 pitch() const noexcept { return glm::pitch((glm::quat const &)*this); }
		
		NODISCARD f32 yaw() const noexcept { return glm::yaw((glm::quat const &)*this); }
		
		NODISCARD f32 roll() const noexcept { return glm::roll((glm::quat const &)*this); }

		NODISCARD operator Mat3() const noexcept { return bit_cast<Mat3>(glm::toMat3((glm::quat const &)*this)); }
		
		NODISCARD operator Mat4() const noexcept { return bit_cast<Mat4>(glm::toMat4((glm::quat const &)*this)); }

		NODISCARD operator Vec3() const noexcept { return { m_data[1], m_data[2], m_data[3] }; }

		NODISCARD operator Vec4() const noexcept { return { m_data[1], m_data[2], m_data[3], m_data[0] }; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD static Quat angle_axis(f32 const angle, Vec3 const & axis) noexcept
		{
			return glm::angleAxis(angle, (glm::vec3 const &)axis);
		}

		NODISCARD static Quat look_at(Vec3 const & direction, Vec3 const & up = up_v<Vec3>) noexcept
		{
			return glm::quatLookAt((glm::vec3 const &)direction, (glm::vec3 const &)up);
		}

		NODISCARD static Quat rotate(Quat const & q, f32 const angle, Vec3 const & axis) noexcept
		{
			return glm::rotate((glm::quat const &)q, angle, (glm::vec3 const &)axis);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Quat & rotate(f32 const angle, Vec3 const & axis) noexcept { return (*this) = rotate(*this, angle, axis); }

		Quat & rotate_x(f32 const x) noexcept { return rotate(x, right_v<Vec3>); }

		Quat & rotate_y(f32 const y) noexcept { return rotate(y, up_v<Vec3>); }

		Quat & rotate_z(f32 const z) noexcept { return rotate(z, front_v<Vec3>); }

		Quat & rotate(f32 const x, f32 const y, f32 const z) noexcept { return rotate_x(x).rotate_y(y).rotate_z(z); }

		Quat & rotate(Vec3 const & xyz) noexcept { return rotate_x(xyz[0]).rotate_y(xyz[1]).rotate_z(xyz[2]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Quat & operator+=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) += ((glm::quat const &)b)), a; }

	inline Quat & operator-=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) -= ((glm::quat const &)b)), a; }

	inline Quat & operator*=(Quat & a, Quat const & b) noexcept { return (((glm::quat &)a) *= ((glm::quat const &)b)), a; }

	inline Quat & operator*=(Quat & a, f32 const b) noexcept { return (((glm::quat &)a) *= b), a; }

	inline Quat & operator/=(Quat & a, f32 const b) noexcept { return (((glm::quat &)a) /= b), a; }

	NODISCARD inline Quat operator+(Quat const & a) noexcept { return a; }

	NODISCARD inline Quat operator-(Quat const & a) noexcept { return { -a[0], -a[1], -a[2], -a[3] }; }

	NODISCARD inline Quat operator+(Quat const & a, Quat const & b) noexcept { return Quat{ a } += b; }

	NODISCARD inline Quat operator-(Quat const & a, Quat const & b) noexcept { return Quat{ a } -= b; }

	NODISCARD inline Quat operator*(Quat const & a, Quat const & b) noexcept { return Quat{ a } *= b; }

	NODISCARD inline Vec3 operator*(Quat const & a, Vec3 const & b) noexcept { return bit_cast<Vec3>((glm::quat const &)a * (glm::vec3 const &)b); }

	NODISCARD inline Vec3 operator*(Vec3 const & a, Quat const & b) noexcept { return bit_cast<Vec3>((glm::vec3 const &)a * (glm::quat const &)b); }

	NODISCARD inline Vec4 operator*(Quat const & a, Vec4 const & b) noexcept { return bit_cast<Vec4>((glm::quat const &)a * (glm::vec4 const &)b); }

	NODISCARD inline Vec4 operator*(Vec4 const & a, Quat const & b) noexcept { return bit_cast<Vec4>((glm::vec4 const &)a * (glm::quat const &)b); }

	NODISCARD inline bool operator==(Quat const & a, Quat const & b) noexcept { return a.m_data == b.m_data; }

	NODISCARD inline bool operator!=(Quat const & a, Quat const & b) noexcept { return a.m_data != b.m_data; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_QUAT_HPP_
