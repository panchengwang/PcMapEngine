
#ifndef MAP_ENGIN_DLL_EXPORT_H
#define MAP_ENGIN_DLL_EXPORT_H




#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define API_EXPORT __declspec(dllexport)
#  define API_IMPORT __declspec(dllimport)
#else
#  define API_EXPORT     __attribute__((visibility("default")))
#  define API_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(AS_LIBRARY)
#  define DLL_EXPORT API_EXPORT
#else
#  define DLL_EXPORT API_IMPORT
#endif



#endif 