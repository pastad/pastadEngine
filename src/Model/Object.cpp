#include "Object.h"

#include "Engine.h"
#include "Scene.h"
#include "Model.h"
#include "Helper.h"
#include "BoundingBox.h"

#include "Light.h"
#include "Camera.h"
#include "Script.h"

#include <iostream>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


Object::Object(std::string identifier, Model * parent, Scene * parent_scene):Transform(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(1,1,1)),m_identifier(identifier)
{	
  m_model = parent;
  m_parent_scene = parent_scene;
  m_physics_static = false;
  refreshAABB();
  m_script = nullptr;
 // std::cout << "objct constructor done"<<std::endl;  
}

Object::~Object()
{	
  if(m_aabb != nullptr)
    delete m_aabb;
}


//  manipulation -------------------------------------------------

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
  refreshAABB();
}


//  load/save -------------------------------------------------

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

  element = document->NewElement("PhysicsStatic");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", isPhysicsStatic());

  element = document->NewElement("Visibility");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", isVisible());

  element = document->NewElement("Static");
  element_object->InsertEndChild(element);
  element->SetAttribute("value", isStaticFlagSet());
}

bool Object::load( tinyxml2::XMLElement *  element)
{
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

  bool physics_enabled = false;
  bool physics_static = false;
  bool visible = true;
  bool static_flag = false;


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
  child = element->FirstChildElement("PhysicsStatic");
  if( child != nullptr)
  {    
    physics_static = child->BoolAttribute("value");   
  } 
  child = element->FirstChildElement("Static");
  if( child != nullptr)
  {    
    static_flag = child->BoolAttribute("value");   
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

  if(physics_static)
    applyPhysicsStatic();
  else
    dontApplyPhysicsStatic();

  if(visible)
    setVisible();
  else
    setInvisible();

  if(static_flag)
    setStaticFlag();
  else
    unsetStaticFlag();
  std::cout << "objct loading done"<<std::endl; 
  return true;
}


//  calcs/refreshs/advance -------------------------------------------------

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

float Object::getAngleTo(glm::vec3 dir, glm::vec3 lp , glm::vec3 pos)
{
  glm::vec3 v = dir;
  v = glm::normalize(v);
  glm::vec3 dir_to_object  = glm::normalize(pos - lp );

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

    //std::cout << "p"<< p.x<<","<< p.y<<","<<p.z<<std::endl;
    p = getModelMatrix() * p ;
    float an = getAngleTo(light->getDirection(), light->getPosition(), glm::vec3(p));
    if( (angle==-1) || an < angle )
      angle = an;
   // std::cout <<an <<std::endl;
  }
  return angle;
}

float Object::getMinAngleToCamera(Camera * cam)
{
  float angle = -1.0f;
  std::vector<glm::vec3> corners = m_model->getBoundingBox()->getCorners();

  for(std::vector<glm::vec3>::iterator it = corners.begin(); it != corners.end();it++)
  {
    glm::vec4 p = glm::vec4((*it),1.0f);

    //std::cout << "p"<< p.x<<","<< p.y<<","<<p.z<<std::endl;
    p = getModelMatrix() * p ;
    float an = getAngleTo(cam->getDirection(),cam->getPosition(), glm::vec3(p));
    if( (angle==-1) || an < angle )
      angle = an;
    //std::cout <<an <<std::endl;
  }
  return angle;
}

void Object::refreshAABB()
{
  std::vector<glm::vec3> corners = getCorners();

  float min_x = 0;
  float max_x = 0;
  float min_y = 0;
  float max_y = 0;
  float min_z = 0;
  float max_z = 0;
  bool f = true;


  for( std::vector<glm::vec3>::iterator it = corners.begin(); it != corners.end();it++)
  {
    glm::vec3 p_1 = (*it);

    if( f|| (min_x > p_1.x) ) 
      min_x = p_1.x;
    if( f|| (min_y > p_1.y) ) 
      min_y = p_1.y;
    if( f|| (min_z > p_1.z) ) 
      min_z = p_1.z;

    if( f || (max_x < p_1.x) ) 
      max_x = p_1.x;
    if( f || (max_y < p_1.y) ) 
      max_y = p_1.y;
    if( f|| (max_z < p_1.z) ) 
      max_z = p_1.z;

    f= false;
  }
  //std::cout << "aabb"<<min_x<<std::endl;

  if(m_aabb == nullptr)
    m_aabb =  new BoundingBox(min_x,max_x,min_y,max_y,min_z,max_z);
  else
    m_aabb->setBoundingPoints(min_x,max_x,min_y,max_y,min_z,max_z);

}

void Object::advanceAnimation(float delta)
{  
  if( m_model->isAnimated() )
  {  
    m_animation_time += delta ; 
    if( m_animation_time > (float)m_model->getAssimpScene()->mAnimations[0]->mDuration )
      m_animation_time = 0.0f;
  }
}

void Object::update(float delta, Scene * scene)
{
  if(m_script != nullptr)
  {
    m_script->update(delta, scene, this);
  }
}


//  getter/setters -------------------------------------------------

// aabb

std::vector<glm::vec3> Object::getAABBCorners()
{
  return m_aabb->getCorners();
}

BoundingBox * Object::getAABB()
{
  return m_aabb;
}


// extraction (tmp) flag

void Object::setExtractionFlag()
{
  m_extraction_flag = true;
}

void Object::unsetExtractionFlag()
{
  m_extraction_flag = false;
}

bool Object::isExtractionFlagSet()
{
  return m_extraction_flag;
}

// static flag

void Object::setStaticFlag()
{
  m_static = true;
}

void Object::unsetStaticFlag()
{
  m_static  = false;
}

bool Object::isStaticFlagSet()
{
  return m_static;
}

// pos/rot/scale

void Object::setPosition(glm::vec3 p)
{
    if (this == NULL)
        std::cout << "wtf" << std::endl;
  Transform::setPosition(p);
  refreshAABB();
  m_needs_fall_check = true;
  if(Engine::getScene() != nullptr)
    Engine::getScene()->refreshLights();
}

void Object::setRotation(glm::vec3 r)
{
  Transform::setRotation(r);
  refreshAABB();
  if(Engine::getScene() != nullptr)
    Engine::getScene()->refreshLights();
}


void Object::setScale(glm::vec3 s)
{
  Transform::setScale(s);
  refreshAABB();  
  if(Engine::getScene() != nullptr)
    Engine::getScene()->refreshLights();
}

glm::mat4 Object::getModelMatrix()
{
  return Transform::getModelMatrix();
}

// parent model

Model * Object::getModel()
{
  return m_model;  
}
void Object::setModel(glm::mat4 model)
{
  m_model_matrix = model;
}

// fall

void Object::applyDrop(glm::vec3 drop)
{
  m_fall_vector+=drop;
  move(m_fall_vector);
}

void Object::setDropVector(glm::vec3 value)
{
  m_fall_vector = value;
}

glm::vec3 Object::getFallVector()
{
  return m_fall_vector;
}

bool Object::shouldFallBeChecked()
{
  bool val = m_needs_fall_check;
  m_needs_fall_check = false;
  return val;
}

// bb corners

std::vector<glm::vec3> Object::getCorners()
{
  std::vector<glm::vec3> r2 = m_model->getBoundingBox()->getCorners();
  std::vector<glm::vec3> r;
  for(std::vector<glm::vec3>::iterator it = r2.begin(); it != r2.end();it++)
  {
    glm::vec4 p = glm::vec4((*it),1.0f);

    //std::cout << "p"<< p.x<<","<< p.y<<","<<p.z<<std::endl;
    p = getModelMatrix() * p ;
    r.push_back( glm::vec3(p) );
  }
  return r;
}

// animation

bool Object::isAnimated()
{
  return m_model->isAnimated();
}

float Object::getAnimationTime()
{  
  return m_animation_time;
}

// id

void Object::setId(int id)
{
  m_id = id;
}
int Object::getId()
{
  return m_id;
}

// priority render

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

// visibility

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

// physics

void Object::applyPhysics()
{  
  m_physics_enabled =true;
}
void Object::dontApplyPhysics()
{
  m_physics_enabled = false;
}

void Object::applyPhysicsStatic()
{
  m_physics_static =true;
}
void Object::dontApplyPhysicsStatic()
{
  m_physics_static = false;
}

bool Object::isPhysicsApplied()
{
  return m_physics_enabled;
}
bool Object::isPhysicsStatic()
{
  return m_physics_static;
}

// script

Script *  Object::addScript()
{
  m_script = new Script();
  m_parent_scene->objectIsScripted(this);
  return m_script;
}

void  Object::removeScript()
{
  delete m_script;
  m_script = nullptr;
}

void Object::setShadowRenderOnly()
{
  m_shadow_render_only = true;
}

void Object::setNotShadowRenderOnly()
{
  m_shadow_render_only = false;
}

bool Object::isOnlyShadowRendered()
{
  return m_shadow_render_only;
}
