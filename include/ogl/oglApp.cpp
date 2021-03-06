#include "oglApp.h"

namespace byhj
{

std::shared_ptr<OGLApp>  OGLApp::app = nullptr;

void OGLApp::v_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	OGLEulerCamera::getInstance()->key_callback(window, key, scancode, action, mode);
}

void OGLApp::v_movement(GLFWwindow *window) 
{
	OGLEulerCamera::getInstance()->movement(window);
}

void OGLApp::v_mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	//OGLSphereCamera::getInstance()->mouse_callback(window, xpos, ypos);
	OGLEulerCamera::getInstance()->mouse_callback(window, xpos, ypos);
}

void OGLApp::v_scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
{

	//OGLSphereCamera::getInstance()->scroll_callback(window, xoffset, yoffset);
	OGLEulerCamera::getInstance()->scroll_callback(window, xoffset, yoffset);
}
	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

#ifdef USE_ANT
		TwEventKeyGLFW(key, action);
#endif
		app->v_keyCallback(window, key, scancode, action, mode);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_mouse(GLFWwindow* window, double xpos, double ypos)
	{
#ifdef USE_ANT
		TwEventMousePosGLFW(xpos, ypos);
#endif
		app->v_mouseCallback(window, xpos, ypos);
	}

	////////////////////////////////////////////////////////////////////////////////////////

	void OGLApp::glfw_scroll(GLFWwindow* window, double xoffset, double yoffset)
	{
#ifdef USE_ANT
		TwEventMouseWheelGLFW(xoffset);
#endif
		app->v_scrollCallback(window, xoffset, yoffset);
	}

	////////////////////////////////////////////////////////////////////////////////////////
	void OGLApp::glfw_mouseButton(GLFWwindow *window, int x, int y, int z)
	{
		//OGLSphereCamera::getInstance()->mouseButton_callback(window, x, y, z);
		TwEventMouseButtonGLFW(x, y);
	}

	void OGLApp::glfw_char(GLFWwindow *window, unsigned int x)
	{
		TwEventCharGLFW(x, GLFW_PRESS);
	}

	void OGLApp::glfw_resize(GLFWwindow* window, int width, int height)
	{
		WindowInfo::getInstance()->setWidth(width);
		WindowInfo::getInstance()->setHeight(height);
		app->resizeCallback(window, width, height);
	}

	void OGLApp::resizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	////////////////////////////////////////////////////////////////////////////////////////

void OGLApp::v_run()
{	
	app = std::make_shared<OGLApp>(*this);

	std::cout << "Starting GLFW context" << std::endl;
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}
	sw = WindowInfo::getInstance()->getWidth();
	sh = WindowInfo::getInstance()->getHeight();

	int MonitorCount;
	GLFWmonitor ** monitors = glfwGetMonitors(&MonitorCount);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
	//glfwGetPrimaryMonitor(), 
	pWindow = glfwCreateWindow(sw, sh, WindowInfo::getInstance()->getTitle().c_str(), nullptr, nullptr);
	glfwSetWindowPos(pWindow, WindowInfo::getInstance()->getPosX() - 100, WindowInfo::getInstance()->getPosY() - 100);
	glfwMakeContextCurrent(pWindow);

	glfwSetCursorPosCallback(pWindow, glfw_mouse);          // - Directly redirect GLFW mouse position events to AntTweakBar
	glfwSetScrollCallback(pWindow, glfw_scroll);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
	glfwSetKeyCallback(pWindow, glfw_key);                         // - Directly redirect GLFW key events to AntTweakBar
#ifdef USE_ANT
	glfwSetMouseButtonCallback(pWindow, glfw_mouseButton); // - Directly redirect GLFW mouse button events to AntTweakBar
	glfwSetCharCallback(pWindow, glfw_char);                      // - Directly redirect GLFW char events to AntTweakBar
#endif
	glfwSetWindowSizeCallback(pWindow, glfw_resize);


	//glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GL_TRUE);
	// GLFW Options
    //glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (pWindow == NULL) {
		std::cerr << "Failed to create GLFW pWindow" << std::endl;
		glfwTerminate();
		return;
	}
	glewExperimental = GL_TRUE;

	//Check the GLSL and OpenGL status 
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return;
	}
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	m_GLRenderer = (const char *)renderer;
	m_GLVersion  = (const char *)version;
	m_GLSLVersion = (const char *)glslVersion;

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	std::cout << "GL Vendor    :" << vendor << std::endl;
	std::cout << "GL Renderer  : " << renderer << std::endl;
	std::cout << "GL Version (std::string)  : " << version << std::endl;
	std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;
	std::cout << "GLSL Version : " << glslVersion << std::endl;
	std::cout << "--------------------------------------------------------------------------------"
		<< std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

#ifdef USE_FONT
	m_pFont.init();
#endif
#ifdef USE_CEGUI
	OGLCEGUI::getInstance()->init();
	OGLCEGUI::getInstance()->setupCallbacks(pWindow);
#endif
	v_init();


	while (!glfwWindowShouldClose(pWindow))
	{
		glfwPollEvents();
		v_movement(pWindow);

		countFps();

		static GLfloat lastFrame = static_cast<float>(glfwGetTime());
		GLfloat currentFrame = static_cast<float>(glfwGetTime());
		GLfloat deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		v_update();
		v_render();

#ifdef USE_FONT
		glDisable(GL_DEPTH_TEST);
		m_pFont.render("Graphics card: " + m_GLRenderer, 10, sh - 40);
		m_pFont.render("GL Version: " + m_GLVersion, 10, sh - 70);
		m_pFont.render("GLSL Version: " + m_GLSLVersion, 10, sh - 100);
		m_pFont.render("FPS: " + std::to_string(m_fps), 10, 30);
		//glEnable(GL_DEPTH_TEST);
#endif

#ifdef USE_CEGUI
		 OGLCEGUI::getInstance()->render();
#endif
		glfwSwapBuffers(pWindow);
	}

	v_shutdown();

	glfwTerminate();
}



void OGLApp::countFps()
{
	static float currTime = glfwGetTime();
	static float lastTime = glfwGetTime();
	static float Time = 0.0f;

	currTime = glfwGetTime();
	Time += currTime - lastTime;

	static int cnt = 0;
	if (Time >= 1.0f)
	{
		m_fps = cnt;
		cnt = 0;
		Time = 0.0f;
	}
	++cnt;
	lastTime = currTime;
}

}