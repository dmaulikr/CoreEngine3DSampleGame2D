#include "SharedResources.h"

//** INCLUDES **/
#if defined (PLATFORM_OSX)
#include <OpenGL/gl.h>
#endif

#if defined (PLATFORM_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>
#include "glew/glew.h"
#endif

#ifdef PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

//TEXTURES

//SOUNDS
