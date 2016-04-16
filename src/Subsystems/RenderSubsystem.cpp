#include "RenderSubsystem.h"

#include "Log.h"
#include "Camera.h"
#include "Scene.h"
#include "RenderShader.h"


RenderSubsystem::RenderSubsystem()
{	
	m_initialized = false;	
}

RenderSubsystem::~RenderSubsystem()
{	
	delete m_shader;
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	if(!m_initialized)
	{
		m_window = window;
		m_shader = new RenderShader();		

	  if( ! m_shader->load("shaders/rendershaderV1") )
			return false;

		Engine::getLog()->log("RenderSubsystem", "started");
		m_initialized = true;
		return true;
	}
	return false;
}
bool RenderSubsystem::shutDown()
{
	if(m_initialized)
	{
		Engine::getLog()->log("RenderSubsystem", "shut down");
		return true;
	}
	return false;
}

void RenderSubsystem::startRender()
{
	if(m_initialized)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
 	 	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	 	glEnable(GL_DEPTH_TEST);
 	 	
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

 	 	m_shader->use();
 	 	m_shader->setRenderPass(1);
 	 	Camera * cam = Engine::getScene()->getCamera();
 	 	m_shader->setProjectionMatrix(cam->getProjection());
 	 	m_shader->setViewMatrix(cam->getView());

    glFinish();
	}
}

void RenderSubsystem::renderPassOne()
{

}
void RenderSubsystem::renderPassTwo()
{
	
}

void RenderSubsystem::render()
{
	startRender();
	Scene * scene = Engine::getScene();

	if(scene != nullptr)
		scene->render(m_shader);

	endRender();
}

void RenderSubsystem::endRender()
{
	if(m_initialized)
	{		
		glfwSwapBuffers(m_window);
	}
}

bool RenderSubsystem::systemCheck()
{
	return m_initialized;
}

RenderShader * RenderSubsystem::getRenderShader()
{
	return m_shader;
}