#ifndef _PN_LANGUAGE_FEATURES_HPP_
#define _PN_LANGUAGE_FEATURES_HPP_

#include <core/version.hpp>
#include <core/preprocessor.hpp>
#include <cstddef>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	CONFIGURATION
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DEBUG_ENABLED
#if _DEBUG
//	Debug
#	define DEBUG_ENABLED true
#else
//	Release
#	define DEBUG_ENABLED false
#endif
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	LANGUAGE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __cplusplus
#	if _MSVC_LANG
#		define CXX_LANG _MSVC_LANG
#	else
#		define CXX_LANG __cplusplus
#	endif
#else
#error "this system does not support C++"
#endif

#define CXX_20 201907L
#define CXX_17 201703L
#define CXX_14 201402L
#define CXX_11 201103L

//	C++20
#if (CXX_LANG >= CXX_20)
#define HAS_CXX_20 1
#define HAS_CXX_17 1
#define HAS_CXX_14 1
#define HAS_CXX_11 1

//	C++17
#elif (CXX_LANG >= CXX_17)
#define HAS_CXX_20 0
#define HAS_CXX_17 1
#define HAS_CXX_14 1
#define HAS_CXX_11 1

//	C++14
#elif (CXX_LANG >= CXX_14)
#define HAS_CXX_20 0
#define HAS_CXX_17 0
#define HAS_CXX_14 1
#define HAS_CXX_11 1

//	C++11
#elif (CXX_LANG >= CXX_11)
#define HAS_CXX_20 0
#define HAS_CXX_17 0
#define HAS_CXX_14 0
#define HAS_CXX_11 1

#else
#error "this version of C++ is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	OPERATING get_os()
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _WIN32 || _WIN64 || WIN32 || WIN64 || __MINGW32__ || __MINGW64__
//	Windows
#define SYSTEM_WINDOWS true

#elif __APPLE__ && __MACH__
//	Apple
#define SYSTEM_APPLE true

#elif __unix__
//	Unix
#	define SYSTEM_UNIX true

#	if __ANDROID__
//	Android
#	define SYSTEM_ANDROID true

#	elif __linux__
//	Linux
#	define SYSTEM_LINUX true

#	elif __FreeBSD__ || __FreeBSD_kernel__
//	FreeBSD
#	define SYSTEM_FREEBSD true

#	else
#	error "unsupported unix operating system"
#	endif

#else
#error "unsupported operating system"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	PROCESSOR
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __x86_64__ || _M_X64 || _x64
//	x64
#define CPU_X64 true
#define ARCHITECTURE 64

#elif __i386__ || _M_IX86
//	x86
#define CPU_X86 true
#define ARCHITECTURE 32

#elif __arm__ || _M_ARM || __aarch64__
//	ARM
#	if __aarch64__
#		define CPU_ARM64 true
#		define ARCHITECTURE 64
#	else
#		define CPU_ARM32 true
#		define ARCHITECTURE 32
#	endif

#elif ppc || _M_PPC || __ppc64__
//	PowerPC
#	if __ppc64__
#		define CPU_PPC64 true
#		define ARCHITECTURE 64
#	else
#		define CPU_PPC32 true
#		define ARCHITECTURE 32
#	endif

#else
#error "unsupported cpu"
#endif

#if !(ARCHITECTURE == 32 || ARCHITECTURE == 64)
#error "unsupported architecture"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	COMPILER
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _MSC_VER
//	Visual Studio
#	define CC_MSVC _MSC_VER

#elif __clang__
//	Clang / LLVM
#	define CC_CLANG __clang__

#elif (__GNUC__ || __GNUG__) && !__clang__
//	GCC
#	ifdef __GNUC__
#		define CC_GCC __GNUC__
#	else
#		define CC_GCC __GNUG__
#	endif

#elif __ICC || __INTEL_COMPILER
//	Intel
#	ifdef __ICC
#		define CC_INTEL __ICC
#	else
#		define CC_INTEL __INTEL_COMPILER
#	endif

#elif __MINGW32__ || __MINGW64__
//	MinGW
#	ifdef __MINGW64__
#		define CC_MINGW __MINGW64__
#	else
#		define CC_MINGW __MINGW32__
#	endif

#elif __EMSCRIPTEN__
//	Emscripten
#define CC_EMSCRIPTEN __EMSCRIPTEN__

#elif __asmjs__
//	asm.js
#define CC_ASMJS __asmjs__

#elif __wasm__
//	WebAssembly
#define CC_WASM __wasm__

#else
#error "unsupported compiler"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	ATTRIBUTES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//	nodiscard
#if __has_cpp_attribute(nodiscard) >= 201603L
#	define NODISCARD [[nodiscard]]
#else
#	define NODISCARD
#endif

//	novtable
#if CC_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

//	inlining
#if CC_MSVC
#	define FORCE_INLINE __forceinline
#	define NEVER_INLINE __declspec(noinline)

#elif CC_CLANG || CC_GCC
#	define FORCE_INLINE __attribute__((always_inline))
#	define NEVER_INLINE __attribute__((noinline))

#else
#	define FORCE_INLINE
#	define NEVER_INLINE
#endif

//	likely
#if __has_cpp_attribute(likely) >= 201907L
#	define LIKELY(expr) ((expr)) [[likely]]
#else
#	define LIKELY(expr) ((expr))
#endif

//	unlikely
#if __has_cpp_attribute(unlikely) >= 201907L
#	define UNLIKELY(expr) ((expr)) [[unlikely]]
#else
#	define UNLIKELY(expr) ((expr))
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	API
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if STATIC_BUILD_ENABLED && DYNAMIC_BUILD_ENABLED
#	error "static and dynamic build are mutually exclusive"
#endif

#if STATIC_BUILD_ENABLED
#	define PN_API_EXPORT
#	define PN_API_IMPORT

#elif SYSTEM_WINDOWS
#	define PN_API_EXPORT __declspec(dllexport)
#	define PN_API_IMPORT __declspec(dllimport)

#elif CC_CLANG || (__GNUC__ && (__GNUC__ >= 4))
#	define PN_API_EXPORT __attribute__((visibility("default")))
#	define PN_API_IMPORT __attribute__((visibility("hidden")))

#else
#	define PN_API_EXPORT
#	define PN_API_IMPORT
#endif

#ifndef PN_API
#	if DYNAMIC_BUILD_ENABLED
#		define PN_API PN_API_EXPORT
#	else
#		define PN_API PN_API_IMPORT
#	endif
#endif

#define PN_API_FUNC(rtype) PN_API rtype

#define PN_API_DATA(rtype) PN_API extern rtype


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	STL FEATURES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _HAS_FUNCTION_ALLOCATOR_SUPPORT
#define _HAS_FUNCTION_ALLOCATOR_SUPPORT true
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	DISABLE ANNOYING COMPILER WARNINGS
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if SYSTEM_WINDOWS
#	pragma warning(disable: 4031)	//	second formal parameter list longer than the first list
#	pragma warning(disable: 4067)	//	unexpected tokens following preprocessor directive - expected a newline
#	pragma warning(disable: 4251)	//	type1 needs to have dll-interface to be used by type2
#	pragma warning(disable: 4307)	//	integral constant overflow
#	pragma warning(disable: 4308)	//	negative integral constant converted to unsigned t
#	pragma warning(disable: 4309)	//	truncation of constant value
#	pragma warning(disable: 4312)	//	conversion to greater size
#	pragma warning(disable: 4723)	//	potential divide by zero
#	pragma warning(disable: 6011)	//	dereferencing NULL pointer
#	pragma warning(disable: 6282)	//	incorrect operator
#	pragma warning(disable: 26437)	//	do not slice
#	pragma warning(disable: 26444)	//	avoid unnamed objecs with custom construction and destruction
#	pragma warning(disable: 26451)	//	arithmetic overflow
#	pragma warning(disable: 26495)	//	value may be finalized
#	pragma warning(disable: 26498)	//	mark as constexpr if desired
#	pragma warning(disable: 26812)	//	unscoped enum
#	pragma warning(disable: 28251)	//	inconsistent annotations
#	pragma warning(disable: 33101)	//	unchecked tolower bound for enum type used as index
#endif

#endif //	!_PN_LANGUAGE_FEATURES_HPP_
