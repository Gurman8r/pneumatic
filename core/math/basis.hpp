#ifndef _PNU_BASIS_HPP_
#define _PNU_BASIS_HPP_

#include <core/math/quat.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Basis
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using self_type			= Basis;
		using storage_type		= Mat3;
		using value_type		= storage_type::value_type;
		using size_type			= storage_type::size_type;
		using difference_type	= storage_type::difference_type;
		using pointer			= storage_type::pointer;
		using reference			= storage_type::reference;
		using const_pointer		= storage_type::const_pointer;
		using const_reference	= storage_type::const_reference;
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		storage_type m_data{};

		constexpr Basis(self_type const &) = default;
		constexpr Basis(self_type &&) noexcept = default;
		constexpr self_type & operator=(self_type const &) = default;
		constexpr self_type & operator=(self_type &&) noexcept = default;
		constexpr operator pointer() noexcept { return m_data; }
		constexpr operator const_pointer() const noexcept { return m_data; }
		constexpr operator storage_type & () & noexcept { return m_data; }
		constexpr operator storage_type const & () const & noexcept { return m_data; }
		constexpr operator storage_type && () && noexcept { return std::move(m_data); }
		constexpr auto begin() noexcept -> iterator { return m_data.begin(); }
		constexpr auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		constexpr auto end() noexcept -> iterator { return m_data.end(); }
		constexpr auto end() const noexcept -> const_iterator { return m_data.end(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Basis(Mat3 const & m = identity_v<Mat3>) noexcept
		{
			::new (m_data) glm::mat3{ (glm::mat3 const &)m };
		}

		Basis(f32 const scalar) noexcept
		{
			::new (m_data) glm::mat3{ scalar };
		}

		template <class X1, class Y1, class Z1, class X2, class Y2, class Z2, class X3, class Y3, class Z3
		> Basis(X1 const x1, Y1 const y1, Z1 const z1, X2 const x2, Y2 const y2, Z2 const z2, X3 const x3, Y3 const y3, Z3 const z3) noexcept
		{
			::new (m_data) glm::mat3{ x1, y1, z1, x2, y2, z2, x3, y3, z3 };
		}

		template <class V1, class V2, class V3
		> Basis(TVec3<V1> const & v1, TVec3<V2> const & v2, TVec3<V3> const & v3) noexcept
		{
			::new (m_data) glm::mat3{ (glm::vec3 const &)v1, (glm::vec3 const &)v2, (glm::vec3 const &)v3 };
		}

		Basis(Quat const & q) {}

		Basis(Quat const & q, Vec3 const & scale) {}

		Basis(f32 const angle, Vec3 const & axis) {}

		Basis(Vec3 const & euler_angles, Vec3 const & scale) {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		operator Quat() const { return Quat{ m_data }; }
		
		Quat get_quat() const { return Quat{ m_data }; }

		Vec3 get_rotation_euler() const { return {}; }
		
		Vec3 get_rotation_axis() const { return {}; }
		
		f32 get_rotation_angle() const { return {}; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Vec3 get_scale() const { return {}; }
		
		Vec3 get_scale_local() const { return {}; }

		Basis & scale(Vec3 const & amount) { return (*this); }

		Basis scaled(Vec3 const & amount) const { return Basis{ *this }.scale(amount); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool operator==(Basis const & a, Basis const & b) noexcept
	{
		return (std::addressof(a) == std::addressof(b)) || a.m_data == b.m_data;
	}
	
	bool operator!=(Basis const & a, Basis const & b) noexcept
	{
		return !(a == b);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_BASIS_HPP_
