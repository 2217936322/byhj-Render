#include "OGLRender.h"
#include "windowInfo.h"
#include "ogl/oglEulerCamera.h"

const static glm::vec3 g_LightPos(1.5f, 0.5f, 0.0f);

namespace byhj
{

	void OGLRender::v_init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_Cube.init();
		m_Plane.init();
	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float>(glfwGetTime());
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		OGLEulerCamera::getInstance()->update(deltaTime);
	}

	void OGLRender::v_render()
	{
		static const float bgColor[4] = {0.2f, 0.4f, 0.5f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);
		static const float one = 1.0f;
		glClearBufferfv(GL_DEPTH, 0, &one);

		static OGLMVPMatrix matrix;
		matrix.view  = OGLEulerCamera::getInstance()->getViewMat();
		matrix.proj  = glm::perspective(1.0f,  WindowInfo::getInstance()->getAspect(), 0.1f, 1000.0f);
		matrix.model = glm::mat4(1.0f);

		m_Cube.render(matrix);
		m_Plane.render(matrix);
	
	}

	void OGLRender::v_shutdown()
	{
		m_Cube.shutdown();
		m_Plane.shutdown();
	}


}