#include "Camera.h"

#include "Engine.h"
#include "IOSubsystem.h"
#include "PhysicSubsystem.h"

#include "Log.h"
#include "Helper.h"

#include "Scene.h"
#include "Object.h"

#include "Plane.h"
#include "BoundingBox.h"
#include "Ray.h"


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

  m_bottom_offset = 0.0f;
  m_surrounding_offset =0.0f;

  float ratio = (float)Engine::getWindowHeight() / (float)Engine::getWindowWidth();
  float tan = std::tan( glm::radians(m_fov) *1.5f   );
  m_near_plane_height = NEAR_CLIPPING_PLANE * tan;
  m_near_plane_width = m_near_plane_height *ratio;
  m_far_plane_height = FAR_CLIPPING_PLANE * tan;
  m_far_plane_width = m_far_plane_height *ratio;

  m_vertical_movement_allowed = true;
  m_up_down_movement_allowed = true;

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


//  movement-------------------------------------------------

void Camera::movementWithCollisionCheck(glm::vec3 dir, float step)
{
  PhysicSubsystem * physics_system = Engine::getPhysicSubsystem();
  Scene * scene = Engine::getScene();

  float bottom_off_halfed = 0.01f;
  if( m_bottom_offset > 0.0f)
    bottom_off_halfed = m_bottom_offset /2.0f;

  glm::vec3 npos =  m_pos + dir * step;
  if( (physics_system != nullptr) && (scene != nullptr) )
  {
    Ray r(m_pos-glm::vec3(0,m_bottom_offset-0.1f,0), dir);
    float distance;
    if( ! physics_system->collisionRayScene(scene, &r, &distance))
    {        
      // if no collision update  
      m_pos = npos;
      //std::cout << "no col"<< dir.x<<" , "<< dir.y<<" , "<< dir.z<<" , " <<std::endl;
    }
    else
    {
      // .. also update if there is enough space in front
      if(distance -step > 0.0f)
        m_pos = npos;
      else
      {
        Ray r2(m_pos-glm::vec3(0,bottom_off_halfed,0), dir, 0.0f, m_surrounding_offset);
        float distance2;
        bool res = physics_system->collisionRayScene(scene, &r2, &distance2);

        // otherwise check if we can make a step forward an upwards
        if( !res || (distance2 > m_surrounding_offset) )
        {
          Ray r3(m_pos-glm::vec3(0,bottom_off_halfed,0)+dir*step ,glm::vec3(0,-1,0));
          float distance3;

          // how far we need to step upward ?? -> distance3
          if(  physics_system->collisionRayScene(scene, &r3, &distance3))
          {
            if((bottom_off_halfed- distance3) < 0.2f)
            m_pos += glm::vec3(0,  (bottom_off_halfed- distance3),0);
          }
        }     
      }
      //std::cout << distance<<std::endl;
    }
  }
  else
  {
  /*  if(scene == nullptr)
      std::cout << "no scene"<<std::endl;
    if(physics_system == nullptr)
      std::cout << "no phy sys"<<std::endl;*/
    m_pos = npos;
  }
}

void Camera::rotate(float deltax,float deltay)
{
  if(m_rotation_allowed)
  {  

    m_rot_1 += deltax*m_rotation_speed;
    m_rot_2 -= deltay*m_rotation_speed;


    // limit to range for better retrieval
    if(m_rot_1 >360.0f)
      m_rot_1 = 0.0f;
    if(m_rot_1 <0.0f)
      m_rot_1 = 360.0f;

    // limt so that we can turn upside down
    if(m_rot_2 >89.0f)
      m_rot_2 = 89.0f;
    if(m_rot_2 <-89.0f)
      m_rot_2 = -89.0f;

    m_forward = Helper::anglesToDirection(m_rot_1,m_rot_2);
    Engine::getScene()->cameraRotated();
    if(external_cameraMovedCallback != nullptr)
      external_cameraMovedCallback();
  }
}

void Camera::move(glm::vec3 step)
{
  m_pos += step;
  recalculateMatrices();
  //Engine::getLog()->log("Camera", "moved camera ");
  if(external_cameraMovedCallback != nullptr)
    external_cameraMovedCallback();
}


//  update/recalc -------------------------------------------------

void Camera::update(float delta_time)
{
  if(! Engine::isGUIMovementLockSet())
  {
    float delta = delta_time;
    m_right = glm::normalize(glm::cross(m_forward, m_up));

    PhysicSubsystem * physics_system = Engine::getPhysicSubsystem();
    Scene * scene = Engine::getScene();

    float step = delta * m_speed;
    bool moved = false;
    //m_pos -= m_up * 0.000001f;

    if(IOSubsystem::isKeyPressed('W'))
    {
      
      glm::vec3 f = m_forward;
      if( !isUpDownTranslationAllowed()) // dont allow running upwards ...
        f.y = 0.0f;
      if(physics_system != nullptr)
      {
        glm::vec3 npos;
        if(physics_system->collisionMovement(scene, m_pos, f , step, m_surrounding_offset, m_bottom_offset, &npos))
          m_pos = npos;
        moved = true;
      }
      else
      {
        moved = true;
        m_pos =  m_pos + f * step;
      }
     // movementWithCollisionCheck(m_forward, step);
    }
    if(IOSubsystem::isKeyPressed('S'))
    {
      
      glm::vec3 f = -m_forward;
      if( !isUpDownTranslationAllowed()) // or downwards
        f.y = 0.0f;
     // movementWithCollisionCheck(-m_forward, step);
      if(physics_system != nullptr)
      {
        glm::vec3 npos;
        if(physics_system->collisionMovement(scene, m_pos, f , step, m_surrounding_offset, m_bottom_offset, &npos))
          m_pos = npos;
        moved = true;
      }
      else
      {
        moved = true;
        m_pos =  m_pos + f * step;
      }
    }
    if(IOSubsystem::isKeyPressed('A'))
    {
     // movementWithCollisionCheck(-right, step);
      if(physics_system != nullptr)
      {
        glm::vec3 npos;
        if(physics_system->collisionMovement(scene, m_pos, -m_right , step, m_surrounding_offset, m_bottom_offset, &npos))
          m_pos = npos;

        moved = true;
      }
      else
      {
        moved = true;
        m_pos =  m_pos + -m_right * step;
      }

      //m_pos -= right * step;
    }
    if(IOSubsystem::isKeyPressed('D'))
    {
      //movementWithCollisionCheck(right, step);
      if(physics_system != nullptr)
      {
        glm::vec3 npos;
        if(physics_system->collisionMovement(scene, m_pos, m_right , step, m_surrounding_offset, m_bottom_offset, &npos))
          m_pos = npos;

        moved = true;
      }
      else
      {
        moved = true;
        m_pos =  m_pos + m_right * step;
      }
    }
    if(IOSubsystem::isKeyPressed(' '))
    {
      if(m_up_down_movement_allowed)
      {
        moved = true;
        m_pos += m_up* step;
      }
    }
    if(IOSubsystem::isKeyPressed('V'))
    {
      if(m_up_down_movement_allowed)
      {
        moved = true;
        m_pos -= m_up* step;
      }
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

void Camera::recalculateMatrices()
{
  m_view = glm::lookAt(m_pos, m_pos +  m_forward, m_up);
  m_view_wt = glm::lookAt(glm::vec3(0,0,0), glm::vec3(0,0,0) +  m_forward, m_up);
  m_projection =glm::perspective(m_fov, (float)Engine::getWindowWidth()/(float)Engine::getWindowHeight(), NEAR_CLIPPING_PLANE, FAR_CLIPPING_PLANE);
  recalculatePlanes();
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


 m_near_plane_top_left = near_plane_center + y_dir * m_near_plane_height - x_dir * m_near_plane_width;
 m_near_plane_top_right = near_plane_center + y_dir * m_near_plane_height + x_dir * m_near_plane_width;
 m_near_plane_bottom_left = near_plane_center - y_dir * m_near_plane_height - x_dir * m_near_plane_width;
 m_near_plane_bottom_right = near_plane_center - y_dir * m_near_plane_height + x_dir * m_near_plane_width;

 m_far_plane_top_left = far_plane_center + y_dir * m_far_plane_height - x_dir * m_far_plane_width;
 m_far_plane_top_right = far_plane_center + y_dir * m_far_plane_height + x_dir * m_far_plane_width;
 m_far_plane_bottom_left = far_plane_center - y_dir * m_far_plane_height - x_dir * m_far_plane_width;
 m_far_plane_bottom_right = far_plane_center - y_dir * m_far_plane_height + x_dir * m_far_plane_width;

 m_plane_front->setPoints(m_far_plane_top_right,m_far_plane_top_left,m_far_plane_bottom_left);
 m_plane_back->setPoints(m_near_plane_top_left, m_near_plane_top_right, m_near_plane_bottom_right);
 m_plane_right->setPoints(m_near_plane_bottom_right,m_near_plane_top_right,m_far_plane_bottom_right);
 m_plane_bottom->setPoints(m_near_plane_bottom_left,m_near_plane_bottom_right,m_far_plane_bottom_right);
 m_plane_left->setPoints(m_near_plane_top_left,m_near_plane_bottom_left,m_far_plane_bottom_left);
 m_plane_top->setPoints(m_near_plane_top_right,m_near_plane_top_left,m_far_plane_top_left);

}



//  getter/setter -------------------------------------------------


std::vector<glm::vec3> Camera::getFrustrumCorners(float min_bound, float max_bound)
{ 

  float near_adjustment = (min_bound/ FAR_CLIPPING_PLANE );
  float far_adjustment = (max_bound/ FAR_CLIPPING_PLANE );

  std::vector<glm::vec3> points;

  glm::vec3 v1 = (m_far_plane_top_left -m_near_plane_top_left) * near_adjustment + m_near_plane_top_left; 
  glm::vec3 v2 = (m_far_plane_top_right -m_near_plane_top_right) * near_adjustment + m_near_plane_top_right;
  glm::vec3 v3 = (m_far_plane_bottom_left -m_near_plane_bottom_left) * near_adjustment + m_near_plane_bottom_left;
  glm::vec3 v4 = (m_far_plane_bottom_right -m_near_plane_bottom_right) * near_adjustment + m_near_plane_bottom_right;


  points.push_back(v1);
  points.push_back(v2);
  points.push_back(v3);
  points.push_back(v4);

  v1 = (m_far_plane_top_left -m_near_plane_top_left) * far_adjustment + m_near_plane_top_left; 
  v2 = (m_far_plane_top_right -m_near_plane_top_right) * far_adjustment + m_near_plane_top_right;
  v3 = (m_far_plane_bottom_left -m_near_plane_bottom_left) * far_adjustment + m_near_plane_bottom_left;
  v4 = (m_far_plane_bottom_right -m_near_plane_bottom_right) * far_adjustment + m_near_plane_bottom_right;

  points.push_back( v1 );
  points.push_back(v2);
  points.push_back(v3);
  points.push_back(v4);

  return points;
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

glm::vec3 Camera::getPosition()
{
  return m_pos;
}

glm::vec3 Camera::getDirection()
{
  return m_forward;
}

glm::vec3 Camera::getRight()
{
  return m_right;
}

glm::vec3 Camera::getUp()
{
  return m_up;
}

float Camera::getPitch()
{
  return m_rot_1;
}

float Camera::getYaw()
{
  return m_rot_2;
}

float Camera::getFOV()
{
  return m_fov;
}

void Camera::setRotationAllowed()
{
  Engine::getLog()->log("Camera","rotation allowed");
  m_rotation_allowed = true;
}

void Camera::setRotationNotAllowed()
{
  Engine::getLog()->log("Camera","rotation not allowed");
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

void Camera::applyPhysicsDrop(float offset)
{
  m_apply_physics = true;
  m_bottom_offset = offset;
}

void Camera::dontApplyPhysicsDrop()
{
  m_apply_physics = false;
}

bool Camera::isPhysicsApplied()
{
  return m_apply_physics;
}

float Camera::getBottomOffset()
{
  return m_bottom_offset;
}

void Camera::applyDrop(glm::vec3 delta)
{
  m_fall_vector+=delta;
  move(m_fall_vector);  
  if(Engine::getScene()!= nullptr)
    Engine::getScene()->cameraMoved();
}

void Camera::allowUpDownTranslation()
{
  m_vertical_movement_allowed = true;
}

void Camera::dontAllowUpDownTranslation()
{
  m_vertical_movement_allowed = false;
}

bool Camera::isUpDownTranslationAllowed()
{
  return m_vertical_movement_allowed;
}

glm::vec3 Camera::getFallVector()
{
  return m_fall_vector;
}

void Camera::setFallVector(glm::vec3 val)
{ 
  m_fall_vector = val;
}

float Camera::getSurrouningOffset()
{
  return m_surrounding_offset;
}

void Camera::setSurroundingOffset(float val)
{
  if(val >=0.0f)
    m_surrounding_offset = val;
}

void Camera::allowUpDownMovement()
{
  m_up_down_movement_allowed = true;
}
void Camera::dontAllowUpDownMovement()
{
  m_up_down_movement_allowed = false;
}


//  inside frustrum check -------------------------------------------------

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


//  callback register -------------------------------------------------

void Camera::registerMovedCallback( void  (*callback)(void)   )
{
  external_cameraMovedCallback = callback;
}
