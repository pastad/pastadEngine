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
//EDIT GLFWwindow * Engine::m_window;
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
SceneEditor * Engine::m_scene_editor = nullptr;
unsigned int Engine::m_edit_mode;
float Engine::m_time_last_render;
float Engine::m_render_update_delta= 0.0f;
bool Engine::m_render_update_needed;
bool Engine::m_fullscreen;
bool Engine::m_gui_movement_lock = false;
bool Engine::m_switch_scene =false;
bool Engine::m_run;
Editor * Engine::m_editor;
std::future<void> Engine::m_editor_future;

PastadEditor *Engine::m_pastad_editor = nullptr;

Engine::EXTERNALUPDATE Engine::m_external_update = nullptr;
Engine::EXTERNALLOADED Engine::m_external_loaded = nullptr;

std::vector<EngineRequest *> Engine::m_requests;



Engine::Engine()
{
	Engine::m_initialized = false;
	m_scene_editor =  nullptr;
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

    m_log = new Log( (LogFlag) (  LF_All )   );
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
   // if( glfwInit() == gl::FALSE_)
    //    return false;

   /* EDIT if(m_edit_mode != EM_QT_EDITOR)
    {
        refreshWindow();
        if(m_window == nullptr)
        {
            glfwTerminate();
            return false;
        }
    }*/

     gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
      if (!didLoad)
      {
        m_log->log("Engine", "OpenGL function wrapper couldn't be loaded");
      //EDIT glfwTerminate();
        delete m_log;
        return false;
      }


	// check window creation
	bool vers_avail = checkVersionSupport(4, 3 );	
	if(!vers_avail)
	{
		m_log->log("Engine", "Opengl:: version not compatible");
        //EDIT glfwTerminate();
		delete m_log;
  	return false;
	}

	// start the subsystems	
	if(! startUpSubsystems())
	{
		m_log->log("Engine", "Subsystems couldn't be initialized");
        //EDIT glfwTerminate();
		delete m_log;
  	return false;
	}

	// for fixing black window bug 
    //EDIT glfwSetWindowSize(m_window,m_win_width,m_win_height);

	// set callback for resize
//EDIT 	glfwSetWindowSizeCallback(m_window, windowSizeChangedCallback);
 //EDIT  glfwSetFramebufferSizeCallback(m_window, framebufferSizeChangedCallback);

  // if in edit mode bind the cursor
  //if(m_edit_mode == EM_NORMAL )
  //  glfwSetInputMode(m_window,GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  gl::DepthFunc(gl::LEQUAL);
  gl::CullFace(gl::BACK);
  gl::Enable(gl::MULTISAMPLE);
  /*
 // m_engine_gui =  new EngineGUI();
 //EDIT  if(!m_engine_gui->initialize())
 //EDIT    return false;
 EDIT if( (m_edit_mode == EM_NORMAL) || (m_edit_mode == EM_QT_EDITOR) )
    m_engine_gui->setInactive();

    if(m_edit_mode == EM_INTERNAL_EDITOR)
    {
        m_scene_editor =  new SceneEditor();
        if(!m_scene_editor->initialize())
            return false;
	}
*/
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
  {
    m_system_flags |= IO_SUBSYSTEM;
  }
  if (!(m_system_flags & RENDER_SUBSYSTEM))
  {
    m_system_flags |= RENDER_SUBSYSTEM;
  }

  // initialize opengl
 //EDIT glfwInit();

  refreshWindow();

  //EDITif (m_window == nullptr)
  //EDIT{
   //EDIT glfwTerminate();
  //EDIT  return false;
  //EDIT}

  // check opengl genderated headers
  gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
  if (!didLoad)
  {
    m_log->log("Engine", "OpenGL function wrapper couldn't be loaded");
   //EDIT glfwTerminate();
    delete m_log;
    return false;
  }

  // check window creation
  bool vers_avail = checkVersionSupport(4, 3);
  if (!vers_avail)
  {
    m_log->log("Engine", "OpenGL version not compatible");
   //EDIT glfwTerminate();
    delete m_log;
    return false;
  }

  // start the subsystems 
  if (!startUpSubsystems())
  {
    m_log->log("Engine", "Subsystems couldn't be initialized");
   //EDIT glfwTerminate();
    delete m_log;
    return false;
  }

  // for fixing black window bug 
  //EDITglfwSetWindowSize(m_window, m_win_width, m_win_height);

  // set callback for resize
  //EDITglfwSetWindowSizeCallback(m_window, windowSizeChangedCallback);
  //EDITglfwSetFramebufferSizeCallback(m_window, framebufferSizeChangedCallback);

  // if in edit mode bind the cursor
  //EDIT if (m_edit_mode == EM_NORMAL)
    //EDIT glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  gl::DepthFunc(gl::LEQUAL);
  gl::CullFace(gl::BACK);
  gl::Enable(gl::MULTISAMPLE);

  m_engine_gui = new EngineGUI();
  if (!m_engine_gui->initialize())
    return false;
  if (m_edit_mode == EM_NORMAL)
    m_engine_gui->setInactive();

  if (m_edit_mode == EM_INTERNAL_EDITOR)
  {
    m_scene_editor = new SceneEditor();
    if (!m_scene_editor->initialize())
      return false;
  }


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

        //EDIT glfwTerminate();

    

    if(m_log != nullptr)
		  m_log->log("Engine", "shut down");
     if(m_scene != nullptr)
      delete m_scene;
		if(m_scene_editor != nullptr)
			delete m_scene_editor;
    if(m_engine_gui != nullptr)
		  delete m_engine_gui;
    if(m_log != nullptr)
		  delete m_log;


    m_log = nullptr;
    m_engine_gui = nullptr;
    m_scene_editor = nullptr;


    if(m_render_system != nullptr)
      delete m_render_system;
    if(m_io_system != nullptr)
      delete m_io_system;
    if(m_physic_system != nullptr)
      delete m_physic_system;

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

bool Engine::startUpSubsystems()
{
  if( m_system_flags & IO_SUBSYSTEM )
  {
    if(! m_io_system->startUp(nullptr) )
      return false;
  }
  if( m_system_flags & RENDER_SUBSYSTEM )
  {//EDIT
    if(! m_render_system->startUp(nullptr) )
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
  //EDITglfwSetWindowShouldClose(m_window, gl::TRUE_);
}



// update functions -------------------------------------------------------------------------------------------------

void Engine::refreshWindow()
{
  //EDITif( m_window != nullptr)
  //EDIT{
   //EDIT glfwDestroyWindow(m_window);
  //EDIT}
 //EDIT glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
 //EDIT glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  //EDITglfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,gl::TRUE_);
 //EDIT glfwWindowHint(GLFW_DOUBLEBUFFER,gl::TRUE_);
 //EDIT glfwWindowHint(GLFW_RESIZABLE,gl::FALSE_);
 //EDIT glfwWindowHint(GLFW_FOCUSED, gl::TRUE_);
  
 //EDIT glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  // create gl context ..... -1 to fix black window bug
  //EDITm_window = glfwCreateWindow(m_win_width-1, m_win_height-1, "Engine", m_fullscreen ? glfwGetPrimaryMonitor() :nullptr , nullptr);
  

  // check window creation
  //EDITif(m_window == nullptr)
  //EDIT{
  //EDIT  glfwTerminate();
  //EDIT}
 //EDITelse
 //EDIT {
 //EDIT   glfwMakeContextCurrent(m_window);
 //EDIT   glfwSetWindowSize(m_window,m_win_width,m_win_height);
 //EDIT }

}

void Engine::update()
{
	//float now = float(glfwGetTime());
	//std::cout << now << std::endl;
	if(m_initialized)
	{    
		sceneSwitch();
		if(m_scene != nullptr)
		{

            m_scene->update(0.1f);

            //if( m_system_flags & PHYSIC_SUBSYSTEM )
            //	m_physic_system->updateScene(m_scene, m_time_delta);

        //  if(m_edit_mode == EM_INTERNAL_EDITOR)
        //	  m_scene_editor->update();
         }

    //EDIT	IOSubsystem::clearKeys();

        //EDIT glfwPollEvents();
        //EDIT timeUpdate();
	}
	//now = float(glfwGetTime());
	//std::cout << now << std::endl;
}

void Engine::timeUpdate()
{
    float now = 0;//EDITfloat(glfwGetTime());
	m_time_samples[m_current_time_sample] = now;
	//std::cout << now << std::endl;

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
//EDIT	    glfwSetWindowTitle(m_window, ss.str().c_str());
	  }
	}
}



// running/render/checks functions -----------------------------------------------------------------------------------------

void Engine::run()
{
  m_run = true;
  while (running() && m_run)
  {
    handleEditorRequests();
   // if(m_scene != nullptr)
   //   m_scene->acquireLock("EngineRun");
   // m_render_system->acquireRenderLock("EngineRun");
    if(m_external_update!= nullptr)
      m_external_update(getTimeDelta());
    update();   
    render();  

    Helper::checkGLError("EngineRun");
    
   // m_render_system->releaseRenderLock("EngineRun");
   // if (m_scene != nullptr)
    //  m_scene->releaseLock("EngineRun");
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
//	std::cout << now << std::endl;
    /*if(glfwWindowShouldClose(m_window))
	{
		m_log->log("Engine", "window was closed");
		return false;
    }*/
	if( ! m_render_system->systemCheck() )
	{
		return false;
	}
	//now = float(glfwGetTime());
	//std::cout << now << std::endl;
	return true;
}

void Engine::render()
{ //gl::ClearColor(1,0,0,1);
   // gl::Clear(gl::COLOR_BUFFER_BIT);
	//float now = float(glfwGetTime());
	//std::cout << now << std::endl;
	if(m_initialized)
    {
        if(m_scene != nullptr)//m_render_update_needed &&
		{
            m_render_system->render( m_edit_mode != EM_QT_EDITOR);
            m_render_update_needed = false;
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

// editor

Editor *  Engine::getEditor()
{
  return m_editor;
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

//editor

SceneEditor * Engine::getSceneEditor()
{
	return m_scene_editor;
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
  //EDITif( m_window == window)
 //EDIT {
 //EDIT 	m_win_width = width;
//EDIT	  m_win_height = height;
 //EDIT
 //EDIT }
  gl::Viewport(0, 0, m_win_width, m_win_height);
  Engine::getLog()->log("Engine", "window resized");
}
void Engine::framebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
  Engine::getLog()->log("Engine", "framebuffer resized");
//  gl::Viewport(0, 0, width, height);
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
	if(m_scene_editor != nullptr)
	{
		m_scene_editor->keyWasPressed(key_code);
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
  if(m_edit_mode == EM_INTERNAL_EDITOR)
  {
    if(m_scene_editor->checkButtonPressed(x,y))
      ret = true;
  }
  
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

void Engine::handleEditorRequests()
{
    /*EDIT
  if(m_scene != nullptr)
  {
    for (std::vector<EngineRequest *>::iterator it = m_requests.begin() ; it != m_requests.end();)
    {
      if ((*it)->getType() == ERT_SHADER_REFRESH)
      {
        refreshShaders();
      }
      if ((*it)->getType() == ERT_ADD_OBJECT)
      {
        AddObjectRequest * aor = ((AddObjectRequest *)(*it));
        Object * obj = m_scene->addObject( aor->getPath(), glm::vec3(0, 0, 0), aor->getStatic());

        if (aor->getShadowOnly())
          obj->setVisibility(Visibility::V_Shadow);
        if(!aor->getVisible())
          obj->setVisibility(Visibility::V_None);


        if (aor->getApplyPhysics())
          obj->applyPhysics();
        if (aor->getPhysicsStatic() )
          obj->applyPhysicsStatic();

        
        m_pastad_editor->changeObject(obj);
      }
      if ((*it)->getType() == ERT_REMOVE_OBJECT)
      {
        getLog()->log("Engine", "rem object");
        m_scene->removeObject(((RemoveObjectRequest *)(*it))->getObject());
        m_pastad_editor->refreshObjectList();
      }
      if ((*it)->getType() == ERT_ADD_LIGHT)
      {
        if (((AddLightRequest *)(*it))->getLightType() == LIGHT_POINT)
        {
          Light * new_light = m_scene->addLight();
          if (!new_light->setPoint(m_scene->getCamera()->getPosition(), glm::vec3(1, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1, 1, 1), 0.5f, 0.1f, 0.09f, 0.032f, ((AddLightRequest *)(*it))->castsShadow()))
            m_scene->removeLight(new_light);
          else
            m_pastad_editor->changeLight(new_light);
          m_pastad_editor->refreshLightList();
        }
        if (((AddLightRequest *)(*it))->getLightType() == LIGHT_DIRECTIONAL)
        {
          Light * new_light = m_scene->addLight();
          if (!new_light->setDirectional(glm::vec3(0, 1, 0), glm::vec3(1, 0.95, 0.9), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0.3f, ((AddLightRequest *)(*it))->castsShadow()))
            m_scene->removeLight(new_light);
          else
            m_pastad_editor->changeLight(new_light);
          m_pastad_editor->refreshLightList();
        }
        if (((AddLightRequest *)(*it))->getLightType() == LIGHT_SPOT)
        {
          Light * new_light = m_scene->addLight();
          if (!new_light->setSpot(m_scene->getCamera()->getPosition(), glm::vec3(1, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1, 1, 1), 1.0f, 1.0f, 0.09f, 0.032f, 45.0f, glm::vec2(0, 0), ((AddLightRequest *)(*it))->castsShadow()))
            m_scene->removeLight(new_light);
          else
            m_pastad_editor->changeLight(new_light);
          m_pastad_editor->refreshLightList();
        }
      }
      if ((*it)->getType() == ERT_REMOVE_LIGHT)
      {
        m_scene->removeLight(((RemoveLightRequest *)(*it))->getLight());
        m_pastad_editor->refreshLightList();
      }
      if ((*it)->getType() == ERT_SET_SHADOW_TECHNIQUE_DIRECTIONAL)
      {
        setShadowTechniqueDirectional(((SetShadowTechniqueDirectionalRequest *)(*it))->getShadowTechnique() );
      }
      if ((*it)->getType() == ERT_SET_SHADOW_TECHNIQUE_POINT)
      {
        setShadowTechniquePoint(((SetShadowTechniquePointRequest *)(*it))->getShadowTechnique());
      }
      if ((*it)->getType() == ERT_SET_SHADOW_TECHNIQUE_SSAO)
      {
        setShadowTechniqueSSAO(((SetShadowTechniqueAdditionalRequest *)(*it))->getShadowTechnique());
      }
      if ((*it)->getType() == ERT_SET_PP_TECHNIQUE)
      {
        setPostProcessing(((SetPPTechniqueRequest *)(*it))->getPPTechnique(), ((SetPPTechniqueRequest *)(*it))->getState());
      }
      if ((*it)->getType() == ERT_LOAD_SCENE)
      {
        Scene * scene = new Scene();

        if (scene->load(((LoadSceneRequest *)(*it))->getPath()))
        {
          std::cout << "setting scene" << std::endl;
          Engine::setScene(scene, true);
          scene->getCamera()->dontApplyPhysicsDrop();
          scene->getCamera()->setRotationAllowed();
        }
     
      }

      delete (*it);
      it = m_requests.erase(it);
    } 
  
  
  }*/
}


void Engine::setPastadEditor(PastadEditor * editor)
{
  m_pastad_editor = editor;
  m_edit_mode = EM_EXTERNAL_EDITOR;
}

PastadEditor * Engine::getPastadEditor()
{
  return m_pastad_editor;
}

void Engine::addRequest(EngineRequest * er)
{
  m_requests.push_back(er);
}

/*
void Engine::startEditor()
{
  char* dummy_args[] = {  NULL };
  int dummy_argc = 0;
  QApplication app(dummy_argc, dummy_args);

  m_editor = new Editor();

  m_editor->setupUi(m_editor);
  m_editor->setScene(getScene());


  m_editor->show();

  app.exec();
}*/

