#include "RenderSubsystem.h"
#include "Log.h"
#include "Engine.h"

RenderSubsystem::RenderSubsystem()
{	
}

RenderSubsystem::~RenderSubsystem()
{	
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	m_window = window;
	Engine::getLog()->log("RenderSubsystem", "started");
	return true;
}
bool RenderSubsystem::shutDown()
{
	Engine::getLog()->log("RenderSubsystem", "shut down");
	return true;
}

void RenderSubsystem::startRender()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderSubsystem::endRender()
{
	glfwSwapBuffers(m_window);
}