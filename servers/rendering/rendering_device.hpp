#ifndef _PNU_RENDERING_DEVICE_HPP_
#define _PNU_RENDERING_DEVICE_HPP_

#include <servers/display_server.hpp>

// rendering device
namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// low-level graphics api
	class PNU_API RenderingDevice : public Object
	{
		DEFINE_CLASS(RenderingDevice, Object);

	private:
		static RenderingDevice * __singleton;

	public:
		RenderingDevice() noexcept { __singleton = this; }
		virtual ~RenderingDevice() noexcept override = default;
		FORCE_INLINE static RenderingDevice * get_singleton() noexcept { return __singleton; }
#define RENDERING_DEVICE (Pnu::RenderingDevice::get_singleton())

	public:
		virtual void initialize() = 0;
		virtual void finalize() = 0;

	public:
		/* MISC */
		enum DataFormat_
		{
			DataFormat_R4G4_UNORM_PACK8,
			DataFormat_R4G4B4A4_UNORM_PACK16,
			DataFormat_B4G4R4A4_UNORM_PACK16,
			DataFormat_R5G6B5_UNORM_PACK16,
			DataFormat_B5G6R5_UNORM_PACK16,
			DataFormat_R5G5B5A1_UNORM_PACK16,
			DataFormat_B5G5R5A1_UNORM_PACK16,
			DataFormat_A1R5G5B5_UNORM_PACK16,
			DataFormat_R8_UNORM,
			DataFormat_R8_SNORM,
			DataFormat_R8_USCALED,
			DataFormat_R8_SSCALED,
			DataFormat_R8_UINT,
			DataFormat_R8_SINT,
			DataFormat_R8_SRGB,
			DataFormat_R8G8_UNORM,
			DataFormat_R8G8_SNORM,
			DataFormat_R8G8_USCALED,
			DataFormat_R8G8_SSCALED,
			DataFormat_R8G8_UINT,
			DataFormat_R8G8_SINT,
			DataFormat_R8G8_SRGB,
			DataFormat_R8G8B8_UNORM,
			DataFormat_R8G8B8_SNORM,
			DataFormat_R8G8B8_USCALED,
			DataFormat_R8G8B8_SSCALED,
			DataFormat_R8G8B8_UINT,
			DataFormat_R8G8B8_SINT,
			DataFormat_R8G8B8_SRGB,
			DataFormat_B8G8R8_UNORM,
			DataFormat_B8G8R8_SNORM,
			DataFormat_B8G8R8_USCALED,
			DataFormat_B8G8R8_SSCALED,
			DataFormat_B8G8R8_UINT,
			DataFormat_B8G8R8_SINT,
			DataFormat_B8G8R8_SRGB,
			DataFormat_R8G8B8A8_UNORM,
			DataFormat_R8G8B8A8_SNORM,
			DataFormat_R8G8B8A8_USCALED,
			DataFormat_R8G8B8A8_SSCALED,
			DataFormat_R8G8B8A8_UINT,
			DataFormat_R8G8B8A8_SINT,
			DataFormat_R8G8B8A8_SRGB,
			DataFormat_B8G8R8A8_UNORM,
			DataFormat_B8G8R8A8_SNORM,
			DataFormat_B8G8R8A8_USCALED,
			DataFormat_B8G8R8A8_SSCALED,
			DataFormat_B8G8R8A8_UINT,
			DataFormat_B8G8R8A8_SINT,
			DataFormat_B8G8R8A8_SRGB,
			DataFormat_A8B8G8R8_UNORM_PACK32,
			DataFormat_A8B8G8R8_SNORM_PACK32,
			DataFormat_A8B8G8R8_USCALED_PACK32,
			DataFormat_A8B8G8R8_SSCALED_PACK32,
			DataFormat_A8B8G8R8_UINT_PACK32,
			DataFormat_A8B8G8R8_SINT_PACK32,
			DataFormat_A8B8G8R8_SRGB_PACK32,
			DataFormat_A2R10G10B10_UNORM_PACK32,
			DataFormat_A2R10G10B10_SNORM_PACK32,
			DataFormat_A2R10G10B10_USCALED_PACK32,
			DataFormat_A2R10G10B10_SSCALED_PACK32,
			DataFormat_A2R10G10B10_UINT_PACK32,
			DataFormat_A2R10G10B10_SINT_PACK32,
			DataFormat_A2B10G10R10_UNORM_PACK32,
			DataFormat_A2B10G10R10_SNORM_PACK32,
			DataFormat_A2B10G10R10_USCALED_PACK32,
			DataFormat_A2B10G10R10_SSCALED_PACK32,
			DataFormat_A2B10G10R10_UINT_PACK32,
			DataFormat_A2B10G10R10_SINT_PACK32,
			DataFormat_R16_UNORM,
			DataFormat_R16_SNORM,
			DataFormat_R16_USCALED,
			DataFormat_R16_SSCALED,
			DataFormat_R16_UINT,
			DataFormat_R16_SINT,
			DataFormat_R16_SFLOAT,
			DataFormat_R16G16_UNORM,
			DataFormat_R16G16_SNORM,
			DataFormat_R16G16_USCALED,
			DataFormat_R16G16_SSCALED,
			DataFormat_R16G16_UINT,
			DataFormat_R16G16_SINT,
			DataFormat_R16G16_SFLOAT,
			DataFormat_R16G16B16_UNORM,
			DataFormat_R16G16B16_SNORM,
			DataFormat_R16G16B16_USCALED,
			DataFormat_R16G16B16_SSCALED,
			DataFormat_R16G16B16_UINT,
			DataFormat_R16G16B16_SINT,
			DataFormat_R16G16B16_SFLOAT,
			DataFormat_R16G16B16A16_UNORM,
			DataFormat_R16G16B16A16_SNORM,
			DataFormat_R16G16B16A16_USCALED,
			DataFormat_R16G16B16A16_SSCALED,
			DataFormat_R16G16B16A16_UINT,
			DataFormat_R16G16B16A16_SINT,
			DataFormat_R16G16B16A16_SFLOAT,
			DataFormat_R32_UINT,
			DataFormat_R32_SINT,
			DataFormat_R32_SFLOAT,
			DataFormat_R32G32_UINT,
			DataFormat_R32G32_SINT,
			DataFormat_R32G32_SFLOAT,
			DataFormat_R32G32B32_UINT,
			DataFormat_R32G32B32_SINT,
			DataFormat_R32G32B32_SFLOAT,
			DataFormat_R32G32B32A32_UINT,
			DataFormat_R32G32B32A32_SINT,
			DataFormat_R32G32B32A32_SFLOAT,
			DataFormat_R64_UINT,
			DataFormat_R64_SINT,
			DataFormat_R64_SFLOAT,
			DataFormat_R64G64_UINT,
			DataFormat_R64G64_SINT,
			DataFormat_R64G64_SFLOAT,
			DataFormat_R64G64B64_UINT,
			DataFormat_R64G64B64_SINT,
			DataFormat_R64G64B64_SFLOAT,
			DataFormat_R64G64B64A64_UINT,
			DataFormat_R64G64B64A64_SINT,
			DataFormat_R64G64B64A64_SFLOAT,
			DataFormat_B10G11R11_UFLOAT_PACK32,
			DataFormat_E5B9G9R9_UFLOAT_PACK32,
			DataFormat_D16_UNORM,
			DataFormat_X8_D24_UNORM_PACK32,
			DataFormat_D32_SFLOAT,
			DataFormat_S8_UINT,
			DataFormat_D16_UNORM_S8_UINT,
			DataFormat_D24_UNORM_S8_UINT,
			DataFormat_D32_SFLOAT_S8_UINT,
			DataFormat_BC1_RGB_UNORM_BLOCK,
			DataFormat_BC1_RGB_SRGB_BLOCK,
			DataFormat_BC1_RGBA_UNORM_BLOCK,
			DataFormat_BC1_RGBA_SRGB_BLOCK,
			DataFormat_BC2_UNORM_BLOCK,
			DataFormat_BC2_SRGB_BLOCK,
			DataFormat_BC3_UNORM_BLOCK,
			DataFormat_BC3_SRGB_BLOCK,
			DataFormat_BC4_UNORM_BLOCK,
			DataFormat_BC4_SNORM_BLOCK,
			DataFormat_BC5_UNORM_BLOCK,
			DataFormat_BC5_SNORM_BLOCK,
			DataFormat_BC6H_UFLOAT_BLOCK,
			DataFormat_BC6H_SFLOAT_BLOCK,
			DataFormat_BC7_UNORM_BLOCK,
			DataFormat_BC7_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8A1_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8A1_SRGB_BLOCK,
			DataFormat_ETC2_R8G8B8A8_UNORM_BLOCK,
			DataFormat_ETC2_R8G8B8A8_SRGB_BLOCK,
			DataFormat_EAC_R11_UNORM_BLOCK,
			DataFormat_EAC_R11_SNORM_BLOCK,
			DataFormat_EAC_R11G11_UNORM_BLOCK,
			DataFormat_EAC_R11G11_SNORM_BLOCK,
			DataFormat_ASTC_4x4_UNORM_BLOCK,
			DataFormat_ASTC_4x4_SRGB_BLOCK,
			DataFormat_ASTC_5x4_UNORM_BLOCK,
			DataFormat_ASTC_5x4_SRGB_BLOCK,
			DataFormat_ASTC_5x5_UNORM_BLOCK,
			DataFormat_ASTC_5x5_SRGB_BLOCK,
			DataFormat_ASTC_6x5_UNORM_BLOCK,
			DataFormat_ASTC_6x5_SRGB_BLOCK,
			DataFormat_ASTC_6x6_UNORM_BLOCK,
			DataFormat_ASTC_6x6_SRGB_BLOCK,
			DataFormat_ASTC_8x5_UNORM_BLOCK,
			DataFormat_ASTC_8x5_SRGB_BLOCK,
			DataFormat_ASTC_8x6_UNORM_BLOCK,
			DataFormat_ASTC_8x6_SRGB_BLOCK,
			DataFormat_ASTC_8x8_UNORM_BLOCK,
			DataFormat_ASTC_8x8_SRGB_BLOCK,
			DataFormat_ASTC_10x5_UNORM_BLOCK,
			DataFormat_ASTC_10x5_SRGB_BLOCK,
			DataFormat_ASTC_10x6_UNORM_BLOCK,
			DataFormat_ASTC_10x6_SRGB_BLOCK,
			DataFormat_ASTC_10x8_UNORM_BLOCK,
			DataFormat_ASTC_10x8_SRGB_BLOCK,
			DataFormat_ASTC_10x10_UNORM_BLOCK,
			DataFormat_ASTC_10x10_SRGB_BLOCK,
			DataFormat_ASTC_12x10_UNORM_BLOCK,
			DataFormat_ASTC_12x10_SRGB_BLOCK,
			DataFormat_ASTC_12x12_UNORM_BLOCK,
			DataFormat_ASTC_12x12_SRGB_BLOCK,
			DataFormat_G8B8G8R8_422_UNORM,
			DataFormat_B8G8R8G8_422_UNORM,
			DataFormat_G8_B8_R8_3PLANE_420_UNORM,
			DataFormat_G8_B8R8_2PLANE_420_UNORM,
			DataFormat_G8_B8_R8_3PLANE_422_UNORM,
			DataFormat_G8_B8R8_2PLANE_422_UNORM,
			DataFormat_G8_B8_R8_3PLANE_444_UNORM,
			DataFormat_R10X6_UNORM_PACK16,
			DataFormat_R10X6G10X6_UNORM_2PACK16,
			DataFormat_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
			DataFormat_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
			DataFormat_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
			DataFormat_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
			DataFormat_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
			DataFormat_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
			DataFormat_R12X4_UNORM_PACK16,
			DataFormat_R12X4G12X4_UNORM_2PACK16,
			DataFormat_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
			DataFormat_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
			DataFormat_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
			DataFormat_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
			DataFormat_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
			DataFormat_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
			DataFormat_G16B16G16R16_422_UNORM,
			DataFormat_B16G16R16G16_422_UNORM,
			DataFormat_G16_B16_R16_3PLANE_420_UNORM,
			DataFormat_G16_B16R16_2PLANE_420_UNORM,
			DataFormat_G16_B16_R16_3PLANE_422_UNORM,
			DataFormat_G16_B16R16_2PLANE_422_UNORM,
			DataFormat_G16_B16_R16_3PLANE_444_UNORM,
			DataFormat_PVRTC1_2BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC1_4BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC2_2BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC2_4BPP_UNORM_BLOCK_IMG,
			DataFormat_PVRTC1_2BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC1_4BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC2_2BPP_SRGB_BLOCK_IMG,
			DataFormat_PVRTC2_4BPP_SRGB_BLOCK_IMG,
			DataFormat_MAX
		};

		static constexpr bool is_depth_stencil_format(DataFormat_ const f) noexcept
		{
			return f == DataFormat_D16_UNORM
				|| f == DataFormat_X8_D24_UNORM_PACK32
				|| f == DataFormat_D32_SFLOAT
				|| f == DataFormat_D16_UNORM_S8_UINT
				|| f == DataFormat_D24_UNORM_S8_UINT
				|| f == DataFormat_D32_SFLOAT_S8_UINT;
		}

		enum BarrierMask_
		{
			BarrierMask_Raster = 1 << 0,
			BarrierMask_Compute = 1 << 1,
			BarrierMask_Transfer = 1 << 2,
			BarrierMask_All = BarrierMask_Raster | BarrierMask_Compute | BarrierMask_Transfer
		};

	public:
		/* BUFFER API */
		enum BufferType_
		{
			BufferType_VertexBuffer,
			BufferType_IndexBuffer,
			BufferType_UniformBuffer,
			BufferType_MAX
		};

		virtual RID buffer_create(BufferType_ buffer_type, size_t size_in_bytes, DynamicBuffer const & data = {}) = 0;
		virtual void buffer_destroy(RID buffer) = 0;
		virtual void buffer_update(RID buffer, size_t offset, void const * data, size_t size_in_bytes) = 0;

	public:
		/* VERTEXARRAY API */
		struct VertexLayout
		{
			struct Attribute
			{
				cstring		name;
				DataType_	type;
				u32			count;
				bool		normalized;
				u32			size;
				u32			offset;

				constexpr Attribute(cstring name, DataType_ type, size_t count, bool normalized = false) noexcept
					: name		{ name }
					, type		{ type }
					, count		{ (u32)count }
					, normalized{ normalized }
					, size		{ (u32)(Pnu::get_data_type_size(type) * count) }
					, offset	{}
				{
				}
			};

			u32 stride{};
			Vector<Attribute> attributes{};

			VertexLayout() noexcept : VertexLayout{
				{ "Position"	, DataType_F32, 4 }, // Vec4f
				{ "Normal"		, DataType_F32, 4 }, // Vec4f
				{ "UV"			, DataType_F32, 4 }, // Vec4f
				{ "Tangent"		, DataType_F32, 4 }, // Vec4f
				{ "Bitangent"	, DataType_F32, 4 }, // Vec4f
			} {}

			void update()
			{
				u32 offset{};
				stride = 0;
				for (VertexLayout::Attribute & e : attributes) {
					e.offset = offset;
					offset += e.size;
					stride += e.size;
				}
			}

			template <class It> VertexLayout(It first, It last) noexcept : attributes{ first, last } { update(); }
			VertexLayout(std::initializer_list<Attribute> init) noexcept : attributes{ init } { update(); }
			VertexLayout(Vector<Attribute> const & attributes) : attributes{ attributes } { update(); }
			VertexLayout(Vector<Attribute> && attributes) noexcept : attributes{ std::move(attributes) } { update(); }
			template <size_t N> VertexLayout(Attribute const (&arr)[N]) noexcept : attributes{ &arr[0], &arr[N] } { update(); }
		};

		virtual RID vertex_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) = 0;
		virtual RID vertex_array_create(size_t vertex_count, VertexLayout const & layout, Vector<RID> const & buffers) = 0;
		virtual void vertex_array_destroy(RID vertex_array) = 0;

		virtual RID index_buffer_create(size_t index_count, DataType_ index_type = DataType_U32, DynamicBuffer const & data = {}) = 0;
		virtual RID index_array_create(RID index_buffer, size_t index_offset, size_t index_count) = 0;
		virtual void index_array_destroy(RID index_array) = 0;

	public:
		/* SAMPLER API */
		enum SamplerFilter_
		{
			SamplerFilter_Nearest,
			SamplerFilter_Linear,
		};

		enum SamplerRepeatMode_
		{
			SamplerRepeatMode_Repeat,
			SamplerRepeatMode_MirroredRepeat,
			SamplerRepeatMode_ClampToEdge,
			SamplerRepeatMode_ClampToBorder,
			SamplerRepeatMode_MirrorClampToEdge,
			SamplerRepeatMode_MAX
		};

		enum SamplerBorderColor_
		{
			SamplerBorderColor_Float_Transparent_Black,
			SamplerBorderColor_Int_Transparent_Black,
			SamplerBorderColor_Float_Opaque_Black,
			SamplerBorderColor_Int_Opaque_Black,
			SamplerBorderColor_Float_Opaque_White,
			SamplerBorderColor_Int_Opaque_White,
			SamplerBorderColor_MAX
		};

		struct SamplerCreateInfo
		{
			SamplerFilter_ mag_filter{ SamplerFilter_Nearest }, min_filter{ SamplerFilter_Nearest }, mip_filter{ SamplerFilter_Nearest };
			SamplerRepeatMode_ repeat_s{ SamplerRepeatMode_Repeat }, repeat_t{ SamplerRepeatMode_Repeat }, repeat_r{ SamplerRepeatMode_Repeat };
			f32 lod_bias{ 0 };
			bool use_anisotropy{ false };
			f32 anisotropy_max{ 1.f };
			bool enable_compare{};
			CompareOperator_ compare_op{ CompareOperator_Always };
			f32 min_lod{ 0 }, max_lod{ 1e20f };
			SamplerBorderColor_ border_color{ SamplerBorderColor_Float_Opaque_Black };
			bool unnormalized_uvw{ false };
		};

		virtual RID sampler_create(SamplerCreateInfo const & sampler_state = {}) = 0;
		virtual void sampler_destroy(RID sampler) = 0;

	public:
		/* TEXTURE API */
		enum TextureType_
		{
			TextureType_1D,
			TextureType_2D,
			TextureType_3D,
			TextureType_Cube,
			TextureType_1D_Array,
			TextureType_2D_Array,
			TextureType_Cube_Array,
			TextureType_Proxy_2D,
			TextureType_Proxy_1D_Array,
			TextureType_Proxy_Cubemap,
			TextureType_MAX
		};

		enum TextureSamples_
		{
			TextureSamples_1,
			TextureSamples_2,
			TextureSamples_4,
			TextureSamples_8,
			TextureSamples_16,
			TextureSamples_32,
			TextureSamples_64,
			TextureSamples_MAX
		};

		enum TextureFlags_
		{
			TextureFlags_None,
			TextureFlags_Sampling = 1 << 0,
			TextureFlags_ColorAttachment = 1 << 1,
			TextureFlags_DepthStencilAttachment = 1 << 2,
			TextureFlags_Storage = 1 << 3,
			TextureFlags_Storage_Atomic = 1 << 4,
			TextureFlags_CpuRead = 1 << 5,
			TextureFlags_CanUpdate = 1 << 6,
			TextureFlags_CanCopyFrom = 1 << 7,
			TextureFlags_CanCopyTo = 1 << 8,
			TextureFlags_ResolveAttachment = 1 << 9,

			TextureFlags_Default =
				TextureFlags_Sampling |
				TextureFlags_CanUpdate |
				TextureFlags_CanCopyFrom
		};

		enum TextureSwizzle_
		{
			TextureSwizzle_Identity,
			TextureSwizzle_Zero,
			TextureSwizzle_One,
			TextureSwizzle_R,
			TextureSwizzle_G,
			TextureSwizzle_B,
			TextureSwizzle_A,
			TextureSwizzle_MAX
		};

		enum TextureSliceType_
		{
			TextureSliceType_2D,
			TextureSliceType_Cube,
			TextureSliceType_3D,
			TextureSliceType_2D_Array,
		};

		struct TextureCreateInfo
		{
			TextureType_ texture_type{ TextureType_2D };
			DataFormat_ color_format{ DataFormat_R8G8B8_UNORM };
			u32 width{ 1 }, height{ 1 }, depth{ 1 }, layers{ 1 }, mipmaps{ 1 };
			SamplerFilter_ min_filter{ SamplerFilter_Nearest }, mag_filter{ SamplerFilter_Nearest };
			SamplerRepeatMode_ repeat_s{ SamplerRepeatMode_Repeat }, repeat_t{ SamplerRepeatMode_Repeat };
			TextureSamples_ samples{ TextureSamples_1 };
			i32 usage_flags{ TextureFlags_Default };
			DataFormat_ color_format_srgb{ DataFormat_MAX };
			TextureSwizzle_ swizzle_r{ TextureSwizzle_R }, swizzle_g{ TextureSwizzle_G }, swizzle_b{ TextureSwizzle_B }, swizzle_a{ TextureSwizzle_A };
		};

		virtual RID texture_create(TextureCreateInfo const & format, DynamicBuffer const & data = {}) = 0;
		virtual void texture_destroy(RID texture) = 0;
		virtual void texture_update(RID texture, DynamicBuffer const & data = {}) = 0;
		virtual void * texture_get_handle(RID texture) = 0;
		virtual DynamicBuffer texture_get_data(RID texture) = 0;

	public:
		/* FRAMEBUFFER */
		virtual RID framebuffer_create(Vector<RID> const & texture_attachments) = 0;
		virtual void framebuffer_destroy(RID framebuffer) = 0;
		virtual void framebuffer_set_size(RID framebuffer, i32 width, i32 height) = 0;

	public:
		/* SHADER */
		enum ShaderLanguage_
		{
			ShaderLanguage_GLSL,
			ShaderLanguage_HLSL,
			ShaderLanguage_MAX,
		};

		enum ShaderStage_
		{
			ShaderStage_Vertex,
			ShaderStage_Fragment,
			ShaderStage_Geometry,
			ShaderStage_TesselationControl,
			ShaderStage_TesselationEvaluation,
			ShaderStage_Compute,
			ShaderStage_MAX,

			ShaderStage_Vertex_Bit = 1 << 0,
			ShaderStage_Fragment_Bit = 1 << 1,
			ShaderStage_Geometry_Bit = 1 << 2,
			ShaderStage_TesselationControl_Bit = 1 << 3,
			ShaderStage_TesselationEvaluation_Bit = 1 << 4,
			ShaderStage_Compute_Bit = 1 << 5,
		};

		struct ShaderStageData
		{
			ShaderStage_ shader_stage{ ShaderStage_MAX };
			DynamicBuffer code{};
		};

		struct ShaderCreateInfo
		{
			ShaderStageData stage_data[ShaderStage_MAX]{};
		};

		virtual RID shader_create(ShaderStageData const (&spec)[ShaderStage_MAX] = {}) = 0;
		virtual void shader_destroy(RID shader) = 0;
		virtual String shader_get_code(RID shader) = 0;
		virtual void shader_set_code(RID shader, String const & value) = 0;
		virtual i32 shader_get_uniform_location(RID shader, String const & name) = 0;

	public:
		/* UNIFORMS */
		enum UniformType_
		{
			UniformType_Sampler,
			UniformType_SamplerWithTexture,
			UniformType_Texture,
			UniformType_Image,
			UniformType_TextureBuffer,
			UniformType_SamplerWithTextureBuffer,
			UniformType_ImageBuffer,
			UniformType_UniformBuffer,
			UniformType_StorageBuffer,
			UniformType_InputAttachment,
			UniformType_MAX
		};

		struct Uniform
		{
			UniformType_ uniform_type{ UniformType_Image };
			i32 binding{};
			Vector<RID> ids{};
		};

		template <class ... Ts
		> using Std140 = ConstantBuffer<16, Ts...>;

		virtual RID uniform_buffer_create(size_t size_in_bytes, DynamicBuffer const & data = {}) = 0;
		virtual RID uniform_set_create(Vector<Uniform> const & uniforms, RID shader) = 0;
		virtual void uniform_set_destroy(RID uniform_set) = 0;

	public:
		/* PIPELINE API */
		enum RenderPrimitive_
		{
			RenderPrimitive_Points,
			RenderPrimitive_Lines,
			RenderPrimitive_LinesWithAdjacency,
			RenderPrimitive_Linestrips,
			RenderPrimitive_LinestripsWithAdjacency,
			RenderPrimitive_Triangles,
			RenderPrimitive_TrianglesWithAdjacency,
			RenderPrimitive_TriangleStrips,
			RenderPrimitive_TriangleStripsWithAdjacency,
			RenderPrimitive_TriangleStripsWithRestartIndex,
			RenderPrimitive_TesselationPatch,
			RenderPrimitive_MAX
		};

		enum PolygonCullMode_
		{
			PolygonCullMode_Disabled,
			PolygonCullMode_Front,
			PolygonCullMode_Back,
		};

		enum PolygonFrontFace_
		{
			PolygonFrontFace_Clockwise,
			PolygonFrontFace_CounterClockwise,
		};

		struct RasterizationState
		{
			bool enable_depth_clamp{ false };
			bool discard_primitives{ false };
			bool enable_wireframe{ false };
			PolygonCullMode_ cull_mode{ PolygonCullMode_Disabled };
			PolygonFrontFace_ front_face{ PolygonFrontFace_Clockwise };
			bool enable_depth_bias{ false };
			f32 depth_bias_constant_factor{ 0.f };
			f32 depth_bias_clamp{ 0.f };
			f32 depth_bias_slope_factor{ 0.f };
			f32 line_width{ 1.f };
			u32 patch_control_points{ 1 };
		};

		struct MultisampleState
		{
			TextureSamples_ sample_count{ TextureSamples_1 };
			bool enable_sample_shading{ false };
			f32 min_sample_shading{ 0.f };
			Vector<u32> sample_mask{};
			bool enable_alpha_to_coverage{ false };
			bool enable_alpha_to_one{ false };
		};

		enum StencilOperation_
		{
			StencilOperation_Keep,
			StencilOperation_Zero,
			StencilOperation_Replace,
			StencilOperation_IncrementAndClamp,
			StencilOperation_DecrementAndClamp,
			StencilOperation_Invert,
			StencilOperation_IncrementAndWrap,
			StencilOperation_DecrementAndWrap,
			StencilOperation_MAX
		};

		struct StencilOperationState
		{
			StencilOperation_ fail{ StencilOperation_Zero };
			StencilOperation_ pass{ StencilOperation_Zero };
			StencilOperation_ depth_fail{ StencilOperation_Zero };
			CompareOperator_ compare{ CompareOperator_Always };
			u32 compare_mask{ 0 };
			u32 write_mask{ 0 };
			u32 reference{ 0 };
		};

		struct DepthStencilState
		{
			bool enable_depth_test{ false };
			bool enable_depth_write{ false };
			CompareOperator_ depth_compare_operator{ CompareOperator_Always };
			bool enable_depth_range{ false };
			f32 depth_range_min{ 0.f }, depth_range_max{ 0.f };
			bool enable_stencil{ false };
			StencilOperationState front_op{}, back_op{};
		};

		enum BlendFactor_
		{
			BlendFactor_Zero,
			BlendFactor_One,
			BlendFactor_SrcColor,
			BlendFactor_OneMinusSrcColor,
			BlendFactor_DstColor,
			BlendFactor_OneMinusDstColor,
			BlendFactor_SrcAlpha,
			BlendFactor_OneMinusSrcAlpha,
			BlendFactor_DstAlpha,
			BlendFactor_OneMinusDstAlpha,
			BlendFactor_ConstantColor,
			BlendFactor_OneMinusConstantColor,
			BlendFactor_ConstantAlpha,
			BlendFactor_OneMinusConstantAlpha,
			BlendFactor_SrcAlphaSaturate,
			BlendFactor_Src1Color,
			BlendFactor_OneMinusSrc1Color,
			BlendFactor_Src1Alpha,
			BlendFactor_OneMinusSrc1Alpha,
			BlendFactor_MAX
		};

		enum BlendOperation_
		{
			BlendOperation_Add,
			BlendOperation_Subtract,
			BlendOperation_ReverseSubtract,
			BlendOperation_Minimum,
			BlendOperation_Maximum,
			BlendOperation_MAX
		};

		struct ColorBlendState
		{
			struct Attachment
			{
				bool enable_blend{ false };
				BlendFactor_ src_color_blend_factor{ BlendFactor_Zero };
				BlendFactor_ dst_color_blend_factor{ BlendFactor_Zero };
				BlendOperation_ color_blend_op{ BlendOperation_Add };
				BlendFactor_ src_alpha_blend_factor{ BlendFactor_Zero };
				BlendFactor_ dst_alpha_blend_factor{ BlendFactor_Zero };
				BlendOperation_ alpha_blend_op{ BlendOperation_Add };
				bool write_r{ true }, write_g{ true }, write_b{ true }, write_a{ true };
			};

			bool enable_logic_op{ false };
			
			LogicOperation_ logic_op{ LogicOperation_Clear };

			Color blend_constant{};
			
			Vector<Attachment> attachments{};

			static ColorBlendState create_disabled(i32 num_attachments = 1)
			{
				ColorBlendState temp{};
				temp.attachments.reserve(num_attachments);
				for (i32 i = 0; i < num_attachments; ++i) {
					temp.attachments.push_back({});
				}
				return temp;
			}

			static ColorBlendState create_blend(i32 num_attachments = 1)
			{
				ColorBlendState temp{};
				temp.attachments.reserve(num_attachments);
				for (i32 i = 0; i < num_attachments; ++i) {
					temp.attachments.push_back({ true,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
						BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
						true, true, true, true });
				}
				return temp;
			}
		};

		virtual RID render_pipeline_create(RID shader, RenderPrimitive_ primitive, RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depth_stencil_state, ColorBlendState const & color_blend_state) = 0;
		virtual void render_pipeline_destroy(RID pipeline) = 0;
		
	public:
		/* DRAW ListRef */
		using DrawListID = typename size_t;

		enum InitialAction_
		{
			InitialAction_Clear,		// start rendering and clear the whole framebuffer (region or not) (supply params)
			InitialAction_ClearRegion,	// start rendering and clear the framebuffer in the specified region (supply params)
			InitialAction_Keep,			// start rendering, but keep attached color texture contents (depth will be cleared)
			InitialAction_Drop,			// start rendering, ignore what is there, just write above it
			InitialAction_Continue,		// continue rendering (framebuffer must have been left in "continue" state as action previously)
			InitialAction_MAX
		};

		enum FinalAction_
		{
			FinalAction_Read,		// will no longer render to it, allows attached textures to be read again, but depth buffer contents will be dropped (Can't be read from)
			FinalAction_Discard,	// discard contents after rendering
			FinalAction_Continue,	// will continue rendering later, attached textures can't be read until re-bound with "finish"
			FinalAction_MAX
		};

		virtual DrawListID draw_list_begin_for_screen(DS::WindowID window = DS::MAIN_WINDOW_ID, Color const & clear_color = {}) = 0;
		virtual DrawListID draw_list_begin(RID framebuffer, InitialAction_ initial_color_action, FinalAction_ final_color_action, InitialAction_ initial_depth_action, FinalAction_ final_depth_action, Vector<Color> const & clear_colors = {}, f32 clear_depth = 1.f, i32 clear_stencil = 0) = 0;
		virtual void draw_list_bind_pipeline(DrawListID draw_list, RID pipeline) = 0;
		virtual void draw_list_bind_uniform_set(DrawListID draw_list, RID uniform_set, size_t index) = 0;
		virtual void draw_list_bind_vertex_array(DrawListID draw_list, RID vertex_array) = 0;
		virtual void draw_list_bind_index_array(DrawListID draw_list, RID index_array) = 0;
		virtual void draw_list_set_line_width(DrawListID draw_list, f32 width) = 0;
		virtual void draw_list_set_push_constant(DrawListID draw_list, void const * data, size_t size) = 0;
		virtual void draw_list_enable_scissor(DrawListID draw_list, IntRect const & rect) = 0;
		virtual void draw_list_disable_scissor(DrawListID draw_list) = 0;
		virtual void draw_list_draw(DrawListID draw_list, bool use_indices, size_t instances, size_t procedural_vertices) = 0;
		virtual void draw_list_end() = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using RD = RenderingDevice;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_RENDERING_DEVICE_HPP_
