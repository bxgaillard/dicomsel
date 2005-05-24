#if defined( _MSC_VER )
// The Visual C++ debugger doesn't know string
// larger than 255 char. STL often creates large
// symbol. The following line cancels warning
// created by Visual.
#pragma warning (disable : 4786)
// The following line prevents from this file to
// be included several times by the compiler with
// Visual C++. This makes compiling quicker, but
// must not be used when your file content may
// change depending on some macro definitions. In
// the general case, leave these lines as they
// are.
#pragma once
// assert is not know by Visual (it's the ASSERT
// macro which is used).
#include <assert.h>
#else
#include <typeinfo>
#endif

#ifdef __STDC__
typedef signed char int8;       // NB: non-ANSI compilers may not grok
#else
typedef char int8;
#endif
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;  // sizeof (uint16) must == 2
#if defined(__alpha) || (defined(_MIPS_SZLONG) && _MIPS_SZLONG == 64)
typedef int int32;
typedef unsigned int uint32;    // sizeof (uint32) must == 4
#else
typedef long int32;
typedef unsigned long uint32;   // sizeof (uint32) must == 4
#endif
