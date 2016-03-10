 #include "oglRender.h"
#include <memory>
#include "windowInfo.h"

namespace byhj
{

	OGLRender::OGLRender()
	{
	}

	OGLRender::~OGLRender()
	{

	}
	
	void OGLRender::v_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		m_camera.key_callback(window, key, scancode, action, mode);
	}

	void OGLRender::v_movement(GLFWwindow *window)
	{
		m_camera.movement(window);
	}

	void OGLRender::v_mouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		m_camera.mouse_callback(window, xpos, ypos);
	}

	void OGLRender::v_scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_camera.scroll_callback(window, xoffset, yoffset);
	}


	void OGLRender::v_init()
	{
		glEnable(GL_DEPTH_TEST);

		init_fbo();
		m_cube.init();
		m_plane.Init();

	}

	void OGLRender::v_update()
	{
		static GLfloat lastFrame = static_cast<float> (glfwGetTime());
		GLfloat currentFrame = static_cast<float> (glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		m_camera.update(deltaTime);

	}

    void OGLRender::v_render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_cube.render( m_camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_plane.Render( colorBuffer);

    }
	void OGLRender::v_shutdown()
	{
	}

	void OGLRender::init_fbo()
	{
		glGenFramebuffers(1, &fbo);

		auto sw = WindowInfo::getInstance()->getWidth();
		auto sh = WindowInfo::getInstance()->getHeight();

		//Create floating point color buffer
		glGenBuffers(1, &colorBuffer);
		glBindTexture(GL_TEXTURE_2D, colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, sw, sh, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLuint rboDepth;
		//Create depth buffer(render buffer)
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, sw, sh);

		//Attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Frame buffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}