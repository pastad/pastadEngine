#include "IOSubsystem.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool IOSubsystem::m_keys[GLFW_KEY_LAST + 1 ];
bool IOSubsystem::m_keys_pressed_and_released[GLFW_KEY_LAST + 1 ];
bool IOSubsystem::m_keys_released_and_pressed[GLFW_KEY_LAST + 1 ];
bool IOSubsystem::m_mouse_buttons[GLFW_MOUSE_BUTTON_8 + 1 ];
bool IOSubsystem::m_mouse_buttons_pressed_and_released[GLFW_MOUSE_BUTTON_8 + 1 ];
bool IOSubsystem::m_mouse_buttons_released_and_pressed[GLFW_MOUSE_BUTTON_8 + 1 ];
double IOSubsystem::m_mouse_x = -1;
double IOSubsystem::m_mouse_y= -1;
glm::vec2 IOSubsystem::m_mouse_delta;


IOSubsystem::EXTERNALKEYCALLBACK IOSubsystem::m_external_keyCallback = nullptr ;
IOSubsystem::EXTERNALMOUSEMOVECALLBACK IOSubsystem::m_external_mouseMoveCallback = nullptr ;
IOSubsystem::EXTERNALMOUSESCROLLCALLBACK IOSubsystem::m_external_mouseScrollCallback = nullptr ;
IOSubsystem::EXTERNALMOUSEKEYCALLBACK IOSubsystem::m_external_mouseKeyCallback = nullptr;

IOSubsystem::IOSubsystem():Subsystem("IOSubsystem")
{
	m_initialized = false;
}

IOSubsystem::~IOSubsystem()
{
}


bool IOSubsystem::startUp(GLFWwindow * window)
{
	if(!m_initialized)
	{
		// init system here
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback (window, mouseScrollCallback); 
		Engine::getLog()->log("IOSubsystem", "started");
		
    m_initialized = true;
    return true;
	}
	return false;
}



// helpers ------------------------------------------

void IOSubsystem::clearKeys()
{
  for(int x=0; x< GLFW_KEY_LAST;x++)
    m_keys_pressed_and_released[x] = m_keys_released_and_pressed[x] = false;

  for(int x=0; x< GLFW_MOUSE_BUTTON_8;x++)
    m_mouse_buttons_pressed_and_released[x] = m_mouse_buttons_released_and_pressed[x] = false;
}


// queries ------------------------------------------

// keys

bool IOSubsystem::isKeyPressed(int key)
{
  if(key <= GLFW_KEY_LAST)
    return m_keys[key];
  else
    return false;
}

bool IOSubsystem::isKeyPressedAndReleased(int key)
{
  if(key <= GLFW_KEY_LAST)
    return m_keys_pressed_and_released[key];
  else
    return false;
}

bool IOSubsystem::isKeyReleasedAndPressed(int key)
{
  if(key <= GLFW_KEY_LAST)
    return m_keys_released_and_pressed[key];
  else
    return false;
}

bool * IOSubsystem::getKeysPressedAndReleased()
{
  return m_keys_pressed_and_released;
}

//mouse

bool IOSubsystem::isMouseButtonPressed(int key)
{
  if(key <= GLFW_MOUSE_BUTTON_8)
    return m_mouse_buttons[key];
  else
    return false;
}

bool IOSubsystem::isMouseButtonPressedAndReleased(int key)
{
  if(key <= GLFW_MOUSE_BUTTON_8)
    return m_mouse_buttons_pressed_and_released[key];
  else
    return false;
}

bool IOSubsystem::isMouseButtonReleasedAndPressed(int key)
{
  if(key <= GLFW_MOUSE_BUTTON_8)
    return m_mouse_buttons_released_and_pressed[key];
  else
    return false;
}

glm::vec2 IOSubsystem::getMouseCoordinates()
{
  return glm::vec2(m_mouse_x,m_mouse_y);
}

glm::vec2 IOSubsystem::getMouseDelta()
{
  glm::vec2 ret = m_mouse_delta; 
  m_mouse_delta = glm::vec2(0,0);
  return ret;
}

// file system

std::string IOSubsystem::readFile(std::string path)
{
  std::ifstream file;
  file.open((path).c_str());
  std::string output;
 
  if(file.is_open())
  {
    std::string line;
    while(file.good())
    {
      getline(file, line);
      output.append(line + "\n");
    }
    file.close();
  }
  else
    Engine::getLog()->log("IOSubsystem","Unable to load file");

  Engine::getLog()->log("IOSubsystem","loaded",path); 

  return output;
}




// callbacks ------------------------------------------

void IOSubsystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
 // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
 //   Engine::errorShutDown();

  if(action == GLFW_PRESS)
  {
    if( ! m_keys[key] )
    {
      m_keys_released_and_pressed[key] = true;
    }    
    m_keys[key] = true;
  }
  if(action == GLFW_RELEASE)
  {
    if(  m_keys[key] )
    {
      m_keys_pressed_and_released[key] = true;
      Engine::keyWasPressed(key);
    }

    m_keys[key] = false;
  }
  if(m_external_keyCallback != nullptr)
    m_external_keyCallback(key, ( action == GLFW_PRESS) ? 1: 0 );
}

void IOSubsystem::mouseMoveCallback(GLFWwindow* window, double x, double y )
{
  if(m_external_mouseMoveCallback != nullptr)
    m_external_mouseMoveCallback(x,y);

  if(m_mouse_x == -1)
  {
    m_mouse_x =x;
    m_mouse_y =y;
  }
  else
  {
    double deltax= x - m_mouse_x ;
    double deltay= y - m_mouse_y ;
    m_mouse_delta = glm::vec2(deltax,deltay);
    m_mouse_x =x;
    m_mouse_y =y;
  }
}

void IOSubsystem::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{ 


 if(action == GLFW_PRESS)
  {        
    if(!m_mouse_buttons[button])  
    {  
      //Engine::getLog()->log("IOSubsystem","mouse pressed and released");   
      m_mouse_buttons_released_and_pressed[button] = true;
    }

    m_mouse_buttons[button] = true;     
  }
  if(action == GLFW_RELEASE)
  {
    //Engine::getLog()->log("IOSubsystem", "mouse button released");
    if(m_mouse_buttons[button])  
    {  
      m_mouse_buttons_pressed_and_released[button] = true;
      Engine::checkGUIsForButtonPresses(m_mouse_x, Engine::getWindowHeight() - m_mouse_y);
    }
    
    m_mouse_buttons[button] = false;
  }

}

void IOSubsystem::mouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{ 
  if(m_external_mouseScrollCallback != nullptr)
    m_external_mouseScrollCallback(xoffset,yoffset);
}



void IOSubsystem::registerKeyCallback( EXTERNALKEYCALLBACK  callback   )
{
  m_external_keyCallback = callback;
}


void IOSubsystem::registerMouseMoveCallback( EXTERNALMOUSEMOVECALLBACK  callback  )
{
  m_external_mouseMoveCallback = callback;
}

void IOSubsystem::registerMouseScrollCallback( EXTERNALMOUSESCROLLCALLBACK callback   )
{
  m_external_mouseScrollCallback = callback;
}

void IOSubsystem::registerMouseKeyCallback( EXTERNALMOUSEKEYCALLBACK callback  )
{
  m_external_mouseKeyCallback = callback;
}