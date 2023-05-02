#include <core/io/file_access.hpp>
#include <core/io/file_access_pack.hpp>
#include <cstdio>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccess, t, TypeFlags_IsAbstract) {}

	FileAccess::CreateFunc FileAccess::__create_func[FileAccessType_MAX]{};

	Ref<FileAccess> FileAccess::create(FileAccessType_ access_type)
	{
		Ref<FileAccess> file{ VALIDATE(__create_func[access_type])() };
		file->set_access_type(access_type);
		return file;
	}

	Ref<FileAccess> FileAccess::create_for_path(String const & path)
	{
		Ref<FileAccess> file;
		if (path.has_prefix("res://")) {
			file = create(FileAccessType_Resources);
		}
		else if (path.has_prefix("usr://")) {
			file = create(FileAccessType_User);
		}
		else {
			file = create(FileAccessType_Filesystem);
		}
		return file;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Ref<FileAccess> FileAccess::open(String const & path, FileMode_ mode, Error_ * error)
	{
		Ref<FileAccess> file{};
		
		if (PackedData * pack; (mode != FileMode_Write) && (pack = get_packed_data()) && pack->is_enabled() && (file = pack->try_open_path(path))) {
			if (error) { *error = Error_OK; }
			return file;
		}

		if ((file = create_for_path(path)) && (file->open_internal(path, mode) == Error_OK)) {
			if (error) { *error = Error_OK; }
			return file;
		}

		if (error) { *error = Error_Unknown; }
		return nullptr;
	}

	Error_ FileAccess::reopen(String const & path, FileMode_ mode)
	{
		return open_internal(path, mode);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u16 FileAccess::read_16() const
	{
		u8 a{ read_8() }, b{ read_8() };
		if (m_big_endian) { util::swap(a, b); }
		u16 c{ b }; c <<= 8; c |= a;
		return c;
	}

	u32 FileAccess::read_32() const
	{
		u16 a{ read_16() }, b{ read_16() };
		if (m_big_endian) { util::swap(a, b); }
		u32 c{ b }; c <<= 16; c |= a;
		return c;
	}

	u64 FileAccess::read_64() const
	{
		u32 a{ read_32() }, b{ read_32() };
		if (m_big_endian) { util::swap(a, b); }
		u64 c{ b }; c <<= 32; c |= a;
		return c;
	}

	f32 FileAccess::read_float() const
	{
		return f32();
	}

	f64 FileAccess::read_double() const
	{
		return f64();
	}

	String FileAccess::read_token() const
	{
		String token{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == ' ' && !token.empty()) { break; }
			else { token += c; }
			c = read_8();
		}
		return token;
	}

	String FileAccess::read_line() const
	{
		String line{};
		u8 c{ read_8() };
		while (!eof_reached()) {
			if (c == '\0' || c == '\n') {
				line += '\n';
				return line;
			}
			else if (c != '\r') {
				line += c;
			}
			c = read_8();
		}
		return line;
	}

	size_t FileAccess::read_buffer(u8 * data, size_t const size) const
	{
		size_t i{};
		for (; i < size && !eof_reached(); ++i) {
			data[i] = read_8();
		}
		return i;
	}

	DynamicBuffer FileAccess::read_buffer(size_t const size) const
	{
		DynamicBuffer buf{};
		if (!size) { return buf; }
		buf.resize(size);
		if (size_t length{ read_buffer(buf.data(), buf.size()) }; length < size) {
			buf.resize(length);
		}
		return buf;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	FileAccess & FileAccess::write_16(u16 value)
	{
		u8 a; a = value & 0xFF;
		u8 b; b = value >> 8;
		if (m_big_endian) { util::swap(a, b); }
		return write_8(a); write_8(b);
	}

	FileAccess & FileAccess::write_32(u32 value)
	{
		u16 a; a = value & 0xFFFF;
		u16 b; b = value >> 16;
		if (m_big_endian) { util::swap(a, b); }
		return write_16(a).write_16(b);
	}

	FileAccess & FileAccess::write_64(u64 value)
	{
		u32 a; a = value & 0xFFFFFFFF;
		u32 b; b = value >> 32;
		if (m_big_endian) { util::swap(a, b); }
		return write_32(a).write_32(b);
	}

	FileAccess & FileAccess::write_float(f32 value)
	{
		return (*this);
	}

	FileAccess & FileAccess::write_double(f64 value)
	{
		return (*this);
	}

	FileAccess & FileAccess::write_token(String const & value)
	{
		if (value.empty()) { return (*this); }
		else { return write_buffer((u8 *)value.data(), value.size()); }
	}

	FileAccess & FileAccess::write_line(String const & value)
	{
		return write_token(value).write_8((u8)'\n');
	}

	FileAccess & FileAccess::write_buffer(u8 const * data, size_t const size)
	{
		if (!data || !size) { return (*this); }
		for (size_t i{}; i < size; ++i) { write_8(data[i]); }
		return (*this);
	}

	FileAccess & FileAccess::write_buffer(DynamicBuffer const & buffer)
	{
		if (buffer.empty()) { return (*this); }
		else { return write_buffer(buffer.data(), buffer.size()); }
	}

	bool FileAccess::exists(String const & path)
	{
		return (get_packed_data() && get_packed_data()->has_path(path))
			|| open(path, FileMode_Read).is_valid();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}