#include "Script.h"

#include "Object.h"
#include "ScriptElement.h"
#include "RotationScriptElement.h"
#include "Scene.h"

Script::Script()
{  
}
Script::~Script()
{  
}

void Script::update(float delta, Scene * scene,  Object * object)
{
  for( std::vector<ScriptElement *>::iterator it = m_script_elements.begin(); it != m_script_elements.end();  )
  {
    (*it)->update(delta,scene,object);
    if( (*it)->isEnded() )
    {
      (*it)->notifySuccessors();
      delete (*it); // ehmmmm nope
      it = m_script_elements.erase(it);
    }
    else
      it++;
  }
}

RotationScriptElement * Script::addRotationScript()
{
  RotationScriptElement * rse = new RotationScriptElement();
  m_script_elements.push_back( (ScriptElement*) rse );
  return rse;
}