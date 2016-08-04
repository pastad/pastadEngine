#include "RotationScriptElement.h"

#include "Object.h"

#include <iostream>

RotationScriptElement::RotationScriptElement(): ScriptElement()
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
  if(  (m_trigger == TRIGGER_ON_OFF) || (m_trigger == TRIGGER_FROM_TO) )
  {
    if(m_execute)
      object->rotate(m_rotation_vector*delta);
  }
}

void RotationScriptElement::setup(glm::vec3 rotation)
{
  m_rotation_vector = rotation;
}