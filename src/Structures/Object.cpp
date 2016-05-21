#include "Object.h"

#include "Engine.h"
#include "Scene.h"
#include "Model.h"
#include "Helper.h"

#include <iostream>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


Object::Object(std::string identifier, Model * parent):Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1)),m_identifier(identifier)
{	
  m_model = parent;
}

Object::~Object()
{	
}

glm::mat4 Object::getModelMatrix()
{
	return Transform::getModelMatrix();
}

void Object::setPriorityRender()
{
  m_priority_render = true;
  if( Engine::getScene() != nullptr)
    Engine::getScene()->refreshRenderObjects();
}
void Object::unsetPriorityRender()
{
  m_priority_render = false;
  if( Engine::getScene() != nullptr) 
    Engine::getScene()->refreshRenderObjects();
}
bool Object::getPriorityRender()
{
  return m_priority_render;
}
std::string Object::getIdentifier()
{
  return m_identifier;
}
void Object::setVisible()
{
  m_visible = true;
  if( Engine::getScene() != nullptr)
    Engine::getScene()->refreshRenderObjects();
}
void Object::setInvisible()
{
  m_visible = false;
  if( Engine::getScene() != nullptr)
    Engine::getScene()->refreshRenderObjects();
}
bool Object::isVisible()
{
  return m_visible;
}
void Object::applyPhysics()
{
  m_physics_enabled =true;
}
void Object::dontApplyPhysics()
{
  m_physics_enabled = false;
}

bool Object::isPhysicsApplied()
{
  return m_physics_enabled;
}

void Object::move(glm::vec3 direction)
{
  setPosition( getPosition() + direction  );
}

void Object::rotate(glm::vec3 delta)
{
  glm::vec3 rot = getRotation();
  rot = glm::vec3( glm::degrees(rot.x),glm::degrees(rot.y),glm::degrees(rot.z)  );
  rot+=delta;
  if(rot.x > 360.0f)  
    rot.x -= 360.0f;
  
  if(rot.y > 360.0f)
    rot.y -= 360.0f;
  
  if(rot.z > 360.0f)
    rot.z -= 360.0f;

  if(rot.x < 0.0f)  
    rot.x += 360.0f;
  if(rot.y < 0.0f)  
    rot.y += 360.0f;
  if(rot.z < 0.0f)  
    rot.z += 360.0f;

  setRotation( rot );
  
}

float Object::getAnimationTime()
{  
  return m_animation_time;
}
void Object::advanceAnimation(float delta)
{  
  if( m_model->isAnimated() )
  {  
    m_animation_time += delta ; //4.0 speed up
    if( m_animation_time > (float)m_model->getAssimpScene()->mAnimations[0]->mDuration )
      m_animation_time = 0.0f;

    //std::cout << m_animation_time<<std::endl;
  }
}

void Object::setId(int id)
{
  m_id = id;
}

int Object::getId()
{
  return m_id;
}

void Object::setModel(glm::mat4 model)
{
  m_model_matrix = model;
}
