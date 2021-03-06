#ifndef Utility_H
#define Utility_H

#include <string>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <DirectXMath.h>
#include <iostream>
#include <random>

using namespace DirectX;

namespace byhj
{

	// Random number generator
	static unsigned int seed = 0x13371337;

	static inline float random_float() //�����
	{
		float res;
		unsigned int tmp;

		seed *= 16807;
		tmp = seed ^ (seed >> 4) ^ (seed << 15);
		*((unsigned int *)&res) = (tmp >> 9) | 0x3F800000;

		return (res - 1.0f);
	}

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])
#define SAFE_DELETE(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }


const GLuint OGL_ZERO = 0x00000000;
const GLuint OGL_ONE  = 0xffffffff;

enum  FontType {
	OGL_FT_FONT,
	D3D_FW_FONT,  // FW1FontWrapper
	D3D_SP_FONT,  // SpriteFont
	D3D_FT_FONT  // Freetype
};

bool checkOGLUniform(GLuint loc, std::string desc, int line, char* file, bool ignore);

#ifdef _DEBUG
#define CHECK_OGL_UNIFORM(loc, description) \
     { static bool ignoreAlways = false; \
        if (!ignoreAlways) \
          if (checkOGLUniform(loc, description, __LINE__, __FILE__, &ignoreAlways)) \
             {_asm{int 3} } \
	 }
#else
#define assert(expression && description)
#endif


}
#endif