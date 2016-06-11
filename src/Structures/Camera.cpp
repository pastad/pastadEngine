#include "Camera.h"

#include "Engine.h"
#include "Log.h"
#include "Scene.h"
#include "Helper.h"
#include "IOSubsystem.h"
#include "Plane.h"
#include "BoundingBox.h"
#include "Object.h"

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
  m_exposure = 1.0f;
  m_plane_top = new Plane();
  m_plane_bottom = new Plane();
  m_plane_left = new Plane();
  m_plane_right = new Plane();
  m_plane_front = new Plane();
  m_plane_back = new Plane();

  float ratio = (float)Engine::getWindowHeight() / (float)Engine::getWindowWidth();
  float tan = std::tan( glm::radians(m_fov) *1.5f   );
  m_near_plane_height = NEAR_CLIPPING_PLANE * tan;
  m_near_plane_width = m_near_plane_height *ratio;
  m_far_plane_height = FAR_CLIPPING_PLANE * tan;
  m_far_plane_width = m_far_plane_height *ratio;

  //std::cout << m_far_plane_height << ","<<m_far_plane_width<<std::endl;

  recalculateMatrices();
}

Camera::~Camera()
{
  delete m_plane_top;
  delete m_plane_back;
  delete m_plane_front;
  delete m_plane_right;
  delete m_plane_left;
  delete m_plane_bottom;
}

void Camera::update(float delta_time)
{
  if(! Engine::isGUIMovementLockSet())
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

    m_forward = Helper::anglesToDirection(m_rot_1,m_rot_2);
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
  recalculatePlanes();
}
void Camera::setRotationAllowed()
{
  m_rotation_allowed = true;
}
void Camera::setRotationNotAllowed()
{
  m_rotation_allowed = false;
}
float Camera::getExposure()
{
  return m_exposure;
}
void Camera::setExposure(float v)
{
  m_exposure  = v;
}
void Camera::recalculatePlanes()
{
 glm::vec3 look_inv = m_forward * -1.0f;
 look_inv = glm::normalize(look_inv);

 glm::vec3 x_dir = glm::cross( m_up , look_inv);

 //std::cout <<x_dir.x<<","<<x_dir.y<<","<<x_dir.z<<std::endl;
 x_dir = glm::normalize(x_dir);

 glm::vec3 y_dir = glm::cross(look_inv , x_dir);

 glm::vec3 near_plane_center = m_pos - look_inv * NEAR_CLIPPING_PLANE;
 glm::vec3 far_plane_center = m_pos - look_inv * FAR_CLIPPING_PLANE;


 glm::vec3 near_plane_top_left = near_plane_center + y_dir * m_near_plane_height - x_dir * m_near_plane_width;
 glm::vec3 near_plane_top_right = near_plane_center + y_dir * m_near_plane_height + x_dir * m_near_plane_width;
 glm::vec3 near_plane_bottom_left = near_plane_center - y_dir * m_near_plane_height - x_dir * m_near_plane_width;
 glm::vec3 near_plane_bottom_right = near_plane_center - y_dir * m_near_plane_height + x_dir * m_near_plane_width;

 glm::vec3 far_plane_top_left = far_plane_center + y_dir * m_far_plane_height - x_dir * m_far_plane_width;
 glm::vec3 far_plane_top_right = far_plane_center + y_dir * m_far_plane_height + x_dir * m_far_plane_width;
 glm::vec3 far_plane_bottom_left = far_plane_center - y_dir * m_far_plane_height - x_dir * m_far_plane_width;
 glm::vec3 far_plane_bottom_right = far_plane_center - y_dir * m_far_plane_height + x_dir * m_far_plane_width;

 m_plane_front->setPoints(far_plane_top_right,far_plane_top_left,far_plane_bottom_left);
 m_plane_back->setPoints(near_plane_top_left, near_plane_top_right, near_plane_bottom_right);
 m_plane_right->setPoints(near_plane_bottom_right,near_plane_top_right,far_plane_bottom_right);
 m_plane_bottom->setPoints(near_plane_bottom_left,near_plane_bottom_right,far_plane_bottom_right);
 m_plane_left->setPoints(near_plane_top_left,near_plane_bottom_left,far_plane_bottom_left);
 m_plane_top->setPoints(near_plane_top_right,near_plane_top_left,far_plane_top_left);

}
unsigned int  Camera::insideFrustrum(Object * obj)
{
  unsigned int ret = FRUSTRUM_INCLUTION;
  std::vector<glm::vec3> corners = obj->getAABBCorners();

  std::vector<Plane *> planes;
  planes.push_back(m_plane_front);
  planes.push_back(m_plane_back);
  planes.push_back(m_plane_top);
  planes.push_back(m_plane_bottom);
  planes.push_back(m_plane_left);
  planes.push_back(m_plane_right);

  for(int x=0;x<6;x++)
  {
    Plane * check_plane = planes.at(x);
    int num_outside =0;
    int num_inside =0;
    for(std::vector<glm::vec3>::iterator it_corners = corners.begin(); it_corners != corners.end();it_corners++)
    {
      if(  check_plane->getDistance( (*it_corners)  ) <  0  )
        num_outside++;
      else
      {
        //std::cout <<check_plane->getDistance( (*it_corners)  )<<std::endl;
        num_inside++;
      }
    }
    if( num_inside ==0 )
    {
      //std::cout << "outside"<<std::endl;
      return FRUSTRUM_NO_INTERSECTION;
    }
    else
    {
      if(num_outside >0)
        ret = FRUSTRUM_INTERSECTION;
    }



  }
  return ret;
}
unsigned int Camera::insideFrustrum(BoundingBox * bb)
{
  unsigned int ret = FRUSTRUM_INCLUTION;
  std::vector<glm::vec3> corners = bb->getCorners();

  std::vector<Plane *> planes;
  planes.push_back(m_plane_front);
  planes.push_back(m_plane_back);
  planes.push_back(m_plane_top);
  planes.push_back(m_plane_bottom);
  planes.push_back(m_plane_left);
  planes.push_back(m_plane_right);

  for(int x=0;x<6;x++)
  {
    Plane * check_plane = planes.at(x);
    int num_outside =0;
    int num_inside =0;
    for(std::vector<glm::vec3>::iterator it_corners = corners.begin(); it_corners != corners.end();it_corners++)
    {
      if(  check_plane->getDistance( (*it_corners)  ) <  0  )
        num_outside++;
      else
      {
        //std::cout <<check_plane->getDistance( (*it_corners)  )<<std::endl;
        num_inside++;
      }
    }
    if( num_inside ==0 )
    {
      //std::cout << "outside"<<std::endl;
      return FRUSTRUM_NO_INTERSECTION;
    }
    else
    {
      if(num_outside >0)
        ret = FRUSTRUM_INTERSECTION;
    }



  }
  return ret;
}