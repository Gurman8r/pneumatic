#ifndef _PNU_FILE_ACCESS_WINDOWS_HPP_
#define _PNU_FILE_ACCESS_WINDOWS_HPP_

#include <core/io/file_access.hpp>

#include <stdio.h>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class PNU_API FileAccessWindows : public FileAccess
	{
		DEFINE_CLASS(FileAccessWindows, FileAccess);

		FILE * m_file{};
		FileMode_ m_mode{};
		mutable FileMode_ m_last_op{};
		mutable Error_ m_last_error{};
		String m_path{}, m_path_abs{}, m_path_save{};

		void _check_errors() const;

	public:
		FileAccessWindows() noexcept = default;
		virtual ~FileAccessWindows() noexcept override;
		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual FileAccessWindows & flush() override;
		virtual FileAccessWindows & close() override;
		virtual bool file_exists(String const & path) override;
		virtual bool is_open() const override;
		virtual String get_path() const override;
		virtual String get_path_abs() const override;
		virtual FileAccessWindows & seek(u64 position) override;
		virtual FileAccessWindows & seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual u8 read_8() const override;
		virtual FileAccessWindows & write_8(u8 value) override;

		static void initialize();
		static void finalize();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PNU_FILE_ACCESS_WINDOWS_HPP_
