#include "Engine.h"

#include <iostream>
#include <sstream>
#include <future>

#include <QtWidgets/QApplication>

#include "Log.h"
#include "Scene.h"
#include "GUI.h"
#include "RenderSubsystem.h"
#include "IOSubsystem.h"
#include "PhysicSubsystem.h"
#include "EngineGUI.h"
#include "SceneEditor.h"
#include "Light.h"
#include "Camera.h"
#include "Object.h"
#include "Helper.h"
#include "PastadEditor.h"

#include "EngineRequest.h"

// the statics
bool Engine::m_initialized;
Log * Engine::m_log;
RenderSubsystem * Engine::m_render_system;
IOSubsystem * Engine::m_io_system;
PhysicSubsystem * Engine::m_physic_system;
GLFWwindow * Engine::m_window = nullptr;
unsigned int Engine::m_system_flags;
Scene * Engine::m_scene;
Scene * Engine::m_scene_next = nullptr;
bool Engine::m_scene_next_delete = false;
unsigned int Engine::m_win_width;
unsigned int Engine::m_win_height;
float Engine::m_time_samples[NUM_TIME_SAMPLES];
unsigned int Engine::m_current_time_sample;
float Engine::m_time_delta;
float Engine::m_time_last;
std::vector<GUI *> Engine::m_guis;
unsigned int Engine::m_guis_id = 0;
EngineGUI * Engine::m_engine_gui;
unsigned int Engine::m_edit_mode;
float Engine::m_time_last_render;
float Engine::m_render_update_delta= 0.0f;
bool Engine::m_render_update_needed;
bool Engine::m_fullscreen;
bool Engine::m_gui_movement_lock = false;
bool Engine::m_switch_scene =false;
bool Engine::m_run;

Engine::EXTERNALUPDATE Engine::m_external_update = nullptr;
Engine::EXTERNALLOADED Engine::m_external_loaded = nullptr;



Engine::Engine()
{
	Engine::m_initialized = false;
  m_engine_gui =   nullptr;
  m_log =   nullptr;
  m_render_system = nullptr;
  m_io_system = nullptr;
  m_physic_system = nullptr;
}

Engine::~Engine()
{		
 
}


// start-up/shut-down functions ---------------------------------------------------------------------------------------

bool Engine::initialize(unsigned int width, unsigned int height, unsigned int system_flags, unsigned int edit, bool fullscreen)
{
  m_win_width = width;
	m_win_height = height;	
	m_edit_mode = edit;
	m_fullscreen = fullscreen;
	m_current_time_sample = 0;
	Engine::m_time_delta = 0.0f;
	Engine::m_time_last = -1.0f;

  m_log = new Log( (LogFlag) (  LF_Editor )   );
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

  // initialize opengl if in normal
  if(m_edit_mode != EM_QT_EDITOR)
  {
    if( glfwInit() == gl::FALSE_)
      return false;

    refreshWindow();
    if(m_window == nullptr)
    {
      glfwTerminate();
      return false;
    }
  }

  gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
  if (!didLoad)
  {
    m_log->log("Engine", "OpenGL function wrapper couldn't be loaded");
    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();
    delete m_log;
    return false;
  }

	// check window creation
	bool vers_avail = checkVersionSupport(4, 3 );	
	if(!vers_avail)
	{
		m_log->log("Engine", "Opengl:: version not compatible");
    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();
		delete m_log;
      return false;
	}

	// start the subsystems	
	if(! startUpSubsystems())
	{
		m_log->log("Engine", "Subsystems couldn't be initialized");
    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();
		delete m_log;
    return false;
	}

   // set callbacks for normal mode
  if(m_edit_mode != EM_QT_EDITOR)
  {
    // for fixing black window bug
    glfwSetWindowSize(m_window,m_win_width,m_win_height);

    // set callback for resize
    glfwSetWindowSizeCallback(m_window, windowSizeChangedCallback);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeChangedCallback);

  // if in edit mode bind the cursor
    if(m_edit_mode == EM_NORMAL )
      glfwSetInputMode(m_window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  gl::DepthFunc(gl::LEQUAL);
  gl::CullFace(gl::BACK);
  gl::Enable(gl::MULTISAMPLE);

  if(m_edit_mode != EM_QT_EDITOR)
  {
    m_engine_gui =  new EngineGUI();

    if(!m_engine_gui->initialize())
       return false;
    if( (m_edit_mode == EM_NORMAL) || (m_edit_mode == EM_QT_EDITOR) )
       m_engine_gui->setInactive();
  }

  m_initialized = true;
  m_log->log("Engine", "initialized");
  return true;
}


bool Engine::initialize(std::string path)
{

  m_log = new Log((LogFlag)(LF_Editor ));
  m_log->debugMode();

  tinyxml2::XMLDocument document;
  tinyxml2::XMLError result = document.LoadFile(path.c_str());

  if (result != tinyxml2::XML_SUCCESS)
  {
    Engine::getLog()->log("Engine", "couldn't load config file");
    return false;
  }

  ShadowTechniqueType shadow_technique;
  ShadowTechniqueType shadow_technique_point = (ShadowTechniqueType) 99;

  if (!readConfig(path, &m_win_width, &m_win_height, &m_fullscreen, &m_edit_mode, &m_system_flags, &shadow_technique, &shadow_technique_point))
    return false;

  m_current_time_sample = 0;
  Engine::m_time_delta = 0.0f;
  Engine::m_time_last = -1.0f;


  m_render_system = new RenderSubsystem();
  m_io_system = new IOSubsystem();
  m_physic_system = new PhysicSubsystem();

  // set minimal systems (Render,IO)
  if (!(m_system_flags & IO_SUBSYSTEM))
    m_system_flags |= IO_SUBSYSTEM;

  if (!(m_system_flags & RENDER_SUBSYSTEM))
    m_system_flags |= RENDER_SUBSYSTEM;


  // initialize opengl
  if(m_edit_mode != EM_QT_EDITOR)
    glfwInit();

  refreshWindow();

  if(m_edit_mode != EM_QT_EDITOR)
  {
    if (m_window == nullptr)
    {
      glfwTerminate();
      return false;
    }
  }

  // check opengl genderated headers
  gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
  if (!didLoad)
  {
    m_log->log("Engine", "OpenGL function wrapper couldn't be loaded");
    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();
    delete m_log;
    return false;
  }

  // check window creation
  bool vers_avail = checkVersionSupport(4, 3);
  if (!vers_avail)
  {
    m_log->log("Engine", "OpenGL version not compatible");
    if(m_edit_mode != EM_QT_EDITOR)
       glfwTerminate();
    delete m_log;
    return false;
  }

  // start the subsystems 
  if (!startUpSubsystems())
  {
    m_log->log("Engine", "Subsystems couldn't be initialized");
    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();
    delete m_log;
    return false;
  }

  if(m_edit_mode != EM_QT_EDITOR)
  {
    // for fixing black window bug
    glfwSetWindowSize(m_window, m_win_width, m_win_height);

    // set callback for resize
    glfwSetWindowSizeCallback(m_window, windowSizeChangedCallback);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeChangedCallback);

    // if in edit mode bind the cursor
    if (m_edit_mode == EM_NORMAL)
      glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  gl::DepthFunc(gl::LEQUAL);
  gl::CullFace(gl::BACK);
  gl::Enable(gl::MULTISAMPLE);

  m_engine_gui = new EngineGUI();
  if (!m_engine_gui->initialize())
    return false;
  if (m_edit_mode == EM_NORMAL)
    m_engine_gui->setInactive();


  m_render_system->setShadowTechniqueDirectional((ShadowTechniqueType)shadow_technique);
  m_render_system->setShadowTechniquePoint((ShadowTechniqueType)shadow_technique);

  m_initialized = true;
  m_log->log("Engine", "initialized");
  return true;
}

void Engine::shutDown()
{
	if(m_initialized)
	{
		shutDownSubsystems();

    m_log->log("Engine", "Subsystems shut down");

    if(m_edit_mode != EM_QT_EDITOR)
      glfwTerminate();

    if(m_log != nullptr)
		  m_log->log("Engine", "shut down");
     if(m_scene != nullptr)
      delete m_scene;
    if(m_engine_gui != nullptr)
		  delete m_engine_gui;
    if(m_log != nullptr)
		  delete m_log;

    m_log = nullptr;
    m_engine_gui = nullptr;

    if(m_render_system != nullptr)
      delete m_render_system;
    if(m_io_system != nullptr)
      delete m_io_system;
    if(m_physic_system != nullptr)
      delete m_physic_system;

		for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
		{
      if( (*it) != nullptr)
				delete (*it);			
		}
    m_guis.clear();

		m_initialized = false;    
	}
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

void Engine::errorShutDown()
{
  m_log->log("Engine", "shut down due to request");
  if(m_edit_mode != EM_QT_EDITOR)
    glfwSetWindowShouldClose(m_window, gl::TRUE_);
}



// update functions -------------------------------------------------------------------------------------------------

void Engine::refreshWindow()
{
  if(m_edit_mode != EM_QT_EDITOR)
  {
    if( m_window != nullptr)
      glfwDestroyWindow(m_window);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,gl::TRUE_);
    glfwWindowHint(GLFW_DOUBLEBUFFER,gl::TRUE_);
    glfwWindowHint(GLFW_RESIZABLE,gl::FALSE_);
    glfwWindowHint(GLFW_FOCUSED, gl::TRUE_);

    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // create gl context ..... -1 to fix black window bug
    m_window = glfwCreateWindow(m_win_width-1, m_win_height-1, "Engine", m_fullscreen ? glfwGetPrimaryMonitor() :nullptr , nullptr);

    // check window creation
    if(m_window == nullptr)
    {
      getLog()->log("Engine", "window couldn't be created");
      glfwTerminate();
    }
    else
    {
      glfwMakeContextCurrent(m_window);
      glfwSetWindowSize(m_window,m_win_width,m_win_height);
    }
  }
}

void Engine::update(float time_ )
{
  //float now = float(glfwGetTime());
  //std::cout << now << std::endl;
  //now =time_;
	if(m_initialized)
	{    
		sceneSwitch();
		if(m_scene != nullptr)
		{
      if(m_time_last != -1.0f)
        m_scene->update( time_ - m_time_last);

      if( m_system_flags & PHYSIC_SUBSYSTEM )
        m_physic_system->updateScene(m_scene, m_time_delta);
    }
    if(m_edit_mode != EM_QT_EDITOR)
    {
      IOSubsystem::clearKeys();
      glfwPollEvents();
    }
    timeUpdate(time_);
	}
	//now = float(glfwGetTime());
	//std::cout << now << std::endl;
}


void Engine::timeUpdate(float time_)
{
  float now = time_;
  if(m_edit_mode != EM_QT_EDITOR)
    now = float(glfwGetTime());
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
      if(m_edit_mode != EM_QT_EDITOR)
        glfwSetWindowTitle(m_window, ss.str().c_str());
	  }
	}
}



// running/render/checks functions -----------------------------------------------------------------------------------------

void Engine::run()
{
  m_run = true;
  while (running() && m_run)
  {
    if(m_external_update!= nullptr)
      m_external_update(getTimeDelta());
    update(0.0f);
    render();  

    Helper::checkGLError("EngineRun");
  }
}

void Engine::stopRunning()
{
  m_run = false;
}

bool Engine::running()
{
	//std::cout << "------------------------------" << std::endl;
	//float now = float(glfwGetTime());
  //std::cout << now << std::endl;
  if(m_edit_mode != EM_QT_EDITOR)
  {
    if(glfwWindowShouldClose(m_window))
    {
      m_log->log("Engine", "window was closed");
      return false;
    }
  }
  if( ! m_render_system->systemCheck() )
		return false;

	//now = float(glfwGetTime());
	//std::cout << now << std::endl;
	return true;
}

void Engine::render()
{
  if(m_edit_mode != EM_QT_EDITOR)
  {
    gl::ClearColor(1,0,0,1);
    gl::Clear(gl::COLOR_BUFFER_BIT);
    //float now = float(glfwGetTime());
    //std::cout << now << std::endl;
  }
	if(m_initialized)
  {
    if(m_edit_mode != EM_QT_EDITOR)
    {
      if(m_render_update_needed && m_scene != nullptr)
      {
        m_render_system->render( m_edit_mode != EM_QT_EDITOR);
        m_render_update_needed = false;
      }
    }
    else
    {
      if(m_scene != nullptr)
      {
        m_render_system->render( m_edit_mode != EM_QT_EDITOR);
        m_render_update_needed = false;
      }
    }
	}
	//now = float(glfwGetTime());
	//std::cout << now << std::endl;
}

bool Engine::checkVersionSupport(unsigned int version_major, unsigned int version_minor )
{
  const GLubyte * glsl_version = gl::GetString(gl::SHADING_LANGUAGE_VERSION);
  const GLubyte * renderer = gl::GetString(gl::RENDERER);
  const GLubyte * vendor = gl::GetString(gl::VENDOR);

  GLint vers_major;
  GLint vers_minor;
  gl::GetIntegerv(gl::MAJOR_VERSION,&vers_minor);
  gl::GetIntegerv(gl::MAJOR_VERSION,&vers_major);

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

void Engine::refreshShaders()
{
  m_render_system->refreshShaders();
}



// getter/setters functions -----------------------------------------------------------------------------------------

// log

Log * Engine::getLog()
{
  return m_log;
}

// scene

Scene * Engine::getScene()
{
  return m_scene;
}

void Engine::setScene(Scene * scene, bool delete_old)
{
  m_switch_scene = true;
	m_scene_next = scene;
	m_scene_next_delete = delete_old;
 
	m_log->log("Engine", "scene was set");
}

void Engine::sceneSwitch()
{
	if(m_switch_scene)
	{
		Scene * t = m_scene;

    m_scene = m_scene_next;

		if(t != nullptr && m_scene_next_delete)
		{
			m_log->log("Engine", "old scene is deleted");
			delete t;
		}
	
		m_scene_next = nullptr; 
		m_log->log("Engine", "scene switched");   
    m_switch_scene = false;

    if(m_external_loaded != nullptr)
     m_external_loaded();
	}
}

// gui

GUI * Engine::addGUI()
{
	GUI * gui = new GUI(m_guis_id);
	m_guis.insert(m_guis.end(), gui);

	m_guis_id++;

	return gui;
}

void Engine::removeGUI(GUI * gui)
{
	for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();)
	{
		if( (*it)->getId() == gui->getId() )
		{
			delete (*it);
			it = m_guis.erase(it);
		}
    else
      it++;
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

// width/height
unsigned int Engine::getWindowWidth()
{
	return m_win_width;
}

unsigned int Engine::getWindowHeight()
{
	return m_win_height;
}


// subsystem
PhysicSubsystem * Engine::getPhysicSubsystem()
{
  if(m_physic_system->systemCheck())
    return m_physic_system;

  return nullptr;
}
RenderSubsystem * Engine::getRenderSubsystem()
{
  if (m_render_system->systemCheck())
    return m_render_system;
}


// fps/time

void Engine::setFPS(float fps)
{
  m_render_update_delta = 1.0f/fps;
}

float Engine::getTimeDelta()
{
  return m_time_delta;
}


// techniques

void Engine::setPostProcessing(PostprocessType type, bool enable)
{
  m_render_system->setPostProcessing(type,enable);
}

void Engine::setShadowTechniquePoint(ShadowTechniqueType type)
{
  m_render_system->setShadowTechniquePoint(type);
} 
void Engine::setShadowTechniqueDirectional(ShadowTechniqueType type)
{
  m_render_system->setShadowTechniqueDirectional(type);
}
void Engine::setShadowTechniqueSSAO(ShadowTechniqueType type)
{
  m_render_system->setShadowSSAO(type);
}

bool Engine::isInEditMode()
{
  return( (m_edit_mode == EM_INTERNAL_EDITOR) ) ? true : false ;
}
bool Engine::isInExternalEditMode()
{
  return((m_edit_mode == EM_EXTERNAL_EDITOR)) ? true : false;
}


// internal callback functions -----------------------------------------------------------------------------------------

void Engine::windowSizeChangedCallback(GLFWwindow* window, int width, int height)
{
  if(m_edit_mode != EM_QT_EDITOR)
  {
    if( m_window == window)
    {
      m_win_width = width;
      m_win_height = height;
    }
  }
  gl::Viewport(0, 0, m_win_width, m_win_height);
  Engine::getLog()->log("Engine", "window resized");
}
void Engine::framebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
  Engine::getLog()->log("Engine", "framebuffer resized");
  if(m_edit_mode != EM_QT_EDITOR)
    gl::Viewport(0, 0, width, height);
}



// request functions -----------------------------------------------------------------------------------------

void Engine::keyWasPressed(unsigned int key_code)
{
	for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
	{
		if( (*it)->isActive() )
		{
			(*it)->keyWasPressed(key_code);
		}
  }
}

bool Engine::checkGUIsForButtonPresses(float x, float y)
{
  bool ret = false;

 /* std::vector<std::future<bool>> futures;

  for( std::vector<GUI*>::iterator it = m_guis.begin(); it != m_guis.end();it++)
  {
    if( (*it)->isActive() )
    {
      futures.push_back (std::async(std::launch::async,buttonCheck,(*it), x,y));
      //if ( (*it)->checkButtonPressed(x,y))
      //  ret = true;
    }
  }
  //futures.push_back (std::async(std::launch::async,buttonCheck,( (GUI *)m_engine_gui), x,y));
  //futures.push_back (std::async(std::launch::async,buttonCheck,( (GUI *)m_scene_editor), x,y));
  if(m_engine_gui->checkButtonPressed(x,y))
    ret = true;
  if(m_edit_mode)
  {
    if(m_scene_editor->checkButtonPressed(x,y))
      ret = true;
  }
  //std::cout << futures.size() << std::endl;
  for(auto &e : futures) 
  {
    bool ergeb = e.get();
    if(ergeb)
      ret = true;
  } */
  
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
  
  return ret;
}

bool Engine::buttonCheck(GUI *gui , float x, float y)
{
  if ( gui->checkButtonPressed(x,y))
    return true;
  return false;
}



// passers functions -----------------------------------------------------------------------------------------

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

bool Engine::isKeyReleasedAndPressed(int key_code)
{
  return m_io_system->isKeyReleasedAndPressed(key_code);
}
bool Engine::isKeyPressedAndReleased(int key_code)
{
  return m_io_system->isKeyPressedAndReleased(key_code);
}
bool Engine::isMouseButtonReleasedAndPressed(int key_code)
{
  return m_io_system->isMouseButtonReleasedAndPressed(key_code);
}



// gui-lock getter setter  -----------------------------------------------------------------------------------------

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

// callback function


void  Engine::setUpdateFunction(EXTERNALUPDATE callback_update)
{
  m_external_update = callback_update;
}
void  Engine::setLoadedFunction(EXTERNALLOADED callback_loaded)
{
  m_external_loaded = callback_loaded;
}



// saver  ---------------------------------------------------------------------------------------------------------

bool Engine::saveConfig(std::string path, unsigned int width, unsigned int height, bool fullscreen, unsigned int edit_mode, unsigned int system_flags, ShadowTechniqueType shadow_technique, ShadowTechniqueType shadow_technique_point)
{
  Engine::getLog()->log("Engine", "saving config to", path);
  tinyxml2::XMLDocument document;
  tinyxml2::XMLNode * root = document.NewElement("EngineConfig");
  document.InsertFirstChild(root);

  tinyxml2::XMLElement * element_window = document.NewElement("Window");
  root->InsertEndChild(element_window);
  element_window->SetAttribute("width", width);
  element_window->SetAttribute("height", height);
  element_window->SetAttribute("fullscreen", fullscreen);

  tinyxml2::XMLElement * element_enginemode = document.NewElement("EngineMode");
  root->InsertEndChild(element_enginemode);
  element_enginemode->SetAttribute("edit", edit_mode);

  tinyxml2::XMLElement * element_subsystems = document.NewElement("Subsystems");
  root->InsertEndChild(element_subsystems);

  element_subsystems->SetAttribute("io", (system_flags & IO_SUBSYSTEM) ? 1 : 0);
  element_subsystems->SetAttribute("render", (system_flags & RENDER_SUBSYSTEM) ? 1 : 0);
  element_subsystems->SetAttribute("physic", (system_flags & PHYSIC_SUBSYSTEM) ? 1 : 0);

  tinyxml2::XMLElement * element_shadow_tech = document.NewElement("ShadowDirectional");
  root->InsertEndChild(element_shadow_tech);

  element_shadow_tech->SetAttribute("technique", (unsigned int)shadow_technique);

  element_shadow_tech = document.NewElement("ShadowPoint");
  root->InsertEndChild(element_shadow_tech);

  element_shadow_tech->SetAttribute("technique", (unsigned int)shadow_technique_point);

  tinyxml2::XMLError eResult = document.SaveFile(path.c_str());

  return true;
}

bool Engine::readConfig(std::string path, unsigned int* width, unsigned int *height, bool *fullscreen, unsigned int* edit_mode, unsigned int *system_flags, ShadowTechniqueType *shadow_technique, ShadowTechniqueType *shadow_technique_point)
{

  tinyxml2::XMLDocument document;
  tinyxml2::XMLError result = document.LoadFile(path.c_str());

  if (result != tinyxml2::XML_SUCCESS)
  {
    Engine::getLog()->log("Engine", "couldn't load config file");
    return false;
  }

  *width = 1240;
  *height = 720;

  *fullscreen = false;

  *edit_mode = 0;

  *system_flags = RENDER_SUBSYSTEM | IO_SUBSYSTEM;

  tinyxml2::XMLNode * root = document.FirstChild();

  if (root != nullptr)
  {
    Engine::getLog()->log("Engine", "reading config: ", path);
    tinyxml2::XMLElement * child = root->FirstChildElement();

    while (child != nullptr)
    {
      std::string type = std::string(child->Name());
      Engine::getLog()->log("Engine", "element read:", type);
      if (type == "Window")
      {
        child->QueryUnsignedAttribute("width", width);
        child->QueryUnsignedAttribute("height", height);

        *fullscreen = child->BoolAttribute("fullscreen");
      }
      if (type == "EngineMode")
      {
        child->QueryUnsignedAttribute("edit", edit_mode);
      }
      if (type == "Subsystems")
      {
        if (child->BoolAttribute("render"))
          *system_flags |= RENDER_SUBSYSTEM;

        if (child->BoolAttribute("physic"))
          *system_flags |= PHYSIC_SUBSYSTEM;

        if (child->BoolAttribute("io"))
          *system_flags |= IO_SUBSYSTEM;


      }
      if (type == "Shadow")
      {
        unsigned int tech;
        child->QueryUnsignedAttribute("technique", &tech);
        if (tech <= ST_STANDARD_RS)
        {
          *shadow_technique = (ShadowTechniqueType)tech;
          *shadow_technique_point = (ShadowTechniqueType)tech;
        }
      }
      if (type == "ShadowDirectional")
      {
        unsigned int tech;
        child->QueryUnsignedAttribute("technique", &tech);
        if (tech <= ST_STANDARD_RS)
        {
          *shadow_technique = (ShadowTechniqueType)tech;
        }
      }
      if (type == "ShadowPoint")
      {
        unsigned int tech;
        child->QueryUnsignedAttribute("technique", &tech);
        if (tech <= ST_STANDARD_RS)
        {
          *shadow_technique_point = (ShadowTechniqueType)tech;
        }
      }

      child = child->NextSiblingElement();
    }
  }
  else
  {
    Engine::getLog()->log("Engine", "no root element in config file");
  }
  return true;
}
