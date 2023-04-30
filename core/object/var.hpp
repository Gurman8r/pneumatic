#ifndef _PNU_VAR_HPP_
#define _PNU_VAR_HPP_

#include <core/io/resource.hpp>

#include <variant>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum VarType_ : size_t
	{
		VarType_Bool,
		VarType_I32, VarType_Vec2i, VarType_Vec3i, VarType_Vec4i, VarType_Mat2i, VarType_Mat3i, VarType_Mat4i,
		VarType_I64,
		VarType_U32, VarType_Vec2u, VarType_Vec3u, VarType_Vec4u, VarType_Mat2u, VarType_Mat3u, VarType_Mat4u,
		VarType_U64,
		VarType_F32, VarType_Vec2f, VarType_Vec3f, VarType_Vec4f, VarType_Mat2f, VarType_Mat3f, VarType_Mat4f,
		VarType_F64, VarType_Vec2d, VarType_Vec3d, VarType_Vec4d, VarType_Mat2d, VarType_Mat3d, VarType_Mat4d,
		VarType_RID,
		VarType_OBJ,
		VarType_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Var final
	{
	public:
		using data_types = typename mpl::type_list
		<
			bool,
			i32, Vec2i, Vec3i, Vec4i, Mat2i, Mat3i, Mat4i,
			i64,
			u32, Vec2u, Vec3u, Vec4u, Mat2u, Mat3u, Mat4u,
			u64,
			f32, Vec2f, Vec3f, Vec4f, Mat2f, Mat3f, Mat4f,
			f64, Vec2d, Vec3d, Vec4d, Mat2d, Mat3d, Mat4d,
			RID,
			OBJ
		>;

		using storage_type = mpl::rename<std::variant, data_types>;

		template <class T> static constexpr bool is_valid_type_v{ mpl::contains_v<T, data_types> };

		template <VarType_ T> using type_t = mpl::nth<T, data_types>;

		template <class> static constexpr VarType_ type_v{ VarType_MAX };
		template <> static constexpr VarType_ type_v<bool>{ VarType_Bool };
		template <> static constexpr VarType_ type_v<i32>{ VarType_I32 };
		template <> static constexpr VarType_ type_v<Vec2i>{ VarType_Vec2i };
		template <> static constexpr VarType_ type_v<Vec3i>{ VarType_Vec3i };
		template <> static constexpr VarType_ type_v<Vec4i>{ VarType_Vec4i };
		template <> static constexpr VarType_ type_v<Mat2i>{ VarType_Mat2i };
		template <> static constexpr VarType_ type_v<Mat3i>{ VarType_Mat3i };
		template <> static constexpr VarType_ type_v<Mat4i>{ VarType_Mat4i };
		template <> static constexpr VarType_ type_v<i64>{ VarType_I64 };
		template <> static constexpr VarType_ type_v<u32>{ VarType_U32 };
		template <> static constexpr VarType_ type_v<Vec2u>{ VarType_Vec2u };
		template <> static constexpr VarType_ type_v<Vec3u>{ VarType_Vec3u };
		template <> static constexpr VarType_ type_v<Vec4u>{ VarType_Vec4u };
		template <> static constexpr VarType_ type_v<Mat2u>{ VarType_Mat2u };
		template <> static constexpr VarType_ type_v<Mat3u>{ VarType_Mat3u };
		template <> static constexpr VarType_ type_v<Mat4u>{ VarType_Mat4u };
		template <> static constexpr VarType_ type_v<u64>{ VarType_U64 };
		template <> static constexpr VarType_ type_v<f32>{ VarType_F32 };
		template <> static constexpr VarType_ type_v<Vec2f>{ VarType_Vec2f };
		template <> static constexpr VarType_ type_v<Vec3f>{ VarType_Vec3f };
		template <> static constexpr VarType_ type_v<Vec4f>{ VarType_Vec4f };
		template <> static constexpr VarType_ type_v<Mat2f>{ VarType_Mat2f };
		template <> static constexpr VarType_ type_v<Mat3f>{ VarType_Mat3f };
		template <> static constexpr VarType_ type_v<Mat4f>{ VarType_Mat4f };
		template <> static constexpr VarType_ type_v<f64>{ VarType_F64 };
		template <> static constexpr VarType_ type_v<Vec2d>{ VarType_Vec2d };
		template <> static constexpr VarType_ type_v<Vec3d>{ VarType_Vec3d };
		template <> static constexpr VarType_ type_v<Vec4d>{ VarType_Vec4d };
		template <> static constexpr VarType_ type_v<Mat2d>{ VarType_Mat2d };
		template <> static constexpr VarType_ type_v<Mat3d>{ VarType_Mat3d };
		template <> static constexpr VarType_ type_v<Mat4d>{ VarType_Mat4d };
		template <> static constexpr VarType_ type_v<RID>{ VarType_RID };
		template <> static constexpr VarType_ type_v<OBJ>{ VarType_OBJ };

		static constexpr size_t get_size_static(VarType_ const type) noexcept
		{
			switch (type) {
			case VarType_Bool: return sizeof(bool);
			case VarType_I32: return sizeof(i32);
			case VarType_Vec2i: return sizeof(Vec2i);
			case VarType_Vec3i: return sizeof(Vec3i);
			case VarType_Vec4i: return sizeof(Vec4i);
			case VarType_Mat2i: return sizeof(Mat2i);
			case VarType_Mat3i: return sizeof(Mat3i);
			case VarType_Mat4i: return sizeof(Mat4i);
			case VarType_I64: return sizeof(i64);
			case VarType_U32: return sizeof(u32);
			case VarType_Vec2u: return sizeof(Vec2u);
			case VarType_Vec3u: return sizeof(Vec3u);
			case VarType_Vec4u: return sizeof(Vec4u);
			case VarType_Mat2u: return sizeof(Mat2u);
			case VarType_Mat3u: return sizeof(Mat3u);
			case VarType_Mat4u: return sizeof(Mat4u);
			case VarType_U64: return sizeof(u64);
			case VarType_F32: return sizeof(f32);
			case VarType_Vec2f: return sizeof(Vec2f);
			case VarType_Vec3f: return sizeof(Vec3f);
			case VarType_Vec4f: return sizeof(Vec4f);
			case VarType_Mat2f: return sizeof(Mat2f);
			case VarType_Mat3f: return sizeof(Mat3f);
			case VarType_Mat4f: return sizeof(Mat4f);
			case VarType_F64: return sizeof(f64);
			case VarType_Vec2d: return sizeof(Vec2d);
			case VarType_Vec3d: return sizeof(Vec3d);
			case VarType_Vec4d: return sizeof(Vec4d);
			case VarType_Mat2d: return sizeof(Mat2d);
			case VarType_Mat3d: return sizeof(Mat3d);
			case VarType_Mat4d: return sizeof(Mat4d);
			case VarType_RID: return sizeof(RID);
			case VarType_OBJ: return sizeof(OBJ);
			}
			return 0;
		}

		static constexpr void const * get_data_static(VarType_ const type, storage_type const & data) noexcept
		{
			switch (type) {
			case VarType_Bool: return &std::get<bool>(data);
			case VarType_I32: return &std::get<i32>(data);
			case VarType_Vec2i: return &std::get<Vec2i>(data);
			case VarType_Vec3i: return &std::get<Vec3i>(data);
			case VarType_Vec4i: return &std::get<Vec4i>(data);
			case VarType_Mat2i: return &std::get<Mat2i>(data);
			case VarType_Mat3i: return &std::get<Mat3i>(data);
			case VarType_Mat4i: return &std::get<Mat4i>(data);
			case VarType_I64: return &std::get<i64>(data);
			case VarType_U32: return &std::get<u32>(data);
			case VarType_Vec2u: return &std::get<Vec2u>(data);
			case VarType_Vec3u: return &std::get<Vec3u>(data);
			case VarType_Vec4u: return &std::get<Vec4u>(data);
			case VarType_Mat2u: return &std::get<Mat2u>(data);
			case VarType_Mat3u: return &std::get<Mat3u>(data);
			case VarType_Mat4u: return &std::get<Mat4u>(data);
			case VarType_U64: return &std::get<u64>(data);
			case VarType_F32: return &std::get<f32>(data);
			case VarType_Vec2f: return &std::get<Vec2f>(data);
			case VarType_Vec3f: return &std::get<Vec3f>(data);
			case VarType_Vec4f: return &std::get<Vec4f>(data);
			case VarType_Mat2f: return &std::get<Mat2f>(data);
			case VarType_Mat3f: return &std::get<Mat3f>(data);
			case VarType_Mat4f: return &std::get<Mat4f>(data);
			case VarType_F64: return &std::get<f64>(data);
			case VarType_Vec2d: return &std::get<Vec2d>(data);
			case VarType_Vec3d: return &std::get<Vec3d>(data);
			case VarType_Vec4d: return &std::get<Vec4d>(data);
			case VarType_Mat2d: return &std::get<Mat2d>(data);
			case VarType_Mat3d: return &std::get<Mat3d>(data);
			case VarType_Mat4d: return &std::get<Mat4d>(data);
			case VarType_RID: return &std::get<RID>(data);
			case VarType_OBJ: return &std::get<OBJ>(data);
			}
			return nullptr;
		}

	public:
		Var() noexcept : m_type{ VarType_MAX }, m_data{} {}

		Var(nullptr_t) noexcept : Var{} {}

		Var(Var const & v) { copy(v); }

		Var(Var && v) noexcept { swap(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var(T const & v) : m_type{ type_v<T> }, m_data{ v } {}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var(T && v) noexcept : m_type{ type_v<T> }, m_data{ std::move(v) } {}

		Var & operator=(Var const & v) { return copy(v); }

		Var & operator=(Var && v) noexcept { return swap(v); }

		Var & operator=(nullptr_t) noexcept { return invalidate(); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & operator=(T const & v) { return assign(v); }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & operator=(T && v) noexcept { return assign(std::move(v)); }

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & assign(T const & v) {
			m_type = type_v<T>;
			m_data = v;
			return (*this);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> Var & assign(T && v) noexcept {
			m_type = type_v<T>;
			m_data = std::move(v);
			return (*this);
		}

		Var & invalidate() noexcept {
			m_type = VarType_MAX;
			storage_type{}.swap(m_data);
			return (*this);
		}

		Var & copy(Var const & v) noexcept {
			if (this != std::addressof(v)) {
				m_type = v.m_type;
				m_data = v.m_data;
			}
			return (*this);
		}

		Var & swap(Var & v) noexcept {
			if (this != std::addressof(v)) {
				util::swap(m_type, v.m_type);
				m_data.swap(v.m_data);
			}
			return (*this);
		}

	public:
		NODISCARD operator bool() const noexcept { return is_valid(); }

		NODISCARD void const * data() const noexcept { return get_data_static(m_type, m_data); }

		NODISCARD size_t size() const noexcept { return get_size_static(m_type); }

		NODISCARD VarType_ type() const noexcept { return m_type; }
		
		NODISCARD bool holds(VarType_ const t) const noexcept { return m_type == t; }

		template <VarType_ T
		> NODISCARD bool holds() const noexcept { return m_type == T; }

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD bool holds() const noexcept { return m_type == type_v<T>; }
		
		NODISCARD bool is_valid() const { return m_type < VarType_MAX; }

		NODISCARD bool is_bool() const { return m_type == VarType_Bool; }

		NODISCARD bool is_signed() const noexcept { return m_type == VarType_I32 || m_type == VarType_I64; }
		
		NODISCARD bool is_unsigned() const noexcept { return m_type == VarType_U32 || m_type == VarType_U64; }

		NODISCARD bool is_int() const noexcept { return is_signed() || is_unsigned(); }
		
		NODISCARD bool is_float() const noexcept { return m_type == VarType_F32; }
		
		NODISCARD bool is_double() const noexcept { return m_type == VarType_F64; }

		NODISCARD bool is_rid() const noexcept { return m_type == VarType_RID; }

		NODISCARD bool is_object() const noexcept { return m_type == VarType_OBJ; }

		NODISCARD bool is_vector() const noexcept {
			return
				m_type == VarType_Vec2i || m_type == VarType_Vec3i || m_type == VarType_Vec4i ||
				m_type == VarType_Vec2u || m_type == VarType_Vec3u || m_type == VarType_Vec4u ||
				m_type == VarType_Vec2f || m_type == VarType_Vec3f || m_type == VarType_Vec4f ||
				m_type == VarType_Vec2d || m_type == VarType_Vec3d || m_type == VarType_Vec4d;
		}

		NODISCARD bool is_matrix() const noexcept {
			return
				m_type == VarType_Mat2i || m_type == VarType_Mat3i || m_type == VarType_Mat4i ||
				m_type == VarType_Mat2u || m_type == VarType_Mat3u || m_type == VarType_Mat4u ||
				m_type == VarType_Mat2f || m_type == VarType_Mat3f || m_type == VarType_Mat4f ||
				m_type == VarType_Mat2d || m_type == VarType_Mat3d || m_type == VarType_Mat4d;
		}

	public:
		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() noexcept -> T &
		{
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <class T, std::enable_if_t<is_valid_type_v<T>, int> = 0
		> NODISCARD auto get() const noexcept -> T const &
		{
			ASSERT(holds<T>());
			return std::get<T>(m_data);
		}

		template <VarType_ T, std::enable_if_t<(T < VarType_MAX), int> = 0
		> NODISCARD auto get() noexcept -> type_t<T> &
		{
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

		template <VarType_ T, std::enable_if_t<(T < VarType_MAX), int> = 0
		> NODISCARD auto get() const noexcept -> type_t<T> const &
		{
			ASSERT(holds<T>());
			return get<type_t<T>>();
		}

	public:
		friend bool operator==(Var const & a, Var const & b) noexcept {
			return (std::addressof(a) == std::addressof(b)) || (a.m_type == b.m_type && a.m_data == b.m_data);
		}

		friend bool operator!=(Var const & a, Var const & b) noexcept {
			return (std::addressof(a) != std::addressof(b)) && (a.m_type != b.m_type || a.m_data != b.m_data);
		}

	private:
		VarType_ m_type{ VarType_MAX };
		storage_type m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_VAR_HPP_
