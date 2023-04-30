#ifndef _PNU_COPYMEM_HPP_
#define _PNU_COPYMEM_HPP_

#include <core/typedefs.hpp>

#include <string.h>

#define copymem(dst, src, size) (std::memcpy(dst, src, size))

#define movemem(dst, src, size) (std::memmove(dst, src, size))

#define zeromem(dst, size) (std::memset(dst, 0, size))

#endif // !_PNU_COPYMEM_HPP_
