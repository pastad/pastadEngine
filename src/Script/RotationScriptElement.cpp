#include "RotationScriptElement.h"

#include "Object.h"
#include "Helper.h"

#include <iostream>

RotationScriptElement::RotationScriptElement(): ScriptElement(SE_ROTATION)
{}

RotationScriptElement::~RotationScriptElement()
{}

void RotationScriptElement::update(float delta, Scene * scene, Object * object)
{
  ScriptElement::updateInternal(delta,scene,object);
  if(m_trigger == TRIGGER_ALWAYS)
  {
    object->rotate(m_rotation_vector*delta);
  }
  if(  (m_trigger == TRIGGER_ON_OFF) || (m_trigger == TRIGGER_FROM_TO) || (m_trigger == TRIGGER_WAIT))
  {
    if(m_execute)
      object->rotate(m_rotation_vector*delta);
  }
}

void RotationScriptElement::setup(glm::vec3 rotation)
{
  m_rotation_vector = rotation;
}

glm::vec3 RotationScriptElement::getRotationVector()
{
  return m_rotation_vector;
}

void RotationScriptElement::save(tinyxml2::XMLElement * element)
{
  ScriptElement::save(element);
  Helper::insertToElement(element, m_rotation_vector);
}
bool RotationScriptElement::load(tinyxml2::XMLElement * element)
{
  if(!ScriptElement::load(element))
    return false;

  Helper::readFromElement(element, & m_rotation_vector);

  return true;
}