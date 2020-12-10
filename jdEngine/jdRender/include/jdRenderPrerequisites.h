#pragma once
#include <jdPrerequisitesUtil.h>

/*****************************************************************************/
/**
 * Library export specifics
 */
 /*****************************************************************************/
#if JD_PLATFORM == JD_PLATFORM_WIN32
# if JD_COMPILER == JD_COMPILER_MSVC
#   if defined ( JD_STATIC_LIB )
#     define JD_UTILITY_EXPORT
#   else
#     if defined ( JD_RENDER_EXPORTS )
#       define JD_RENDER_EXPORT __declspec( dllexport )
#     else
#       define JD_RENDER_EXPORT __declspec( dllimport )
#     endif
#   endif
#else // Any other compiler
#  if defined( JD_STATIC_LIB )
#    define JD_UTILITY_EXPORT
#  else 
#    if defined( JD_RENDER_EXPORTS )
#       define JD_RENDER_EXPORT __attribute__ (( dllexport ))
#     else
#       define JD_RENDER_EXPORT __attribute__ (( dllimport ))
#     endif
#   endif
#endif
# define JD_UTILITY_HIDDEN
#else // Linux/Mac settings
# define JD_RENDER_EXPORT __attribute__ ((visIbility ("default")))
# define JD_RENDER_HIDDEN __attribute__ ((visibility ("hidden")))
#endif

#define SAFE_RELEASE(x){if (x) {x->Release(); x = nullptr;}}
