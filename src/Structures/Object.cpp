#include "Object.h"

#include "Engine.h"
#include "Scene.h"
#include "Model.h"
#include "Helper.h"
#include "BoundingBox.h"

#include "Light.h"

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

bool Object::isAnimated()
{
  return m_model->isAnimated();
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
void Object::save(tinyxml2::XMLNode * parent, tinyxml2::XMLDocument* document)
{  
  tinyxml2::XMLElement * element_object = document->NewElement("Object");
  parent->InsertEndChild(element_object);

  tinyxml2::XMLElement *  element = document->NewElement("Position");
  element_object->InsertEndChild(element);;
  Helper::insertToElement(element, getPosition() );

  element = document->NewElement("Rotation");
  element_object->InsertEndChild(element);;
  Helper::insertToElement(element, getRotationDegrees() );

  element = document->NewElement("Scale");
  element_object->InsertEndChild(element);;
  Helper::insertToElement(element, getScale() );

  element = document->NewElement("Identifier");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", getIdentifier().c_str());

  element = document->NewElement("PhysicsEnabled");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", isPhysicsApplied());

  element = document->NewElement("Visibility");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", isVisible());
}

bool Object::load( tinyxml2::XMLElement *  element)
{
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  bool physics_enabled;
  bool visible;

  tinyxml2::XMLElement *child = element->FirstChildElement("Visibility");
  if( child != nullptr)
  {    
    visible = child->BoolAttribute("value");   
  } 
  child = element->FirstChildElement("PhysicsEnabled");
  if( child != nullptr)
  {    
    physics_enabled = child->BoolAttribute("value");   
  } 
  child = element->FirstChildElement("Position");
  if( child != nullptr)
  {
    Helper::readFromElement(child, &position);
  }

  child = element->FirstChildElement("Rotation");
  if( child != nullptr)
    Helper::readFromElement(child, &rotation);

  child = element->FirstChildElement("Scale");
  if( child != nullptr)
    Helper::readFromElement(child, &scale);

  setPosition(position);
  setRotation(rotation);
  setScale(scale);
  if(physics_enabled)
    applyPhysics();
  else
    dontApplyPhysics();

  if(visible)
    setVisible();
  else
    setInvisible();
  
  return true;
}
glm::vec3 Object::getMinBBDistantPoint(glm::vec3 ref)
{
  std::vector<glm::vec3> corners = m_model->getBoundingBox()->getCorners();
  glm::vec3 p_min;
  float dist_min = -1.0f;

  for(std::vector<glm::vec3>::iterator it = corners.begin(); it != corners.end();it++)
  {
    glm::vec4 p = glm::vec4((*it),1.0f);
    p = getModelMatrix() * p ;
    float dist = glm::distance(glm::vec3(p), ref);

    //std::cout << dist  <<std::endl;
    if( (dist < dist_min) || (dist_min==-1.0f)  )
    {
      dist_min = dist;
      p_min = glm::vec3(p);
    }
  }
  return p_min;
}

float Object::getAngleToLight(Light * light, glm::vec3 pos)
{
  glm::vec3 v = light->getDirection();
  v = glm::normalize(v);
  glm::vec3 dir_to_object  = glm::normalize(pos - light->getPosition() );

  float angle_mid = std::acos(glm::dot( glm::vec2(dir_to_object.x,dir_to_object.z), glm::vec2( v.x ,v.z) ) / ( glm::length(glm::vec2(dir_to_object.x,dir_to_object.z)) * glm::length(glm::vec2( v.x ,v.z) )  )); 
  return glm::degrees(angle_mid); 
}
float Object::getMinAngleToLight(Light * light)
{
  float angle = -1.0f;
  std::vector<glm::vec3> corners = m_model->getBoundingBox()->getCorners();

  for(std::vector<glm::vec3>::iterator it = corners.begin(); it != corners.end();it++)
  {
    glm::vec4 p = glm::vec4((*it),1.0f);
    p = getModelMatrix() * p ;
    float an = getAngleToLight(light, glm::vec3(p));
    if( (angle==-1) || an < angle )
      angle = an;
    std::cout <<an <<std::endl;
  }
  return angle;
}