#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "tinyxml2.h"

class ScriptElement;
class RotationScriptElement;
class TranslationScriptElement;
class Object;
class Scene;

// class for scripting

class Script
{

public:
  Script();
  ~Script();

  Script(const Script& other) = delete;
  Script& operator=(const Script& other) = delete;

  // updates the script
  void update(float delta, Scene * scene, Object * object);

  // adds a RotationScript
  RotationScriptElement * addRotationScript();

  // adds a TranslationScript
  TranslationScriptElement * addTranslationScript();

  // returns all script elements
  std::vector<ScriptElement *> getScriptElements();

  // removes a script element
  void removeScriptElement(ScriptElement * se);

  // saves the script
  void save(tinyxml2::XMLElement * element, tinyxml2::XMLDocument* document);

  // loads the script
  bool load(tinyxml2::XMLElement *  element);

  // resets all scripts
  void reset();

protected:  

  // the elements making up the script
  std::vector<ScriptElement *> m_script_elements;

};

#endif // SCRIPT_H