#include "IOSubsystem.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool IOSubsystem::m_keys[GLFW_KEY_LAST];
double IOSubsystem::m_mouse_x = -1;
double IOSubsystem::m_mouse_y= -1;

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
    double deltax=m_mouse_x -x;
    double deltay=m_mouse_y -y;
   // Engine::getScene()->getCamera()->rotate(deltax,deltay);
  }
}
bool IOSubsystem::isKeyPressed(int key)
{
  return m_keys[key];
}