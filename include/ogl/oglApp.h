#ifndef OGLAPP_H
#define OGLAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <memory>

#include "app.h"
#include "oglRender.h"
#include "oglFont.h"

#ifdef _WIN32
#include <windows.h>
const int ScreenWidth  = static_cast<int>( GetSystemMetrics(SM_CXSCREEN) )  * 0.8;
const int ScreenHeight = static_cast<int>(  GetSystemMetrics(SM_CYSCREEN) ) * 0.8;

const int PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth);
const int PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight);

#else
const int ScreenWidth = 1200;
const int ScreenHeight = 1000;
const int PosX = 300;
const int PosY = 100;
#endif

namespace byhj 
{

class OGLApp : public App
{
    private:

	public:
		OGLApp() = default;
		~OGLApp() = default;

	public:

		void setRender(Render *pRender);
		void setGui(Gui *pGui);
		void setFont(OGLFont *pFont);

protected:
	Render*  m_pRender = nullptr;
	Gui  * m_pGui = nullptr;
	OGLFont *m_pFont = new OGLFont;

		void v_run() override;
		void v_end() override;

		virtual void v_KeyCallback(GLFWwindow* Triangle, int key, int scancode, int action, int mode) 
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(Triangle, GL_TRUE);
			if (key ==  GLFW_KEY_C && action == GLFW_PRESS)
				glfwSetInputMode(Triangle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		virtual void v_Movement(GLFWwindow *Triangle) {}
		virtual void v_MouseCallback(GLFWwindow* Triangle, double xpos, double ypos) {}
		virtual void v_ScrollCallback(GLFWwindow* Triangle, double xoffset, double yoffset) {}
		
		int   getSW() const ;
		int   getSH() const ;
		void countFps();
	protected:
		struct WindowInfo
		{
			WindowInfo():title("OpenGL-Learning:"), 
				         Width(ScreenWidth), 
						 Height(ScreenHeight),
				         posX(PosX), 
					     posY(PosY) {}

			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;

		std::string m_GLRenderer;
		std::string m_GLVersion;
		std::string m_GLSLVersion;
		GLuint m_fps = 0;

		float GetAspect() const ;
	};  //class

}


#endif  //