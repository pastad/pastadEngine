#include "Camera.h"

#include "Engine.h"
#include "Log.h"
#include "IOSubsystem.h"

#include "glm/ext.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

Camera::Camera(float x, float y, float z): m_pos(x,y,z)
{
  m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
  m_up    = glm::vec3(0.0f, 1.0f,  0.0f);
  m_fov = 45.0f;
  m_rot_2 =0.0f;
  m_rot_1 = 90.0f;
  m_speed = 0.01f;
 // rotate(0.0f,0.0f);    
  recalculateMatrices();
}

Camera::~Camera()
{
}

void Camera::update()
{
  float delta = 0.1;//Engine::getInstance()->getLastDelta();
  glm::vec3 right = glm::normalize(glm::cross(m_forward, m_up));

  float step = delta * m_speed;
  bool moved = false;
  //m_pos -= m_up * 0.000001f;
  if(IOSubsystem::isKeyPressed('W'))
  {
    moved = true;
    m_pos += m_forward * step;
  }
  if(IOSubsystem::isKeyPressed('S'))
  {
    moved = true;
    m_pos -= m_forward * step;
  }
  if(IOSubsystem::isKeyPressed('A'))
  {
    moved = true;
    m_pos -= right * step;
  }
  if(IOSubsystem::isKeyPressed('D'))
  {
    moved = true;
    m_pos += right * step;
  }
  if(IOSubsystem::isKeyPressed(' '))
  {
    moved = true;
    m_pos += m_up* step;
  }
  if(IOSubsystem::isKeyPressed('V'))
  {
    moved = true;
    m_pos -= m_up* step;
  }
  if(moved)
  {
    // std::stringstream ss;
    // ss << m_pos.x <<", "<< m_pos.y <<", "<< m_pos.z << "facing "<< m_forward.x <<", "<< m_forward.y <<", "<< m_forward.z;
    //Engine::getLog()->log("Camera","moved to", ss.str());
  }
  recalculateMatrices();

}
glm::mat4 Camera::getView()
{
  return  m_view;
}
glm::mat4 Camera::getViewWithoutTranslation()
{
  return  m_view_wt;
}

glm::mat4 Camera::getProjection()
{
  return m_projection;
}
void Camera::rotate(float deltax,float deltay)
{
  m_rot_1 +=deltax*0.02f;
  m_rot_2 -=deltay*0.02f;
  glm::vec3 front;
  front.x = cos(glm::radians(m_rot_1)) * cos(glm::radians(m_rot_2));
  front.y = sin(glm::radians(m_rot_2));
  front.z = sin(glm::radians(m_rot_1)) * cos(glm::radians(m_rot_2));
  m_forward = glm::normalize(front);
  std::stringstream ss;
  ss << m_pos.x <<", "<< m_pos.y <<", "<< m_pos.z << "facing "<< m_forward.x <<", "<< m_forward.y <<", "<< m_forward.z;
  Engine::getLog()->log("Camera","moved to", ss.str());
  recalculateMatrices();
}
glm::vec3 Camera::getPosition()
{
  return m_pos;
}

void Camera::move(glm::vec3 step)
{
  m_pos+=step;
  recalculateMatrices();

  Engine::getLog()->log("Camera", "moved camera ");
}
void Camera::recalculateMatrices()
{
  m_view = glm::lookAt(m_pos, m_pos +  m_forward, m_up);
  m_view_wt = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,0) +  m_forward, m_up);
  m_projection =glm::perspective(m_fov, (float)Engine::getWindowWidth()/(float)Engine::getWindowHeight(), 0.1f, 1000.0f);
}