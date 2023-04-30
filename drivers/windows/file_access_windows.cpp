#include <drivers/windows/file_access_windows.hpp>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccessWindows::_check_errors() const
	{
		ASSERT(m_file);
		if (feof(m_file)) {
			m_last_error = Error_EOF;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(FileAccessWindows, t) {}

	FileAccessWindows::~FileAccessWindows() noexcept { FileAccessWindows::close(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ FileAccessWindows::open_internal(String const & path, FileMode_ mode)
	{
		FileAccessWindows::close();

		constexpr cstring m_mode[4]{ "rb", "wb", "rb+", "wb+" };

		m_file = fopen(path.c_str(), m_mode[mode]);

		if (!m_file) {
			return Error_Unknown;
		}

		return Error_OK;
	}

	FileAccessWindows & FileAccessWindows::flush()
	{
		ASSERT(m_file);
		fflush(m_file);
		if (m_last_op == FileMode_Write) {
			m_last_op = FileMode_MAX;
		}
		return (*this);
	}

	FileAccessWindows & FileAccessWindows::close()
	{
		if (m_file) {
			fclose(m_file);
			m_file = nullptr;
		}
		return (*this);
	}

	bool FileAccessWindows::exists(String const & path)
	{
		FILE * const fp{ fopen(path.c_str(), "rb") };
		ON_SCOPE_EXIT(&fp) { fclose(fp); };
		return (bool)fp;
	}

	bool FileAccessWindows::is_open() const
	{
		return m_file != nullptr;
	}

	String FileAccessWindows::get_path() const
	{
		return m_path;
	}

	String FileAccessWindows::get_path_abs() const
	{
		return m_path_abs;
	}

	FileAccessWindows & FileAccessWindows::seek(u64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_SET)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
		return (*this);
	}

	FileAccessWindows & FileAccessWindows::seek_end(i64 position)
	{
		ASSERT(m_file);
		m_last_error = Error_OK;
		if (_fseeki64(m_file, position, SEEK_END)) {
			_check_errors();
		}
		m_last_op = FileMode_MAX;
		return (*this);
	}

	u64 FileAccessWindows::get_position() const
	{
		i64 const pos{ _ftelli64(m_file) };
		if (pos < 0) {
			_check_errors();
		}
		return pos;
	}

	u64 FileAccessWindows::get_length() const
	{
		ASSERT(m_file);
		u64 const pos{ get_position() };
		_fseeki64(m_file, 0, SEEK_END);
		u64 const size{ get_position() };
		_fseeki64(m_file, pos, SEEK_SET);
		return size;
	}

	bool FileAccessWindows::eof_reached() const
	{
		_check_errors();
		return m_last_error == Error_EOF;
	}

	Error_ FileAccessWindows::get_error() const
	{
		return m_last_error;
	}

	u8 FileAccessWindows::read_8() const
	{
		ASSERT(m_file);
		if (m_mode == FileMode_Write || m_mode == FileMode_Read) {
			if (m_last_op == FileMode_Write) {
				fflush(m_file);
			}
			m_last_op = FileMode_Read;
		}
		u8 b;
		if (fread(&b, 1, 1, m_file) == 0) {
			_check_errors();
			b = '\0';
		}
		return b;
	}

	FileAccessWindows & FileAccessWindows::write_8(u8 value)
	{
		ASSERT(m_file);
		if (m_mode == FileMode_ReadWrite || m_mode == FileMode_WriteRead) {
			if (m_last_op == FileMode_Read) {
				if (m_last_error != Error_EOF) {
					fseek(m_file, 0, SEEK_CUR);
				}
			}
			m_last_op = FileMode_Write;
		}
		fwrite(&value, 1, 1, m_file);
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void FileAccessWindows::initialize()
	{
		FileAccess::__create_func[FileAccessType_Resources] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
		FileAccess::__create_func[FileAccessType_User] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
		FileAccess::__create_func[FileAccessType_Filesystem] = []() -> Ref<FileAccess> { return memnew(FileAccessWindows); };
	}

	void FileAccessWindows::finalize()
	{
		FileAccess::__create_func[FileAccessType_Resources] = nullptr;
		FileAccess::__create_func[FileAccessType_User] = nullptr;
		FileAccess::__create_func[FileAccessType_Filesystem] = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}