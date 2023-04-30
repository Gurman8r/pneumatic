#ifndef _PNU_VERSION_HPP_
#define _PNU_VERSION_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif

#ifndef VERSION_MINOR
#define VERSION_MINOR 0
#endif

#ifndef VERSION_PATCH
#define VERSION_PATCH 0
#endif

#ifndef VERSION_BUILD
#define VERSION_BUILD 0
#endif

#ifndef VERSION_NAME
#define VERSION_NAME "pneumatic-engine"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define VERSION_BRANCH "" VERSION_MAJOR "." VERSION_MINOR

#if VERSION_PATCH
#define VERSION_NUMBER "" VERSION_BRANCH "." VERSION_PATCH
#else
#define VERSION_NUMBER "" VERSION_BRANCH
#endif

#define VERSION_HEX 0x10000 * VERSION_MAJOR + 0x100 * VERSION_MINOR + VERSION_PATCH

#define VERSION_FULL_CONFIG "" VERSION_NUMBER

#define VERSION_FULL_BUILD "" VERSION_FULL_CONFIG "." VERSION_BUILD

#define VERSION_FULL_NAME "" VERSION_NAME " v" VERSION_FULL_BUILDS

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_PNU_VERSION_HPP_
