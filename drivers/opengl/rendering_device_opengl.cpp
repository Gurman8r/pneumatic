#if OPENGL_ENABLED

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace pn;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef OPENGL_LOADER_CUSTOM
#if __has_include(OPENGL_LOADER_CUSTOM)
#	include OPENGL_LOADER_CUSTOM
#endif

#elif defined(OPENGL_LOADER_ES2)
#include <GLES2/gl2.h>
#define OPENGL_INIT()

#elif defined(OPENGL_LOADER_ES3)
#if defined(os_apple) && (TARGET_OS_IOS || TARGET_OS_TV)
#	include <OpenGLES/ES3/gl.h>
#else
#	include <GLES3/gl3.h>
#endif
#define OPENGL_INIT()

#elif defined(OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#define OPENGL_INIT() do { \
	glewExperimental = true; \
	ASSERT(glewInit() == GLEW_OK); \
} while (0)

#elif defined(OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#define OPENGL_INIT() do { \
	ASSERT(gl3wInit()); \
} while(0)

#elif defined(OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#define OPENGL_INIT() do { \
	ASSERT(gladLoadGL()); \
} while(0)

#else
#	error "Can't find OpenGL"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define glSetEnabled(m_cap, m_bool) ((m_bool) ? glEnable(m_cap) : glDisable(m_cap))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void glCheckError(cstring expr, cstring file, u32 line)
{
	u32 const code{ glGetError() };
	cstring name{}, desc{};
	switch (code)
	{
	default: return;
	case GL_INVALID_ENUM: {
		name = "GL_INVALID_ENUM";
		desc = "An unacceptable value has been specified for an enumerated argument.";
	} break;
	case GL_INVALID_VALUE: {
		name = "GL_INVALID_VALUE";
		desc = "A numeric argument is out of range.";
	} break;
	case GL_INVALID_OPERATION: {
		name = "GL_INVALID_OPERATION";
		desc = "The specified operation is not allowed in the current state.";
	} break;
	case GL_STACK_OVERFLOW: {
		name = "GL_STACK_OVERFLOW";
		desc = "This command would cause a stack overflow.";
	} break;
	case GL_STACK_UNDERFLOW: {
		name = "GL_STACK_UNDERFLOW";
		desc = "This command would cause a stack underflow.";
	} break;
	case GL_OUT_OF_MEMORY: {
		name = "GL_OUT_OF_MEMORY";
		desc = "There is not enough memory left to execute the command.";
	} break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: {
		name = "GL_INVALID_FRAMEBUFFER_OPERATION";
		desc = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
	} break;
	}

	get_os()->printerrf(
		"\nAn internal OpenGL call failed in \"%s\" (%u) \n"
		"Code: %u\n"
		"Expression: %s\n"
		"Description: %s\n"
		, file, line, code, expr, desc);
}

#if DEBUG_ENABLED
#	define glCheck(expr) \
	do { expr; glCheckError(TOSTR(expr), __FILE__, __LINE__); } while (0)
#else
#	define glCheck(expr) expr
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void get_data_info(RD::DataFormat_ const data_format, ImageFormat_ * image_format, u32 * internal_format, u32 * format, u32 * type) noexcept
{
	ImageFormat_ _image_format{};
	u32 _internal_format{}, _format{}, _type{};
	switch (data_format)
	{
	case RD::DataFormat_R8_UNORM: {
		_image_format = ImageFormat_R8;
		_internal_format = GL_RGBA;
		_format = GL_RED;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8_UNORM: {
		_image_format = ImageFormat_RG8;
		_internal_format = GL_RGBA;
		_format = GL_RG;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8B8_UNORM: {
		_image_format = ImageFormat_RGB8;
		_internal_format = GL_RGBA;
		_format = GL_RGB;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8B8A8_UNORM: {
		_image_format = ImageFormat_RGBA8;
		_internal_format = GL_RGBA;
		_format = GL_RGBA;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_D24_UNORM_S8_UINT: {
		_image_format = ImageFormat_L8;
		_internal_format = GL_DEPTH24_STENCIL8;
		_format = GL_DEPTH_STENCIL;
		_type = GL_UNSIGNED_INT_24_8;
	} break;
	}
	if (image_format) { *image_format = _image_format; }
	if (internal_format) { *internal_format = _internal_format; }
	if (format) { *format = _format; }
	if (type) { *type = _type; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ENUM_MAPPING(TO_GL, DataType_, u32,
	
	GL_NONE, // void
	
	GL_BOOL, GL_BYTE, GL_BYTE, // bool, u8, char
	
	GL_BYTE, GL_SHORT, GL_INT, GL_INT64_ARB, // i8, i16, i32, i64
	
	GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_UNSIGNED_INT64_ARB, // u8, u16, u32, u64
	
	GL_FLOAT, GL_DOUBLE // f32, f64
);

ENUM_MAPPING(TO_GL, CompareOperator_, u32,
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
);

ENUM_MAPPING(TO_GL, LogicOperation_, u32,
	GL_CLEAR,
	GL_AND,
	GL_AND_REVERSE,
	GL_COPY,
	GL_AND_INVERTED,
	GL_NOOP,
	GL_XOR,
	GL_OR,
	GL_NOR,
	GL_EQUIV,
	GL_INVERT,
	GL_OR_REVERSE,
	GL_NAND,
	GL_SET
);

using BufferType_ = RD::BufferType_;
ENUM_MAPPING(TO_GL, BufferType_, u32,
	GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
	GL_UNIFORM_BUFFER
);

using TextureType_ = RD::TextureType_;
ENUM_MAPPING(TO_GL, TextureType_, u32,
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_CUBE_MAP_ARRAY,
	GL_PROXY_TEXTURE_2D,
	GL_PROXY_TEXTURE_1D_ARRAY,
	GL_PROXY_TEXTURE_CUBE_MAP
);

using TextureSwizzle_ = RD::TextureSwizzle_;
ENUM_MAPPING(TO_GL, TextureSwizzle_, i32,
	0, // TextureSwizzle_Identity
	GL_ZERO,
	GL_ONE,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA
);

using SamplerFilter_ = RD::SamplerFilter_;
ENUM_MAPPING(TO_GL, SamplerFilter_, u32,
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
);

using SamplerRepeatMode_ = RD::SamplerRepeatMode_;
ENUM_MAPPING(TO_GL, SamplerRepeatMode_, u32,
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRROR_CLAMP_TO_EDGE
);

using ShaderStage_ = RD::ShaderStage_;
ENUM_MAPPING(TO_GL, ShaderStage_, u32,
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER
);

using RenderPrimitive_ = RD::RenderPrimitive_;
ENUM_MAPPING(TO_GL, RenderPrimitive_, u32,
	GL_POINTS,
	GL_LINES,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP,
	GL_LINE_STRIP_ADJACENCY,
	GL_TRIANGLES,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_STRIP_ADJACENCY,
	0, // RenderPrimitive_TriangleStripsWithRestartIndex
	GL_PATCHES
);

using PolygonCullMode_ = RD::PolygonCullMode_;
ENUM_MAPPING(TO_GL, PolygonCullMode_, u32,
	GL_FRONT_AND_BACK,
	GL_FRONT,
	GL_BACK
);

using PolygonFrontFace_ = RD::PolygonFrontFace_;
ENUM_MAPPING(TO_GL, PolygonFrontFace_, u32,
	GL_CW,
	GL_CCW
);

using StencilOperation_ = RD::StencilOperation_;
ENUM_MAPPING(TO_GL, StencilOperation_, u32,
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP
);

using BlendFactor_ = RD::BlendFactor_;
ENUM_MAPPING(TO_GL, BlendFactor_, u32,
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA
);

using BlendOperation_ = RD::BlendOperation_;
ENUM_MAPPING(TO_GL, BlendOperation_, u32,
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX
);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(RenderingDeviceOpenGL, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{}
{
	OPENGL_INIT();

	glCheck(glEnable(GL_ALPHA_TEST));
	glCheck(glAlphaFunc(GL_GREATER, 0.0001f));
	glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::initialize()
{
}

void RenderingDeviceOpenGL::finalize()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::buffer_create(BufferType_ buffer_type, size_t size_in_bytes, DynamicBuffer const & data)
{
	ASSERT(0 < size_in_bytes);

	_BufferBase * b{};
	switch (buffer_type)
	{
	// vertex buffer
	case BufferType_VertexBuffer: {
		b = memnew(_VertexBuffer{});
		b->usage = data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	} break;

	// index buffer
	case BufferType_IndexBuffer: {
		b = memnew(_IndexBuffer{});
		b->usage = data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	} break;

	// uniform buffer
	case BufferType_UniformBuffer: {
		b = memnew(_UniformBuffer{});
		b->usage = GL_STATIC_DRAW;
	} break;
	}

	if (!b) { return nullptr; }
	b->buffer_type = TO_GL(buffer_type);
	b->size = (u32)size_in_bytes;
	b->data = data;

	glCheck(glGenBuffers(1, &b->handle));
	glCheck(glBindBuffer(b->buffer_type, b->handle));
	glCheck(glBufferData(b->buffer_type, b->size, b->data.data(), b->usage));
	glCheck(glBindBuffer(b->buffer_type, NULL));

	return (RID)b;
}

void RenderingDeviceOpenGL::buffer_destroy(RID buffer)
{
	_BufferBase & b{ *VALIDATE((_BufferBase *)buffer) };
	if (b.handle) { glCheck(glDeleteBuffers(1, &b.handle)); }
	switch (b.buffer_type) {
	default: { CRASH("INVALID BUFFER TYPE"); } break;
	case GL_ARRAY_BUFFER: { memdelete((_VertexBuffer *)buffer); } break;
	case GL_ELEMENT_ARRAY_BUFFER: { memdelete((_IndexBuffer *)buffer); } break;
	case GL_UNIFORM_BUFFER: { memdelete((_UniformBuffer *)buffer); } break;
	}
}

void RenderingDeviceOpenGL::buffer_update(RID buffer, size_t offset, void const * data, size_t size_in_bytes)
{
	_BufferBase & b{ *VALIDATE((_BufferBase *)buffer) };
	b.data.write(offset, data, size_in_bytes);
	if (b.size < offset + size_in_bytes) { b.size = (u32)(offset + size_in_bytes); }
	glCheck(glBindBuffer(b.buffer_type, b.handle));
	glCheck(glBufferSubData(b.buffer_type, (u32)offset, b.size, b.data.data()));
	glCheck(glBindBuffer(b.buffer_type, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertex_buffer_create(size_t size_in_bytes, DynamicBuffer const & data)
{
	ASSERT(0 < size_in_bytes);
	return buffer_create(BufferType_VertexBuffer, size_in_bytes, data);
}

RID RenderingDeviceOpenGL::vertex_array_create(size_t vertex_count, VertexLayout const & layout, Vector<RID> const & buffers)
{
	ASSERT(0 < vertex_count);
	RID const vertex_array{ (RID)memnew(_VertexArray{}) };
	_VertexArray & va{ *VALIDATE((_VertexArray *)vertex_array) };
	va.vertex_count = (u32)vertex_count;
	va.layout = layout;
	va.buffers = buffers;

	glCheck(glGenVertexArrays(1, &va.handle));
	glCheck(glBindVertexArray(va.handle));

	for (RID const vertex_buffer : va.buffers)
	{
		_VertexBuffer & vb{ *VALIDATE((_VertexBuffer *)vertex_buffer) };

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb.handle));

		for (u32 i = 0, imax = (u32)va.layout.attributes.size(); i < imax; ++i)
		{
			VertexLayout::Attribute const & fmt{ va.layout.attributes[i] };

			if (fmt.type == DataType_I32)
			{
				glCheck(glVertexAttribIPointer(i, fmt.count, GL_INT, va.layout.stride, (void *)(intptr_t)fmt.offset));
			}
			else
			{
				ASSERT(fmt.type == DataType_F32 || fmt.type == DataType_Bool);

				i32 const fmt_type{ fmt.type == DataType_F32 ? GL_FLOAT : GL_BOOL };

				glCheck(glVertexAttribPointer(i, fmt.count, fmt_type, fmt.normalized, va.layout.stride, (void *)(intptr_t)fmt.offset));
			}

			glCheck(glEnableVertexAttribArray((u32)i));
		}
	}

	return vertex_array;
}

void RenderingDeviceOpenGL::vertex_array_destroy(RID vertex_array)
{
	_VertexArray & va{ *VALIDATE((_VertexArray *)vertex_array) };
	for (RID const vertex_buffer : va.buffers) { if (vertex_buffer) { buffer_destroy(vertex_buffer); } }
	if (va.handle) { glCheck(glDeleteVertexArrays(1, &va.handle)); }
	memdelete((_VertexArray *)vertex_array);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::index_buffer_create(size_t index_count, DataType_ index_type, DynamicBuffer const & data)
{
	ASSERT(0 < index_count);
	ASSERT(index_type == DataType_U8 || index_type == DataType_U16 || index_type == DataType_U32);
	RID const index_buffer{ buffer_create(BufferType_IndexBuffer, index_count * get_data_type_size(index_type), data) };
	_IndexBuffer & ib{ *VALIDATE((_IndexBuffer *)index_buffer) };
	ib.index_count = (u32)index_count;
	ib.index_type = TO_GL(index_type);
	return index_buffer;
}

RID RenderingDeviceOpenGL::index_array_create(RID index_buffer, size_t index_offset, size_t index_count)
{
	RID const index_array{ (RID)memnew(_IndexArray{}) };
	_IndexBuffer & ib{ *VALIDATE((_IndexBuffer *)index_buffer) };
	_IndexArray & ia{ *VALIDATE((_IndexArray *)index_array) };
	ia.index_buffer = index_buffer;
	ia.index_offset = (u32)index_offset;
	ia.index_count = (u32)index_count;
	ia.index_type = ib.index_type;
	return index_array;
}

void RenderingDeviceOpenGL::index_array_destroy(RID index_array)
{
	_IndexArray & ia{ *VALIDATE((_IndexArray *)index_array) };
	if (ia.index_buffer) { buffer_destroy(ia.index_buffer); }
	memdelete((_IndexArray *)index_array);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureCreateInfo const & spec, DynamicBuffer const & data)
{
	RID const texture{ (RID)memnew(_Texture{}) };
	_Texture & t{ *VALIDATE((_Texture *)texture) };
	t.texture_type = TO_GL(spec.texture_type);
	t.color_format = spec.color_format;
	t.color_format_srgb = spec.color_format_srgb;
	t.width_2d = t.width = spec.width;
	t.height_2d = t.height = spec.height;
	t.depth = spec.depth;
	t.layers = spec.layers;
	t.mipmaps = spec.mipmaps;
	t.samples = spec.samples;
	t.repeat_s = TO_GL(spec.repeat_s);
	t.repeat_t = TO_GL(spec.repeat_t);
	t.min_filter = TO_GL((SamplerFilter_)(spec.min_filter + (2 * (0 < spec.mipmaps))));
	t.mag_filter = TO_GL(spec.mag_filter);
	t.usage_flags = spec.usage_flags;
	t.swizzle_mask = {
		(spec.swizzle_r == TextureSwizzle_Identity) ? GL_RED : TO_GL(spec.swizzle_r),
		(spec.swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : TO_GL(spec.swizzle_g),
		(spec.swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : TO_GL(spec.swizzle_b),
		(spec.swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : TO_GL(spec.swizzle_a) };
	get_data_info(t.color_format, &t.image_format, 0, 0, 0);
	_texture_update(t, data.data());
	return texture;
}

void RenderingDeviceOpenGL::texture_destroy(RID texture)
{
	_Texture & t{ *VALIDATE((_Texture *)texture) };
	if (t.handle) { glCheck(glDeleteTextures(1, &t.handle)); }
	memdelete((_Texture *)texture);
}

void RenderingDeviceOpenGL::texture_update(RID texture, DynamicBuffer const & data)
{
	_Texture & t{ *VALIDATE((_Texture *)texture) };
	if (t.handle) { glCheck(glDeleteTextures(1, &t.handle)); }
	_texture_update(t, data.data());
}

void RenderingDeviceOpenGL::_texture_update(_Texture & t, void const * data)
{
	u32 _internal_format, _format, _type;
	get_data_info(t.color_format, nullptr, &_internal_format, &_format, &_type);

	glCheck(glGenTextures(1, &t.handle));
	glCheck(glBindTexture(t.texture_type, t.handle));

	switch (t.texture_type) {
	default: {
		CRASH("INVALID TEXTURE TYPE");
	} break;
	case GL_TEXTURE_2D: {
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, t.width, t.height, 0, _format, _type, data));
	} break;
	case GL_TEXTURE_3D: {
		// TODO: load texture3d
	} break;
	case GL_TEXTURE_CUBE_MAP: {
		// TODO: load texturecube
	} break;
	}

	if (0 < t.mipmaps) { glCheck(glGenerateMipmap(t.texture_type)); }
	glCheck(glTexParameteri(t.texture_type, GL_TEXTURE_WRAP_S, t.repeat_s));
	glCheck(glTexParameteri(t.texture_type, GL_TEXTURE_WRAP_T, t.repeat_t));
	glCheck(glTexParameteri(t.texture_type, GL_TEXTURE_MIN_FILTER, t.min_filter));
	glCheck(glTexParameteri(t.texture_type, GL_TEXTURE_MAG_FILTER, t.mag_filter));
	glCheck(glTexParameteriv(t.texture_type, GL_TEXTURE_SWIZZLE_RGBA, t.swizzle_mask));
}

void * RenderingDeviceOpenGL::texture_get_handle(RID texture)
{
	return (void *)(intptr_t)(VALIDATE((_Texture *)texture)->handle);
}

DynamicBuffer RenderingDeviceOpenGL::texture_get_data(RID texture)
{
	_Texture & t{ *VALIDATE((_Texture *)texture) };

	u32 _internal_format, _format;
	get_data_info(t.color_format, nullptr, &_internal_format, nullptr, &_format);

	DynamicBuffer data{};
	data.resize((size_t)(t.width * t.height * t.depth));
	glCheck(glBindTexture(t.texture_type, t.handle));
	glCheck(glGetTexImage(t.texture_type, 0, _internal_format, _format, data));
	return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	_Framebuffer * const fb{ memnew(_Framebuffer{}) };
	fb->texture_attachments = texture_attachments;

	glCheck(glGenFramebuffers(1, &fb->handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));

	u32 color_attachment_index{};
	for (size_t i{}; i < fb->texture_attachments.size(); ++i)
	{
		_Texture & t{ *VALIDATE((_Texture *)fb->texture_attachments[i]) };
		if (i == 0) { fb->width = t.width; fb->height = t.height; }

		glCheck(glBindTexture(t.texture_type, t.handle));

		if (flag_read(t.usage_flags, TextureFlags_ColorAttachment)) {
			ASSERT(!flag_read(t.usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), t.texture_type, t.handle, 0));
		}
		else {
			ASSERT(flag_read(t.usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, t.texture_type, t.handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	return (RID)fb;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID framebuffer)
{
	_Framebuffer & fb{ *VALIDATE((_Framebuffer *)framebuffer) };
	for (RID texture : fb.texture_attachments) { texture_destroy(texture); }
	if (fb.handle) { glCheck(glDeleteFramebuffers(1, &fb.handle)); }
	memdelete((_Framebuffer *)framebuffer);
}

void RenderingDeviceOpenGL::framebuffer_set_size(RID framebuffer, i32 width, i32 height)
{
	_Framebuffer & fb{ *VALIDATE((_Framebuffer *)framebuffer) };
	if (fb.width == width && fb.height == height) { return; }
	fb.width = width;
	fb.height = height;

	if (fb.handle) { glCheck(glDeleteFramebuffers(1, &fb.handle)); }
	glCheck(glGenFramebuffers(1, &fb.handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb.handle));

	u32 color_attachment_index{};
	for (size_t i{}; i < fb.texture_attachments.size(); ++i)
	{
		_Texture & t{ *VALIDATE((_Texture *)fb.texture_attachments[i]) };
		t.width = fb.width;
		t.height = fb.height;

		texture_update(fb.texture_attachments[i]);

		if (flag_read(t.usage_flags, TextureFlags_ColorAttachment)) {
			ASSERT(!flag_read(t.usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), t.texture_type, t.handle, 0));
		}
		else {
			ASSERT(flag_read(t.usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, t.texture_type, t.handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::sampler_create(SamplerCreateInfo const & sampler_state)
{
	_Sampler * const ss{ memnew(_Sampler{}) };
	ss->mag_filter = TO_GL(sampler_state.mag_filter);
	ss->min_filter = TO_GL(sampler_state.min_filter);
	ss->mip_filter = TO_GL(sampler_state.mip_filter);
	ss->repeat_s = TO_GL(sampler_state.repeat_s);
	ss->repeat_t = TO_GL(sampler_state.repeat_t);
	ss->repeat_r = TO_GL(sampler_state.repeat_r);
	ss->use_anisotropy = sampler_state.use_anisotropy;
	ss->anisotropy_max = sampler_state.anisotropy_max;
	ss->enable_compare = sampler_state.enable_compare;
	ss->compare_op = TO_GL(sampler_state.compare_op);
	ss->lod_bias = sampler_state.lod_bias;
	ss->min_lod = sampler_state.min_lod;
	ss->max_lod = sampler_state.max_lod;
	ss->border_color = sampler_state.border_color;
	ss->unnormalized_uvw = sampler_state.unnormalized_uvw;

	glCheck(glGenSamplers(1, &ss->handle));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_S, ss->repeat_s));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_T, ss->repeat_t));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_R, ss->repeat_r));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_MAG_FILTER, ss->mag_filter));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_MIN_FILTER, ss->min_filter));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_COMPARE_MODE, ss->enable_compare ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_COMPARE_FUNC, ss->compare_op));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_LOD_BIAS, ss->lod_bias));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MAX_ANISOTROPY, ss->anisotropy_max));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MIN_LOD, ss->min_lod));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MAX_LOD, ss->max_lod));

	switch (ss->border_color)
	{
	case SamplerBorderColor_Float_Transparent_Black: {
		constexpr Vec4f c{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Transparent_Black: {
		constexpr Vec4i c{ 0, 0, 0, 0 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Float_Opaque_Black: {
		constexpr Vec4f c{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Opaque_Black: {
		constexpr Vec4i c{ 0, 0, 0, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Float_Opaque_White: {
		constexpr Vec4f c{ 1.f, 1.f, 1.f, 1.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Opaque_White: {
		constexpr Vec4i c{ 1, 1, 1, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	}

	return (RID)ss;
}

void RenderingDeviceOpenGL::sampler_destroy(RID sampler)
{
	_Sampler & ss{ *VALIDATE((_Sampler *)sampler) };
	if (ss.handle) { glCheck(glDeleteSamplers(1, &ss.handle)); }
	memdelete((_Sampler *)sampler);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(ShaderStageData const (&spec)[ShaderStage_MAX])
{
	_Shader * const s{ memnew(_Shader{}) };
	glCheck(s->handle = glCreateProgramObjectARB());

	// validate spec
	for (ShaderStageData const & stage : spec) {
		if (!stage.code.empty()) { s->stage_data.push_back(stage); }
	}

	// compile
	for (ShaderStageData const & stage : s->stage_data)
	{
		cstring code_ptr{ VALIDATE(stage.code.c_str()) };

		u32 obj;
		glCheck(obj = glCreateShaderObjectARB(TO_GL(stage.shader_stage)));
		glCheck(glShaderSourceARB(obj, 1, &code_ptr, nullptr));
		glCheck(glCompileShaderARB(obj));

		i32 compile_status;
		glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &compile_status));
		if (!compile_status) {
			char log_str[1024]{};
			i32 log_len{};
			glCheck(glGetInfoLogARB(obj, sizeof(log_str), &log_len, log_str));
			glCheck(glDeleteObjectARB(obj));
			glCheck(glDeleteProgramsARB(1, &s->handle));
			memdelete(s);
			get_os()->printerrf("%.*s\n", log_len, log_str);
			return nullptr;
		}

		glCheck(glAttachObjectARB(s->handle, obj));
		glCheck(glDeleteObjectARB(obj));
	}

	// link
	glCheck(glLinkProgramARB(s->handle));
	i32 link_status;
	glCheck(glGetObjectParameterivARB(s->handle, GL_OBJECT_LINK_STATUS_ARB, &link_status));
	if (!link_status) {
		char log_str[1024]{};
		i32 log_len{};
		glCheck(glGetInfoLogARB(s->handle, sizeof(log_str), &log_len, log_str));
		glCheck(glDeleteProgramsARB(1, &s->handle));
		memdelete(s);
		get_os()->printerrf("%.*s\n", log_len, log_str);
		return nullptr;
	}

	return (RID)s;
}

void RenderingDeviceOpenGL::shader_destroy(RID shader)
{
	_Shader & s{ *VALIDATE((_Shader *)shader) };
	if (s.handle) { glCheck(glDeleteProgramsARB(1, &s.handle)); }
	memdelete((_Shader *)shader);
}

String RenderingDeviceOpenGL::shader_get_code(RID shader)
{
	return {};
}

void RenderingDeviceOpenGL::shader_set_code(RID shader, String const & value)
{
}

i32 RenderingDeviceOpenGL::shader_get_uniform_location(RID shader, String const & name)
{
	return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::uniform_buffer_create(size_t size_in_bytes, DynamicBuffer const & data)
{
	ASSERT(0 < size_in_bytes);
	return buffer_create(BufferType_UniformBuffer, size_in_bytes, data);
}

RID RenderingDeviceOpenGL::uniform_set_create(Vector<Uniform> const & uniforms, RID shader)
{
	RID const uniform_set{ (RID)memnew(_UniformSet{}) };
	_UniformSet & us{ *VALIDATE((_UniformSet *)uniform_set) };
	us.shader = shader;
	for (size_t i{}; i < uniforms.size(); ++i)
	{
		_UniformDescriptor & ud{ us.uniforms.emplace_back(_UniformDescriptor{}) };
		ud.uniform_type = uniforms[i].uniform_type;
		ud.binding = uniforms[i].binding;
		switch (ud.uniform_type)
		{
		case UniformType_Sampler: {
		} break;
		case UniformType_SamplerWithTexture: {
		} break;
		case UniformType_Texture: {
			ud.length = (u32)uniforms[i].ids.size();
			for (RID const texture : uniforms[i].ids) { ud.images.push_back(texture); }
		} break;
		case UniformType_Image: {
		} break;
		case UniformType_TextureBuffer: {
		} break;
		case UniformType_SamplerWithTextureBuffer: {
		} break;
		case UniformType_ImageBuffer: {
		} break;
		case UniformType_UniformBuffer: {
			ASSERT(1 == uniforms[i].ids.size());
			ud.length = 1;
			ud.buffers.push_back(uniforms[i].ids[0]);
		} break;
		case UniformType_StorageBuffer: {
		} break;
		case UniformType_InputAttachment: {
		} break;
		}
	}
	return uniform_set;
}

void RenderingDeviceOpenGL::uniform_set_destroy(RID uniform_set)
{
	_UniformSet & us{ *VALIDATE((_UniformSet *)uniform_set) };
	memdelete((_UniformSet *)uniform_set);
}

void RenderingDeviceOpenGL::_uniform_set_bind(_UniformSet const & us)
{
	for (_UniformDescriptor const & ud : us.uniforms)
	{
		switch (ud.uniform_type)
		{
		case UniformType_Sampler: {
		} break;
		case UniformType_SamplerWithTexture: {
		} break;
		case UniformType_Texture: {
			ASSERT(0 < ud.images.size());
			if (1 == ud.images.size()) {
				_Texture & t{ *VALIDATE((_Texture *)ud.images.front()) };
				glCheck(glBindTextureUnit(ud.binding, t.handle));
				glCheck(glUniform1iARB(ud.binding, ud.binding));
			}
		} break;
		case UniformType_Image: {
		} break;
		case UniformType_TextureBuffer: {
		} break;
		case UniformType_SamplerWithTextureBuffer: {
		} break;
		case UniformType_ImageBuffer: {
		} break;
		case UniformType_UniformBuffer: {
			ASSERT(1 == ud.buffers.size());
			_UniformBuffer & ub{ *VALIDATE((_UniformBuffer *)ud.buffers[0]) };
			glCheck(glBindBufferRange(GL_UNIFORM_BUFFER, ud.binding, ub.handle, 0, ub.size));
		} break;
		case UniformType_StorageBuffer: {
		} break;
		case UniformType_InputAttachment: {
		} break;
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::render_pipeline_create(RID shader, RenderPrimitive_ primitive, RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depth_stencil_state, ColorBlendState const & color_blend_state)
{
	RID const pipeline{ (RID)memnew(_RenderPipeline{}) };
	_RenderPipeline & rp{ *VALIDATE((_RenderPipeline *)pipeline) };
	rp.shader = shader;
	rp.primitive = TO_GL(primitive);
	rp.rasterization_state = rasterization_state;
	rp.multisample_state = multisample_state;
	rp.depth_stencil_state = depth_stencil_state;
	rp.color_blend_state = color_blend_state;
	return pipeline;
}

void RenderingDeviceOpenGL::render_pipeline_destroy(RID pipeline)
{
	_RenderPipeline & rp{ *VALIDATE((_RenderPipeline *)pipeline) };
	memdelete((_RenderPipeline *)pipeline);
}

void RenderingDeviceOpenGL::_render_pipeline_bind(_RenderPipeline const & rp)
{
	// pipeline shader
	_Shader & s{ *VALIDATE((_Shader *)rp.shader) };
	glCheck(glUseProgramObjectARB(s.handle));

	// rasterization state
	glCheck(glSetEnabled(GL_DEPTH_CLAMP, rp.rasterization_state.enable_depth_clamp));
	glCheck(glSetEnabled(GL_RASTERIZER_DISCARD, rp.rasterization_state.discard_primitives));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(TO_GL(rp.rasterization_state.cull_mode)));
	glCheck(glFrontFace(TO_GL(rp.rasterization_state.front_face)));
	glCheck(glPixelTransferf(GL_DEPTH_BIAS, rp.rasterization_state.depth_bias_constant_factor));
	glCheck(glLineWidth(rp.rasterization_state.line_width));

	// multisample state
	glCheck(glSetEnabled(GL_SAMPLE_SHADING, rp.multisample_state.enable_sample_shading));
	glCheck(glMinSampleShading(rp.multisample_state.min_sample_shading));
	for (size_t i{}; i < rp.multisample_state.sample_mask.size(); ++i) {
		glCheck(glSampleMaski((u32)i, rp.multisample_state.sample_mask[i]));
	}
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE, rp.multisample_state.enable_alpha_to_coverage));
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_ONE, rp.multisample_state.enable_alpha_to_one));

	// depth stencil state
	glCheck(glSetEnabled(GL_DEPTH_TEST, rp.depth_stencil_state.enable_depth_test));
	glCheck(glDepthFunc(TO_GL(rp.depth_stencil_state.depth_compare_operator)));
	glCheck(glDepthRange(rp.depth_stencil_state.depth_range_min, rp.depth_stencil_state.depth_range_max));
	glCheck(glSetEnabled(GL_STENCIL_TEST, rp.depth_stencil_state.enable_stencil));
	glCheck(glStencilFuncSeparate(GL_FRONT, TO_GL(rp.depth_stencil_state.front_op.compare), rp.depth_stencil_state.front_op.reference, rp.depth_stencil_state.front_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_FRONT, rp.depth_stencil_state.front_op.write_mask));
	glCheck(glStencilFuncSeparate(GL_BACK, TO_GL(rp.depth_stencil_state.back_op.compare), rp.depth_stencil_state.back_op.reference, rp.depth_stencil_state.back_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_BACK, rp.depth_stencil_state.back_op.write_mask));

	// color blend state
	glCheck(glSetEnabled(GL_LOGIC_OP, rp.color_blend_state.enable_logic_op));
	glCheck(glLogicOp(TO_GL(rp.color_blend_state.logic_op)));
	
	// TODO: fixme
	if (rp.color_blend_state.attachments.empty()) { return; }
	auto const & attachment{ rp.color_blend_state.attachments[0] };
	glCheck(glSetEnabled(GL_BLEND, attachment.enable_blend));
	glCheck(glBlendColor(rp.color_blend_state.blend_constant[0], rp.color_blend_state.blend_constant[1], rp.color_blend_state.blend_constant[2], rp.color_blend_state.blend_constant[3]));
	glCheck(glBlendFuncSeparate(TO_GL(attachment.src_color_blend_factor), TO_GL(attachment.dst_color_blend_factor), TO_GL(attachment.src_alpha_blend_factor), TO_GL(attachment.dst_alpha_blend_factor)));
	glCheck(glBlendEquationSeparate(TO_GL(attachment.color_blend_op), TO_GL(attachment.alpha_blend_op)));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RD::DrawListID RenderingDeviceOpenGL::draw_list_begin_for_screen(DS::WindowID window, Color const & clear_color)
{
	DrawListID const draw_list{ m_draw_list.size() };
	_DrawList & dl{ m_draw_list.emplace_back(_DrawList{}) };

	Vec2i const size{ get_display_server()->window_get_size(window) };
	glCheck(glViewport(0, 0, size[0], size[1]));
	glCheck(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	return draw_list;
}

RD::DrawListID RenderingDeviceOpenGL::draw_list_begin(RID framebuffer, InitialAction_ initial_color_action, FinalAction_ final_color_action, InitialAction_ initial_depth_action, FinalAction_ final_depth_action, Vector<Color> const & clear_colors, f32 clear_depth, i32 clear_stencil)
{
	DrawListID const draw_list{ m_draw_list.size() };
	_DrawList & dl{ m_draw_list.emplace_back(_DrawList{}) };

	_Framebuffer const & fb{ *VALIDATE((_Framebuffer const *)framebuffer) };
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb.handle));
	glCheck(glViewport(0, 0, fb.width, fb.height));

	u32 clear_mask{};

	if (1 == clear_colors.size()) {
		clear_mask |= GL_COLOR_BUFFER_BIT;
		glCheck(glClearColor(clear_colors[0][0], clear_colors[0][1], clear_colors[0][2], clear_colors[0][3]));
	}

	clear_mask |= GL_DEPTH_BUFFER_BIT;
	glCheck(glClearDepth(clear_depth));

	clear_mask |= GL_STENCIL_BUFFER_BIT;
	glCheck(glClearStencil(clear_stencil));

	glCheck(glClear(clear_mask));

	return draw_list;
}

void RenderingDeviceOpenGL::draw_list_bind_pipeline(DrawListID draw_list, RID pipeline)
{
	ASSERT(draw_list < m_draw_list.size());
	_DrawList & dl{ m_draw_list[draw_list] };
	if (pipeline == dl.state.pipeline) { return; }
	dl.state.pipeline = pipeline;
	_RenderPipeline const & rp{ *VALIDATE((_RenderPipeline const *)pipeline) };
	dl.command_buffer.push_back(std::bind(&RenderingDeviceOpenGL::_render_pipeline_bind, this, rp));
	if (dl.state.pipeline_shader != rp.shader) {
		dl.state.pipeline_shader = rp.shader;
		// shader changed...
	}
}

void RenderingDeviceOpenGL::draw_list_bind_uniform_set(DrawListID draw_list, RID uniform_set, size_t index)
{
	ASSERT(draw_list < m_draw_list.size());
	_DrawList & dl{ m_draw_list[draw_list] };
	ASSERT(index < ARRAY_SIZE(dl.state.sets));
	if (index >= dl.state.set_count) { dl.state.set_count = (u32)index + 1; }
	dl.state.sets[index].uniform_set = uniform_set;
	dl.state.sets[index].bound = false;
}

void RenderingDeviceOpenGL::draw_list_bind_vertex_array(DrawListID draw_list, RID vertex_array)
{
	ASSERT(draw_list < m_draw_list.size());
	_DrawList & dl{ m_draw_list[draw_list] };
	if (vertex_array == dl.state.vertex_array) { return; }
	dl.state.vertex_array = vertex_array;
	dl.command_buffer.push_back([vertex_array]()
	{
		_VertexArray const & va{ *VALIDATE((_VertexArray const *)vertex_array) };
		glCheck(glBindVertexArray(va.handle));
	});
}

void RenderingDeviceOpenGL::draw_list_bind_index_array(DrawListID draw_list, RID index_array)
{
	ASSERT(draw_list < m_draw_list.size());
	_DrawList & dl{ m_draw_list[draw_list] };
	if (index_array == dl.state.index_array) { return; }
	dl.state.index_array = index_array;
	dl.command_buffer.push_back([index_array]()
	{
		_IndexArray const & ia{ *VALIDATE((_IndexArray const *)index_array) };
		_IndexBuffer const & ib{ *VALIDATE((_IndexBuffer const *)ia.index_buffer) };
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.handle));
	});
}

void RenderingDeviceOpenGL::draw_list_set_line_width(DrawListID draw_list, f32 width)
{
	ASSERT(draw_list < m_draw_list.size());
	m_draw_list[draw_list].command_buffer.push_back([width]()
	{
		glCheck(glLineWidth(width));
	});
}

void RenderingDeviceOpenGL::draw_list_set_push_constant(DrawListID draw_list, void const * data, size_t data_size)
{
	ASSERT(draw_list < m_draw_list.size());
	m_draw_list[draw_list].command_buffer.push_back([data, data_size]()
	{
		// TODO...
	});
}

void RenderingDeviceOpenGL::draw_list_enable_scissor(DrawListID draw_list, IntRect const & rect)
{
	ASSERT(draw_list < m_draw_list.size());
	m_draw_list[draw_list].command_buffer.push_back([rect]()
	{
		glCheck(glEnable(GL_SCISSOR_TEST));
		glCheck(glScissor(rect[0], rect[1], rect[2], rect[3]));
	});
}

void RenderingDeviceOpenGL::draw_list_disable_scissor(DrawListID draw_list)
{
	ASSERT(draw_list < m_draw_list.size());
	m_draw_list[draw_list].command_buffer.push_back([]()
	{
		glCheck(glDisable(GL_SCISSOR_TEST));
	});
}

void RenderingDeviceOpenGL::draw_list_draw(DrawListID draw_list, bool use_indices, size_t instances, size_t procedural_vertices)
{
	ASSERT(draw_list < m_draw_list.size());
	_DrawList & dl{ m_draw_list[draw_list] };

	// bind uniforms
	for (u32 i = 0; i < dl.state.set_count; ++i)
	{
		if (!dl.state.sets[i].bound) {
			_UniformSet const & us{ *VALIDATE((_UniformSet *)dl.state.sets[i].uniform_set) };
			dl.command_buffer.push_back(std::bind(&RenderingDeviceOpenGL::_uniform_set_bind, this, us));
			dl.state.sets[i].bound = true;
		}
	}

	_RenderPipeline const & rp{ *VALIDATE((_RenderPipeline const *)dl.state.pipeline) };
	_VertexArray const & va{ *VALIDATE((_VertexArray const *)dl.state.vertex_array) };
	if (use_indices)
	{
		// draw elements
		_IndexArray const & ia{ *VALIDATE((_IndexArray const *)dl.state.index_array) };
		_IndexBuffer const & ib{ *VALIDATE((_IndexBuffer const *)ia.index_buffer) };
		dl.command_buffer.push_back([
			&buffers = va.buffers,
			primitive = rp.primitive,
			index_count = ia.index_count,
			index_type = ia.index_type,
			instances = (u32)instances
		]()
		{
			for (RID const & vertex_buffer : buffers) {
				_VertexBuffer const & vb{ *VALIDATE((_VertexBuffer const *)vertex_buffer) };
				glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb.handle));
				glCheck(glDrawElementsInstanced(primitive, index_count, index_type, nullptr, instances));
			}
		});
	}
	else
	{
		// draw arrays
		dl.command_buffer.push_back([
			&buffers = va.buffers,
			primitive = rp.primitive,
			vertex_count = va.vertex_count,
			instances = (u32)instances
		]()
		{
			for (RID const & vertex_buffer : buffers) {
				_VertexBuffer const & vb{ *VALIDATE((_VertexBuffer const *)vertex_buffer) };
				glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb.handle));
				glCheck(glDrawArraysInstanced(primitive, 0, vertex_count, instances));
			}
		});
	}
}

void RenderingDeviceOpenGL::draw_list_end()
{
	// end the draw list
	ASSERT(!m_draw_list.empty());
	for (_DrawList const & dl : m_draw_list) { for (auto const & f : dl.command_buffer) { f(); } }
	m_draw_list.clear();

	// reset
	glCheck(glBindVertexArray(NULL));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	glCheck(glUseProgramObjectARB(NULL));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
