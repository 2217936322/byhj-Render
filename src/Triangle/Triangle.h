#ifndef Triangle_H
#define Triangle_H

#include <gl/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include "ogl/oglShader.h"
#include "utility.h"

namespace byhj
{
	class Triangle 
	{
	public:

	Triangle() ;
	~Triangle();

	void init();
	void update();
	void render();
	void shutdown();

	private:

		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint m_program  = OGL_ONE;
		GLuint m_vao  = OGL_ONE;
		GLuint m_ibo  = OGL_ONE;
		GLuint m_vbo  = OGL_ONE;
		OGLShader m_TriangleShader = "Triangle Shader";
	};
}
#endif