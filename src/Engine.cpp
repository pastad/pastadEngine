#include "Engine.h"
#include <iostream>

#include "Log.h"
#include "RenderSubsystem.h"

bool Engine::m_initialized;
Log * Engine::m_log;
RenderSubsystem * Engine::m_render_system;
GLFWwindow * Engine::m_window;

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

	// initialize opengl
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// create gl context
	m_window = glfwCreateWindow(width, height, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	// check if everything is running correctly
	bool vers_avail = checkVersionSupport(4, 3 );
	if(!vers_avail)
	{
		glfwTerminate();
  	return false;
	}
	if(m_window == nullptr)
	{
		glfwTerminate();
  	return false;
	}

	// start the subsystems
	m_render_system->startUp(m_window);

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

bool Engine::checkVersionSupport(unsigned int version_major, unsigned int version_minor )
{
	const GLubyte * glsl_version =  glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte * renderer =  glGetString(GL_RENDERER);
	const GLubyte * vendor =  glGetString(GL_VENDOR);

	GLint vers_major;
	GLint vers_minor;
	glGetIntegerv(GL_MAJOR_VERSION,&vers_minor); 
	glGetIntegerv(GL_MAJOR_VERSION,&vers_major); 

	std::cout<<"OpenGL version: "<<vers_major<<"." <<version_minor<<std::endl;
	std::cout<<"GLSL   version: "<<glsl_version<<std::endl;
	//std::cout<<"Vendor        :"<<<vendor<<std::endl;
	//std::cout<<"Renderer      :"<<renderer<<std::endl;

	if(vers_major >= version_major)
	{
		if(vers_minor < version_minor)
			return false;
	}
	else
		return false;

	return true;
}

void Engine::update()
{
	if(m_initialized)
	{
		glfwPollEvents();

	}
}

bool Engine::running()
{
	if(glfwWindowShouldClose(m_window))
	{
		m_log->log("Engine", "window was closed");
		return false;
	}
	return true;
}

void Engine::beginRender()
{
	if(m_initialized)
		m_render_system->startRender();
}

void Engine::endRender()
{
	if(m_initialized)
		m_render_system->endRender();
}