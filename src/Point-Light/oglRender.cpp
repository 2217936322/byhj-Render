 #include "oglRender.h"
#include <memory>


namespace byhj
{

	OGLRender::OGLRender()
	{
	}

	OGLRender::~OGLRender()
	{

	}


	void OGLRender::v_init()
	{
		m_plane.init();
	}

	void OGLRender::v_update()
	{
		m_plane.update();
	}

	void OGLRender::v_render()
	{
		glEnable(GL_DEPTH_TEST);


		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);

		m_plane.render();

	}
	void OGLRender::v_shutdown()
	{
		m_plane.shutdown();
	}
}