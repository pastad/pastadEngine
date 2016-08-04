#ifndef ROTATION_SCRIPT_ELEMENT_H
#define ROTATION_SCRIPT_ELEMENT_H

#include "ScriptElement.h"

// class for script elements

class RotationScriptElement : ScriptElement
{

public:
  RotationScriptElement();
  ~RotationScriptElement();

  RotationScriptElement(const RotationScriptElement& other) = delete;
  RotationScriptElement& operator=(const RotationScriptElement& other) = delete;

  // setup
  void setup(glm::vec3 rotation);

  // the update
  void update(float delta, Scene * scene, Object * object) ;

  using ScriptElement::setupAlways;
  using ScriptElement::setupFromTo;
  using ScriptElement::setupOnOff;

protected:  

  // the vecor to rotate 
  glm::vec3 m_rotation_vector;
};

#endif // ROTATION_SCRIPT_ELEMENT_H