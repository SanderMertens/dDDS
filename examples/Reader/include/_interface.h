/* _interface.h
 *
 * This file contains generated code. Do not modify!
 */

#if BUILDING_READER && defined _MSC_VER
#define READER_DLL_EXPORTED __declspec(dllexport)
#elif BUILDING_READER
#define READER_EXPORT __attribute__((__visibility__("default")))
#elif defined _MSC_VER
#define READER_EXPORT __declspec(dllimport)
#else
#define READER_EXPORT
#endif

