#include "IOSubsystem.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool IOSubsystem::m_keys[GLFW_KEY_LAST + 1 ];
bool IOSubsystem::m_mouse_buttons[GLFW_MOUSE_BUTTON_8 + 1 ];
double IOSubsystem::m_mouse_x = -1;
double IOSubsystem::m_mouse_y= -1;
glm::vec2 IOSubsystem::m_mouse_delta;

IOSubsystem::IOSubsystem()
{
	m_initialized = false;
}

IOSubsystem::~IOSubsystem()
{
}


bool IOSubsystem::startUp(GLFWwindow * window)
{
	if(! m_initialized)
	{
		// init system here
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
		m_initialized = true;
		Engine::getLog()->log("IOSubsystem", "started");
		return true;
	}
	return false;
}

bool IOSubsystem::shutDown()
{
	if(m_initialized)
	{

		m_initialized = false;
		Engine::getLog()->log("IOSubsystem", "shut down");
		return true;
	}
	return false;
}

bool IOSubsystem::systemCheck()
{
	return m_initialized;
}

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
void IOSubsystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    Engine::errorShutDown();

  if(action == GLFW_PRESS)
  {
    m_keys[key] = true;
  }
  if(action == GLFW_RELEASE)
  {
    m_keys[key] = false;
  }
}
void IOSubsystem::mouseMoveCallback(GLFWwindow* window, double x, double y )
{
 // std::stringstream ss;
 // ss << x<<" "<<y;
  //Engine::getLog()->log("IOSubsystem","mouse move",ss.str());   

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
  // std::stringstream ss;
 //ss <<m_mouse_x<<" "<< Engine::getWindowHeight() - m_mouse_y;
  //Engine::getLog()->log("IOSubsystem","mouse pressed",ss.str());   
  if(action == GLFW_PRESS)
  {        
    m_mouse_buttons[button] = true;
    Engine::checkGUIsForButtonPresses(m_mouse_x, Engine::getWindowHeight() - m_mouse_y);
  }
  if(action == GLFW_RELEASE)
  {
   // Engine::getLog()->log("IOSubsystem", "mouse button released");
    m_mouse_buttons[button] = false;
  }

}

bool IOSubsystem::isKeyPressed(int key)
{
  if(key <= GLFW_KEY_LAST)
    return m_keys[key];
  else
    return false;
}
bool IOSubsystem::isMouseButtonPressed(int key)
{
  if(key <= GLFW_MOUSE_BUTTON_8)
    return m_mouse_buttons[key];
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