#include "Engine.h"
#include <iostream>

#include "Log.h"
#include "RenderSubsystem.h"
#include <GLFW/glfw3.h>

bool Engine::m_initialized;
Log * Engine::m_log;
RenderSubsystem * Engine::m_render_system;

Engine::Engine()
{
	Engine::m_initialized = false;
	m_render_system =  new RenderSubsystem();
}
Engine::~Engine()
{	
}

bool Engine::initialize(unsigned int width, unsigned int height)
{
	m_log = new Log();
	m_log->debugMode();

	glfwInit();

	GLFWwindow * window = glfwCreateWindow(width, height, "Engine", nullptr, nullptr);
  if(window == nullptr)
  {
		glfwTerminate();
  	return false;
  }

	m_render_system->startUp(window);

	m_initialized = true;
	m_log->log("Engine", "initialized");
	return true;
}

void Engine::shutDown()
{
	if(m_initialized)
	{

		m_render_system->shutDown();

		glfwTerminate();

		m_log->log("Engine", "shut down");
		delete m_log;
	}
}

Log * Engine::getLog()
{
	return m_log;
}