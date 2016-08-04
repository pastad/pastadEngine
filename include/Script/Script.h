#ifndef SCRIPT_H
#define SCRIPT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

class ScriptElement;
class RotationScriptElement;
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


protected:  

  // the elements making up the script
  std::vector<ScriptElement *> m_script_elements;

};

#endif // SCRIPT_H