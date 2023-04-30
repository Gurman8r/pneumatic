#include <core/io/file_access_zip.hpp>

namespace Pnu
{
	struct ZipData { Ref<FileAccess> f{}; };

	static void * ism_open(voidpf opaque, cstring path, i32 mode) {
		if (mode & ZLIB_FILEFUNC_MODE_WRITE) {
			return nullptr;
		}
		auto f{ FileAccess::open(path, FileMode_Read) };
		ASSERT(f);
		ZipData * zd{ memnew(ZipData) };
		zd->f = f;
		return zd;
	}

	static uLong ism_read(voidpf opaque, voidpf stream, voidpf buf, uLong size) {
		ZipData * zd{ (ZipData *)stream };
		zd->f->read_buffer((u8 *)buf, size);
		return size;
	}

	static uLong ism_write(voidpf opaque, voidpf stream, void const * buf, uLong size) {
		return 0;
	}

	static long ism_tell(voidpf opaque, voidpf stream) {
		ZipData * zd{ (ZipData *)stream };
		return (long)zd->f->get_position();
	}

	static long ism_seek(voidpf opaque, voidpf stream, uLong offset, i32 origin) {
		ZipData * zd{ (ZipData *)stream };
		u64 pos{ offset };
		switch (origin) {
		default: break;
		case ZLIB_FILEFUNC_SEEK_CUR: { pos = zd->f->get_position() + offset; } break;
		case ZLIB_FILEFUNC_SEEK_END: { pos = zd->f->get_length() + offset; } break;
		};
		zd->f->seek(pos);
		return 0;
	}

	static i32 ism_close(voidpf opaque, voidpf stream) {
		ZipData * zd{ (ZipData *)stream };
		memdelete(zd);
		return 0;
	}

	static i32 ism_testerror(voidpf opaque, voidpf stream) {
		ZipData * zd{ (ZipData *)stream };
		return zd->f->get_error() != Error_OK ? 1 : 0;
	}

	static voidpf ism_alloc(voidpf opaque, uInt items, uInt size) {
		return memalloc(items * size);
	}

	static void ism_free(voidpf opaque, voidpf address) {
		memfree(address);
	}
}

namespace Pnu
{
	ZipArchive * ZipArchive::__singleton{};

	ZipArchive::ZipArchive() { SINGLETON_CTOR(); }

	ZipArchive::~ZipArchive()
	{
		SINGLETON_DTOR();
		for (size_t i{}; i < m_packages.size(); ++i) {
			unzClose(m_packages[i].m_zfile);
		}
		m_packages.clear();
	}

	ZipArchive * ZipArchive::get_singleton()
	{
		return BRANCHLESS_IF((!__singleton), (__singleton = memnew(ZipArchive))), __singleton;
	}

	void ZipArchive::close_handle(unzFile file) const
	{
		ASSERT(file);
		unzCloseCurrentFile(file);
		unzClose(file);
	}

	unzFile ZipArchive::get_file_handle(String const & path) const
	{
		ASSERT(file_exists(path));
		File file{ *util::getptr(m_files, path) };

		zlib_filefunc_def io;
		memset(&io, 0, sizeof(io));
		io.opaque = nullptr;
		io.zopen_file = ism_open;
		io.zread_file = ism_read;
		io.zwrite_file = ism_write;
		io.ztell_file = ism_tell;
		io.zseek_file = ism_seek;
		io.zclose_file = ism_close;
		io.zerror_file = ism_testerror;
		io.alloc_mem = ism_alloc;
		io.free_mem = ism_free;

		unzFile pkg{ unzOpen2(m_packages[file.package].path.c_str(), &io) };
		ASSERT(pkg);
		i32 unz_err{ unzGoToFilePos(pkg, &file.file_pos) };
		if (unz_err != UNZ_OK || unzOpenCurrentFile(pkg) != UNZ_OK) {
			unzClose(pkg);
			CRASH("");
		}
		return pkg;
	}

	bool ZipArchive::file_exists(String const & path) const
	{
		return m_files.contains(path);
	}

	bool ZipArchive::try_open_pack(String const & package_path, bool replace_files, u64 offset)
	{
		ASSERT((offset == 0) && "Invalid PCK data. Note that loading files with a non-zero offset isn't supported with ZIP archives.");
		if (package_path.extension() != ".zip") { return false; }
		
		zlib_filefunc_def io;
		memset(&io, 0, sizeof(io));
		io.opaque = nullptr;
		io.zopen_file = ism_open;
		io.zread_file = ism_read;
		io.zwrite_file = ism_write;
		io.ztell_file = ism_tell;
		io.zseek_file = ism_seek;
		io.zclose_file = ism_close;
		io.zerror_file = ism_testerror;
		
		unzFile zfile{ unzOpen2(package_path.c_str(), &io) };
		ASSERT(zfile);
		
		unz_global_info64 gi;
		i32 err{ unzGetGlobalInfo64(zfile, &gi) };
		ASSERT(err == UNZ_OK);
		
		Package pkg{ package_path, zfile };
		m_packages.push_back(pkg);
		i32 pkg_num{ (i32)m_packages.size() - 1 };
		
		for (u64 i{}; i < gi.number_entry; ++i)
		{
			char filename_inzip[260]{};

			unz_file_info64 info;
			err = unzGetCurrentFileInfo64(zfile, &info, filename_inzip, sizeof(filename_inzip), nullptr, 0, nullptr, 0);
			if (err != UNZ_OK) { continue; }
		
			File f{ pkg_num };
			unzGetFilePos(zfile, &f.file_pos);
		
			String const path{ package_path.filename() + "://" + String{filename_inzip} };
			m_files[path] = f;

			get_packed_data()->add_path(package_path, path, 1, 0, path.hash_code(), this, replace_files, false);
		
			if ((i + 1) < gi.number_entry) {
				unzGoToNextFile(zfile);
			}
		}

		return false;
	}

	Ref<FileAccess> ZipArchive::get_file(String const & path, PackedData::PackedFile * file)
	{
		return memnew(FileAccessZip(path, *file));
	}
}

namespace Pnu
{
	EMBED_CLASS(FileAccessZip, t) {}

	FileAccessZip::FileAccessZip(String const & path, PackedData::PackedFile const & file)
	{
		open_internal(path, FileMode_Read);
	}

	FileAccessZip::~FileAccessZip()
	{
		close();
	}

	Error_ FileAccessZip::open_internal(String const & path, FileMode_ mode)
	{
		m_zfile = VALIDATE(get_zip_archive())->get_file_handle(path);
		ASSERT(m_zfile);
		ASSERT(UNZ_OK == unzGetCurrentFileInfo64(m_zfile, &m_info, nullptr, 0, nullptr, 0, nullptr, 0));
		return Error_OK;
	}

	FileAccessZip & FileAccessZip::close()
	{
		if (m_zfile) {
			VALIDATE(get_zip_archive())->close_handle(m_zfile);
			m_zfile = nullptr;
		}
		return (*this);
	}

	FileAccessZip & FileAccessZip::flush()
	{
		CRASH("this should never be called");
		return (*this);
	}

	bool FileAccessZip::exists(String const & path)
	{
		return false;
	}

	bool FileAccessZip::is_open() const
	{
		return m_zfile;
	}

	FileAccessZip & FileAccessZip::seek(u64 position)
	{
		ASSERT(m_zfile);
		unzSeekCurrentFile(m_zfile, (i32)position);
		return (*this);
	}

	FileAccessZip & FileAccessZip::seek_end(i64 position)
	{
		ASSERT(m_zfile);
		unzSeekCurrentFile(m_zfile, (i32)(get_length() + position));
		return (*this);
	}

	u64 FileAccessZip::get_position() const
	{
		ASSERT(m_zfile);
		return unztell(m_zfile);
	}

	u64 FileAccessZip::get_length() const
	{
		ASSERT(m_zfile);
		return m_info.uncompressed_size;
	}

	bool FileAccessZip::eof_reached() const
	{
		ASSERT(m_zfile);
		return m_eof;
	}

	Error_ FileAccessZip::get_error() const
	{
		if (!m_zfile) {
			return Error_Unknown;
		}
		if (eof_reached()) {
			return Error_EOF;
		}
		return Error_OK;
	}

	String FileAccessZip::get_path() const
	{
		return String();
	}

	String FileAccessZip::get_path_abs() const
	{
		return String();
	}

	u8 FileAccessZip::read_8() const
	{
		u8 temp{};
		read_buffer(&temp, 1);
		return temp;
	}

	size_t FileAccessZip::read_buffer(u8 * data, size_t const size) const
	{
		ASSERT(data && size);
		ASSERT(m_zfile);

		m_eof = unzeof(m_zfile);
		if (m_eof) {
			return 0;
		}
		i64 const read{ unzReadCurrentFile(m_zfile, data, (i32)size) };
		ASSERT(-1 < read);
		if ((u64)read < size) {
			m_eof = true;
		}
		return (size_t)read;
	}

	FileAccessZip & FileAccessZip::write_8(u8 value)
	{
		CRASH("this should never be called");
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}