#ifndef _PNU_TEXT_FILE_HPP_
#define _PNU_TEXT_FILE_HPP_

#include <core/io/resource.hpp>

namespace Pnu
{
	class PNU_API TextFile : public Resource
	{
		DEFINE_CLASS(TextFile, Resource);

		String m_text{};

	public:
		TextFile() noexcept {}
		explicit TextFile(String const & path);
		virtual String const & get_text() const;
		virtual bool has_text(String const & compare_to = {}) const;
		virtual void set_text(String const & value);
	};
}

#endif // !_PNU_TEXT_FILE_HPP_
