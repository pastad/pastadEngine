#include "Engine.h"

#include <iostream>
#include <sstream>

#include "Log.h"
#include "Scene.h"
#include "GUI.h"
#include "RenderSubsystem.h"
#include "IOSubsystem.h"
#include "PhysicSubsystem.h"
#include "EngineGUI.h"
#include "SceneEditor.h"
#include "Light.h"

// the statics
bool Engine::m_initialized;
Log * Engine::m_log;
RenderSubsystem * Engine::m_render_system;
IOSubsystem * Engine::m_io_system;
PhysicSubsystem * Engine::m_physic_system;
GLFWwindow * Engine::m_window;
unsigned int Engine::m_system_flags;
Scene * Engine::m_scene;
unsigned int Engine::m_win_width;
unsigned int Engine::m_win_height;
float Engine::m_time_samples[NUM_TIME_SAMPLES];
unsigned int Engine::m_current_time_sample;
float Engine::m_time_delta;
float Engine::m_time_last;
std::vector<GUI *> Engine::m_guis;
unsigned int Engine::m_guis_id = 0;
EngineGUI * Engine::m_engine_gui;
SceneEditor * Engine::m_scene_editor = nullptr;
bool Engine::m_edit_mode;
float Engine::m_time_last_render;
float Engine::m_render_update_delta= 0.0f;
bool Engine::m_render_update_needed;
bool Engine::m_fullscreen;
bool Engine::m_gui_movement_lock = false;


Engine::Engine()
{
	Engine::m_initialized = false;
	
}
Engine::~Engine()
{		
}

bool Engine::initialize(unsigned int width, unsigned int height, unsigned int system_flags, bool edit, bool fullscreen)
{
	m_win_width = width;
	m_win_height = height;	
	m_edit_mode = edit;
	m_fullscreen = fullscreen;
	m_current_time_sample = 0;
	Engine::m_time_delta = 0.0f;
	Engine::m_time_last = -1.0f;

	m_log = new Log();
	m_log->debugMode();

	m_render_system =  new RenderSubsystem();
	m_io_system =  new IOSubsystem();
	m_physic_system = new PhysicSubsystem();

	// set minimal systems (Render,IO)
	m_system_flags = system_flags;
	if( ! (m_system_flags & IO_SUBSYSTEM) )
	{
		m_system_flags |= IO_SUBSYSTEM;
	}
	if( ! (m_system_flags & RENDER_SUBSYSTEM) )
	{
		m_system_flags |= RENDER_SUBSYSTEM;
	}
	
	// initialize opengl
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// create gl context ..... -1 to fix black window bug
	m_window = glfwCreateWindow(width-1, height-1, "Engine", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);	

	// check window creation
	if(m_window == nullptr)
	{
		glfwTerminate();
  	return false;
	}*/

	refreshWindow();
	if(m_window == nullptr)
	{
		glfwTerminate();
  	return false;
  }

	// check opengl genderated headers
	int loaded = ogl_LoadFunctions();
	if(loaded == ogl_LOAD_FAILED)
	{
		m_log->log("Engine", "OpenGL function wrapper couldn't be loaded");
		glfwTerminate();
		delete m_log;
		return false;
	}

	// check window creation
	bool vers_avail = checkVersionSupport(4, 3 );	
	if(!vers_avail)
	{
		m_log->log("Engine", "OpenGL version not compatible");
		glfwTerminate();
		delete m_log;
  	return false;
	}

	// start the subsystems	
	if(! startUpSubsystems())
	{
		m_log->log("Engine", "Subsystems couldn't be initialized");
		glfwTerminate();
		delete m_log;
  	return false;
	}

	// for fixing black window bug 
	glfwSetWindowSize(m_window,m_win_width,m_win_height);

	// set callback for resize
	glfwSetWindowSizeCallback(m_window, windowSizeChangedCallback);

	glDepthFunc(GL_LEQUAL);
  glCullFace(GL_BACK);
  glEnable(GL_MULTISAMPLE);

  m_engine_gui =  new EngineGUI();
  if(!m_engine_gui->initialize())
  	return false;  
  if(!m_edit_mode)
  	m_engine_gui->setInactive();

  if(m_edit_mode)
  {
  	m_scene_editor =  new SceneEditor();
  	if(!m_scene_editor->initialize())
  		return false;
	}

	m_initialized = true;
	m_log->log("Engine", "initialized");
	return true;
}

void Engine::refreshWindow()
{
	if( m_window != nullptr)
	{
		glfwDestroyWindow(m_window);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER,GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// create gl context ..... -1 to fix black window bug
	m_window = glfwCreateWindow(m_win_width-1, m_win_height-1, "Engine", m_fullscreen ? glfwGetPrimaryMonitor() :nullptr , nullptr);

	// check window creation
	if(m_window == nullptr)
	{
		glfwTerminate();
	}
	else
	{
		glfwMakeContextCurrent(m_window);	
		glfwSetWindowSize(m_window,m_win_width,m_win_height);
	}

}

void Engine::shutDown()
{
	if(m_initialized)
	{
		shutDownSubsystems();

		glfwTerminate();

		m_log->log("Engine", "shut down");
		delete m_scene_editor;
		delete m_engine_gui;
		delete m_log;
		delete m_render_system;
		delete m_io_system;

		for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
		{
			if( (*it) != nullptr)
			{	
				delete (*it);
			}
		}
		m_guis.clear();
		m_initialized = false;
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
		if(m_scene != nullptr)
		{
			m_scene->update(m_time_delta);

			if( m_system_flags & PHYSIC_SUBSYSTEM )
				m_physic_system->updateScene(m_scene, m_time_delta);

		}
		if(m_edit_mode)
			m_scene_editor->update();

		IOSubsystem::clearKeys();
		
		glfwPollEvents();

		timeUpdate();
	}
}

void Engine::timeUpdate()
{
	float now = float(glfwGetTime());
	m_time_samples[m_current_time_sample] = now;

  if(m_time_last == -1.0f)
  {
  	m_time_last = now;
  	m_time_last_render = now;
  }
  else
  {  	
  	m_time_delta = now - m_time_last;
  	m_time_last = now;
  }

  if( (now - m_time_last_render ) > m_render_update_delta)
  {  	
  	m_render_update_needed = true;
  	m_time_last_render = now;  

	  // smooth the FPS for better representation
	  m_current_time_sample = (m_current_time_sample + 1) % NUM_TIME_SAMPLES;

	  if( m_current_time_sample == 0 ) 
	  {
	    float sample_sum = 0.0f;
	    for( int i = 0; i < NUM_TIME_SAMPLES-1 ; i++ )
	      sample_sum += m_time_samples[i + 1] - m_time_samples[i];
	    float fps = NUM_TIME_SAMPLES / sample_sum;

	    std::stringstream ss;
	    
	    ss.precision(4);
	    ss<< "("<< m_win_width<< " x "<< m_win_height << ") FPS: " << fps << "  Delta: " <<m_time_delta;
	    glfwSetWindowTitle(m_window, ss.str().c_str());
	  }
	}
}

bool Engine::running()
{
	if(glfwWindowShouldClose(m_window))
	{
		m_log->log("Engine", "window was closed");
		return false;
	}
	if( ! m_render_system->systemCheck() )
	{
		return false;
	}
	return true;
}


void Engine::render()
{
	if(m_initialized)
	{
		if(m_render_update_needed)
		{
			m_render_system->render();
			m_render_update_needed = false;
		}
	
	}
}

void Engine::errorShutDown()
{
	m_log->log("Engine", "shut down due to request");
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}

bool Engine::startUpSubsystems()
{
	if( m_system_flags & IO_SUBSYSTEM )
	{
		if(! m_io_system->startUp(m_window) )
			return false;
	}
	if( m_system_flags & RENDER_SUBSYSTEM )
	{
		if(! m_render_system->startUp(m_window) )
			return false;
	}
	if( m_system_flags & PHYSIC_SUBSYSTEM )
	{
		if(! m_physic_system->startUp() )
			return false;
	}

	return true;
}

bool Engine::shutDownSubsystems()
{	
	if( m_system_flags & PHYSIC_SUBSYSTEM )
	{
		if(! m_physic_system->shutDown() )
			return false;
	}
	if( m_system_flags & RENDER_SUBSYSTEM )
	{
		if(! m_render_system->shutDown() )
			return false;
	}
	if( m_system_flags & IO_SUBSYSTEM )
	{
		if(! m_io_system->shutDown() )
			return false;
	}

	return true;
}

void Engine::setScene(Scene * scene)
{
	m_scene = scene;
	m_log->log("Engine", "scene was set");
}

Scene * Engine::getScene()
{
	return m_scene;
}
GUI * Engine::getGUI()
{
	GUI * gui = new GUI(m_guis_id);
	m_guis.insert(m_guis.end(), gui);

	m_guis_id++;

	return gui;
}


void Engine::removeGUI(GUI * gui)
{
	for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
	{
		if( (*it)->getId() == gui->getId() )
		{
			delete (*it);
			m_guis.erase(it);
			it = m_guis.end();
		}
	}
}

std::vector<GUI *> * Engine::getGUIs()
{
	return &m_guis;
}
EngineGUI * Engine::getEngineGUI()
{
	return m_engine_gui;
}
SceneEditor * Engine::getSceneEditor()
{
	return m_scene_editor;
}

unsigned int Engine::getWindowWidth()
{
	return m_win_width;
}
unsigned int Engine::getWindowHeight()
{
	return m_win_height;
}

void Engine::windowSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	m_win_width = width;
	m_win_height = height;
	glViewport(0, 0, width, height);
}

bool Engine::checkGUIsForButtonPresses(float x, float y)
{
	bool ret = false;

	for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
	{
		if( (*it)->isActive() )
		{
			if ( (*it)->checkButtonPressed(x,y))
				ret = true;
		}
	}
	if(m_engine_gui->checkButtonPressed(x,y))
		ret = true;
	if(m_scene_editor->checkButtonPressed(x,y))
		ret = true;

	return ret;
}

void Engine::setPostProcessing(PostprocessType type, bool enable)
{
	m_render_system->setPostProcessing(type,enable);
}	
void Engine::setShadowTechnique(ShadowTechniqueType type)
{
	m_render_system->setShadowTechnique(type);
}	

bool Engine::isInEditMode()
{
	return m_edit_mode;
}


void Engine::refreshShaders()
{
	m_render_system->refreshShaders();
}

void Engine::setFPS(float fps)
{
	m_render_update_delta = 1.0f/fps;
}

float Engine::getTimeDelta()
{
	return m_time_delta;
}

void Engine::keyWasPressed(unsigned int key_code)
{
	for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
	{
		if( (*it)->isActive() )
		{
			(*it)->keyWasPressed(key_code);
		}
	}
	if(m_scene_editor != nullptr)
	{
		m_scene_editor->keyWasPressed(key_code);
	}
}


// passers

Object * Engine::pickObjectAtCenter()
{
	return m_render_system->pickObjectAtCenter();
}

Object * Engine::pickObjectAt(glm::vec2 p)
{
	// convert mouse pos to texture pos
	glm::vec2 pos(p.x, getWindowHeight()-p.y);
	return m_render_system->pickObjectAt(pos);
}

Light * Engine::pickLightAt(glm::vec2 p)
{
	// convert mouse pos to texture pos
	glm::vec2 pos(p.x, getWindowHeight()-p.y);
	return m_render_system->pickLightAt(pos);
}

bool Engine::isGUIMovementLockSet()
{
	return m_gui_movement_lock;
}

void Engine::setGUIMovementLock()
{
	m_gui_movement_lock = true;
}
void Engine::unsetGUIMovementLock()
{
	m_gui_movement_lock = false;
}