#ifndef _PNU_FILE_ACCESS_HPP_
#define _PNU_FILE_ACCESS_HPP_

#include <core/object/class.hpp>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum FileAccessType_ {
		FileAccessType_Resources,
		FileAccessType_User,
		FileAccessType_Filesystem,
		FileAccessType_MAX
	};

	enum FileMode_ {
		FileMode_Read,		// r
		FileMode_Write,		// w
		FileMode_ReadWrite,	// r+
		FileMode_WriteRead,	// w+
		FileMode_MAX
	};

	enum FileCompressionMode_ {
		FileCompressionMode_FastLZ,
		FileCompressionMode_Deflate,
		FileCompressionMode_ZSTD,
		FileCompressionMode_GZip,
		FileCompressionMode_MAX
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file access
	class PNU_API FileAccess : public Object
	{
		DEFINE_CLASS(FileAccess, Object);

		bool m_big_endian{};
		FileAccessType_ m_access_type{};

	protected:
		using CreateFunc = Ref<FileAccess>(*)();
		static CreateFunc __create_func[FileAccessType_MAX];
		static Ref<FileAccess> create(FileAccessType_ access_type);
		static Ref<FileAccess> create_for_path(String const & path);

		NODISCARD FileAccessType_ get_access_type() const noexcept { return m_access_type; }
		void set_access_type(FileAccessType_ value) noexcept { m_access_type = value; }

		virtual Error_ open_internal(String const & path, FileMode_ mode) = 0;

	public:
		static Ref<FileAccess> open(String const & path, FileMode_ mode, Error_ * error = nullptr);
		virtual Error_ reopen(String const & path, FileMode_ mode);

		virtual FileAccess & flush() = 0;
		virtual FileAccess & close() = 0;
		NODISCARD virtual bool file_exists(String const & path) = 0;
		NODISCARD virtual bool is_open() const = 0;
		virtual FileAccess & seek(u64 position) = 0;
		virtual FileAccess & seek_end(i64 position = 0) = 0;
		NODISCARD virtual u64 get_position() const = 0;
		NODISCARD virtual u64 get_length() const = 0;
		NODISCARD virtual bool eof_reached() const = 0;
		NODISCARD virtual Error_ get_error() const = 0;

		NODISCARD virtual String get_path() const { return {}; }
		NODISCARD virtual String get_path_abs() const { return {}; }

		NODISCARD bool is_big_endian() const noexcept { return m_big_endian; }
		virtual void set_big_endian(bool big_endian) noexcept { m_big_endian = big_endian; }

		NODISCARD virtual u8 read_8() const = 0;
		NODISCARD virtual u16 read_16() const;
		NODISCARD virtual u32 read_32() const;
		NODISCARD virtual u64 read_64() const;
		NODISCARD virtual f32 read_float() const;
		NODISCARD virtual f64 read_double() const;
		NODISCARD virtual String read_token() const;
		NODISCARD virtual String read_line() const;
		virtual size_t read_buffer(u8 * data, size_t const size) const;
		NODISCARD DynamicBuffer read_buffer(size_t const size) const;

		virtual FileAccess & write_8(u8 value) = 0;
		virtual FileAccess & write_16(u16 value);
		virtual FileAccess & write_32(u32 value);
		virtual FileAccess & write_64(u64 value);
		virtual FileAccess & write_float(f32 value);
		virtual FileAccess & write_double(f64 value);
		virtual FileAccess & write_token(String const & value);
		virtual FileAccess & write_line(String const & value);
		virtual FileAccess & write_buffer(u8 const * data, size_t const size);
		FileAccess & write_buffer(DynamicBuffer const & buffer);

	public:
		static bool exists(String const & path);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_FILE_ACCESS_HPP_
