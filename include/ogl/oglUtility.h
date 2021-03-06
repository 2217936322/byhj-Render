#ifndef UTILITY_H
#define UTILITY_H

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>

namespace byhj
{

	struct OGLMVPMatrix
	{
	   glm::mat4 model;
	   glm::mat4 view;
	   glm::mat4 proj;
	};


	const GLuint OGLVALUE = 0xffffffff;
}

#define ARRAY_ELEMENTS(a) ( sizeof(a)/sizeof(a[0]) )
#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }
#define GL_BUFFER_OFFSET(offset)  (GLvoid*)(NULL + offset) 

#endif
