#ifndef ROTATION_SCRIPT_ELEMENT_H
#define ROTATION_SCRIPT_ELEMENT_H

#include "ScriptElement.h"

#include "tinyxml2.h"

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

  // returns the rotation vector
  glm::vec3 getRotationVector();

  using ScriptElement::setupAlways;
  using ScriptElement::setupFromTo;
  using ScriptElement::setupOnOff;

  // saves the rotation script
  void save(tinyxml2::XMLElement * element);

  // loads the rotation script
  bool load(tinyxml2::XMLElement * element);

protected:  

  // the vecor to rotate 
  glm::vec3 m_rotation_vector;
};

#endif // ROTATION_SCRIPT_ELEMENT_H