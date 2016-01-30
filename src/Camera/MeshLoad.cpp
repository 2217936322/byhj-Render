#include "MeshLoad.h"

namespace byhj
{
	void MeshLoad::Init(int sw, int sh)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		m_Aspect = static_cast<GLfloat>(sw) / sh;

		m_LightGui.v_init(sw, sh);
		m_RotationGui.v_init(sw, sh);

		init_buffer();
		init_shader();
	}

	void MeshLoad::Update()
	{

		glUseProgram(m_Program);

		m_LightGui.v_update();
		m_RotationGui.v_update();

	    GLfloat time = glfwGetTime();

		glm::mat4 model = m_RotationGui.getRotationMat(); 
		glm::mat4 view  = glm::lookAt(glm::vec3(0.0f, 0.2f, 5.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) );
		glm::mat4 proj  = glm::perspective(45.0f, m_Aspect, 0.1f, 1000.0f);

		glUniformMatrix4fv(uniform_loc.model, 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(uniform_loc.view, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(uniform_loc.proj, 1, GL_FALSE, &proj[0][0]);
		glUniform3fv(uniform_loc.ambient, 1, &m_LightGui.getLight(0)[0]);

		glUseProgram(0);
	}

	void MeshLoad::Render()
	{
		glUseProgram(m_Program);

		m_RotationGui.v_render();
		m_LightGui.v_render();
		m_Model.draw(m_Program);

		glUseProgram(0);
	}

	void MeshLoad::Shutdown()
	{
	}

	void MeshLoad::init_buffer()
	{
		m_Model.loadModel("armadillo.obj", LoadType::OGL);
	}


	void MeshLoad::init_shader()
	{
		m_MeshLoadShader.init();
		m_MeshLoadShader.attach(GL_VERTEX_SHADER, "model.vert");
		m_MeshLoadShader.attach(GL_FRAGMENT_SHADER, "model.frag");
		m_MeshLoadShader.link();
		m_MeshLoadShader.info();

		m_Program = m_MeshLoadShader.getProgram();
		uniform_loc.model = glGetUniformLocation(m_Program, "u_Model");
		uniform_loc.view  = glGetUniformLocation(m_Program, "u_View");
		uniform_loc.proj  = glGetUniformLocation(m_Program, "u_Proj");
		uniform_loc.ambient = glGetUniformLocation(m_Program, "u_ambient");
		
	}
}