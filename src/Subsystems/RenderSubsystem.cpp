#include "RenderSubsystem.h"
#include "Log.h"

RenderSubsystem::RenderSubsystem()
{	
	m_initialized = false;
}

RenderSubsystem::~RenderSubsystem()
{	
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	if(!m_initialized)
	{
		m_window = window;

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
	}
}

void RenderSubsystem::endRender()
{
	if(m_initialized)
		glfwSwapBuffers(m_window);
}

bool RenderSubsystem::systemCheck()
{
	return m_initialized;
}