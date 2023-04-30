#ifndef _PN_FILE_ACCESS_ZIP_HPP_
#define _PN_FILE_ACCESS_ZIP_HPP_

#include <core/io/file_access_pack.hpp>

#include <minizip/unzip.h>

namespace pn
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// zip archive
	class PN_API ZipArchive : public PackSource
	{
	public:
		struct File {
			i32 package{ -1 };
			unz_file_pos file_pos{};
		};

		struct Package {
			String path{};
			unzFile m_zfile{};
		};
		
	private:
		static ZipArchive * __singleton;
		Vector<Package> m_packages{};
		HashMap<String, File> m_files{};

	public:
		ZipArchive();
		~ZipArchive();
		static ZipArchive * get_singleton();

		void close_handle(unzFile file) const;
		NODISCARD unzFile get_file_handle(String const & path) const;
		NODISCARD bool file_exists(String const & path) const;
		NODISCARD virtual bool try_open_pack(String const & path, bool replace_files, u64 offset) override;
		NODISCARD virtual Ref<FileAccess> get_file(String const & path, PackedData::PackedFile * file) override;
	};

	SINGLETON_WRAPPER(ZipArchive, get_zip_archive);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// file access zip
	class PN_API FileAccessZip : public FileAccess
	{
		DEFINE_CLASS(FileAccessZip, FileAccess);

		unzFile			m_zfile{};
		unz_file_info64	m_info{};
		mutable bool	m_eof{};

	public:
		FileAccessZip(String const & path, PackedData::PackedFile const & file);
		virtual ~FileAccessZip() override;
		virtual Error_ open_internal(String const & path, FileMode_ mode) override;
		virtual FileAccessZip & close() override;
		virtual FileAccessZip & flush() override;
		virtual bool exists(String const & path) override;
		virtual bool is_open() const override;
		virtual FileAccessZip & seek(u64 position) override;
		virtual FileAccessZip & seek_end(i64 position) override;
		virtual u64 get_position() const override;
		virtual u64 get_length() const override;
		virtual bool eof_reached() const override;
		virtual Error_ get_error() const override;
		virtual String get_path() const override;
		virtual String get_path_abs() const override;
		virtual u8 read_8() const override;
		virtual size_t read_buffer(u8 * data, size_t const size) const override;
		virtual FileAccessZip & write_8(u8) override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_PN_FILE_ACCESS_ZIP_HPP_
