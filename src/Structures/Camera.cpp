#include "Camera.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "IOSubsystem.h"

#include "glm/ext.hpp"
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>

Camera::Camera(float x, float y, float z): m_pos(x,y,z),m_rotation_allowed(false)
{
  m_forward = glm::vec3(0.0f, 0.0f, 1.0f);
  m_up    = glm::vec3(0.0f, 1.0f,  0.0f);
  m_fov = 45.0f;
  m_rot_2 =0.0f;
  m_rot_1 = 90.0f;
  m_speed = 10.0f;
  m_rotation_speed = 0.2f;  
  recalculateMatrices();
}

Camera::~Camera()
{
}

void Camera::update(float delta_time)
{
  float delta = delta_time;
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
  if(IOSubsystem::isKeyPressed('L'))
  {
    moved = true;
    rotate(5,0);
  }
  if(IOSubsystem::isKeyPressed('J'))
  {
    moved = true;
    rotate(-5,0);
  }
  if(IOSubsystem::isKeyPressed('K'))
  {
    moved = true;
    rotate(0,5);
  }
  if(IOSubsystem::isKeyPressed('I'))
  {
    moved = true;
    rotate(0,-5);
  }
  if(moved)
  {
    Engine::getScene()->cameraMoved();  
  }
  glm::vec2 mouse_movement = IOSubsystem::getMouseDelta();
  rotate(mouse_movement.x, mouse_movement.y);
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
  if(m_rotation_allowed)
  {
    m_rot_1 +=deltax*m_rotation_speed;
    m_rot_2 -=deltay*m_rotation_speed;

    // limit to range for better retrieval
    if(m_rot_1 >360.0f)
      m_rot_1 = 0.0f;
    if(m_rot_1 <0.0f)
      m_rot_1 = 360.0f;


    glm::vec3 front;
    front.x = cos(glm::radians(m_rot_1)) * cos(glm::radians(m_rot_2));
    front.y = sin(glm::radians(m_rot_2));
    front.z = sin(glm::radians(m_rot_1)) * cos(glm::radians(m_rot_2));
    m_forward = glm::normalize(front);
    Engine::getScene()->cameraRotated();
  }
}
glm::vec3 Camera::getPosition()
{
  return m_pos;
}
glm::vec3 Camera::getDirection()
{
  return m_forward;
}
float Camera::getAngleAroundY()
{
  return m_rot_1;
}
float Camera::getFOV()
{
  return m_fov;
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
  m_projection =glm::perspective(m_fov, (float)Engine::getWindowWidth()/(float)Engine::getWindowHeight(), NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
}
void Camera::setRotationAllowed()
{
  m_rotation_allowed = true;
}
void Camera::setRotationNotAllowed()
{
  m_rotation_allowed = false;
}