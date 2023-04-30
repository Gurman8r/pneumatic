#include <scene/resources/text_file.hpp>
#include <servers/text_server.hpp>
#include <fstream>

namespace Pnu
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(TextFile, t)
	{
		t.tp_bind = BIND_CLASS(TextFile, t)
		{
			return t
				.def("get_text", &TextFile::get_text)
				.def("has_text", &TextFile::has_text)
				.def("set_text", &TextFile::set_text)
				;
		};
	}

	TextFile::TextFile(String const & path)
	{
		if (path.empty()) { return; }
		std::ifstream file{ path.c_str(), std::ios_base::binary };
		ON_SCOPE_EXIT(&) { file.close(); };
		if (!file) { return; }
		set_path(path);
		file.seekg(0, std::ios_base::end);
		if (std::streampos size{ file.tellg() }; 0 < size) {
			file.seekg(0, std::ios_base::beg);
			m_text.resize((size_t)size);
			file.read((char *)m_text.data(), size);
		}
	}

	String const & TextFile::get_text() const
	{
		return m_text;
	}

	bool TextFile::has_text(String const & compare_to) const
	{
		return compare_to.empty() ? m_text.empty() : (m_text == compare_to);
	}

	void TextFile::set_text(String const & value)
	{
		m_text = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}