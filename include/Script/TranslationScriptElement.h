#ifndef TRANSLATIO_SCRIPT_ELEMENT_H
#define TRANSLATIO_SCRIPT_ELEMENT_H


#include "ScriptElement.h"

class TranslationScriptElement :public ScriptElement
{
public:
  TranslationScriptElement();
  ~TranslationScriptElement();

  TranslationScriptElement(const TranslationScriptElement& other) = delete;
  TranslationScriptElement& operator=(const TranslationScriptElement& other) = delete;

  // setup
  void setup(glm::vec3 target, float time );

  // the update
  void update(float delta, Scene * scene, Object * object);

  // usings
  using ScriptElement::setupAlways;
  using ScriptElement::setupFromTo;
  using ScriptElement::setupOnOff;

  // saves the rotation script
  void save(tinyxml2::XMLElement * element);

  // loads the rotation script
  bool load(tinyxml2::XMLElement * element);

  // returns or sets the target
  glm::vec3 getTarget();
  void setTarget(glm::vec3 target);

  // returns the time
  float getTime();

private:
  
  // the target of the translation
  glm::vec3 m_target;
  glm::vec3 m_step;

  // the time the object has to move
  float m_time;

  // true if first
  bool m_first = true;
};

#endif // TRANSLATIO_SCRIPT_ELEMENT_H