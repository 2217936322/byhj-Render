#include "oglRender.h"

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

	}

	void OGLRender::v_update()
	{

	}

    void OGLRender::v_render()
	{
		static const GLfloat black[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
		static const GLfloat one[] ={ 1.0f };
		glClearBufferfv(GL_DEPTH, 0, one);


    }
	void OGLRender::v_shutdown()
	{

	}

}
