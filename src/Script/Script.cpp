#include "Script.h"

#include "Object.h"
#include "ScriptElement.h"
#include "RotationScriptElement.h"
#include "TranslationScriptElement.h"
#include "Scene.h"
#include "Engine.h"

#include <iostream>

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
      //(*it)->notifySuccessors();
      delete (*it); 
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
TranslationScriptElement * Script::addTranslationScript()
{
  TranslationScriptElement * rse = new TranslationScriptElement();
  m_script_elements.push_back((ScriptElement*)rse);
  return rse;
}

std::vector<ScriptElement *> Script::getScriptElements()
{
  return m_script_elements;
}

void Script::removeScriptElement(ScriptElement * se)
{
  for (std::vector<ScriptElement *>::iterator it = m_script_elements.begin(); it != m_script_elements.end(); )
  {
    if( (*it) ==se )
      it = m_script_elements.erase(it);
    else
      it++;
  }
}

void Script::save(tinyxml2::XMLElement * element_script, tinyxml2::XMLDocument* document)
{
  for (std::vector<ScriptElement *>::iterator it = m_script_elements.begin(); it != m_script_elements.end(); it++)
  {
    tinyxml2::XMLElement *  element = document->NewElement("ScriptElement");
    element_script->InsertEndChild(element);;
    
    if( (*it)->getType() == SE_ROTATION)
      ( (RotationScriptElement *) (*it) )->save(element);

  }
}
bool Script::load(tinyxml2::XMLElement *  element)
{
  tinyxml2::XMLElement * child = element->FirstChildElement("ScriptElement");
  while (child != nullptr)
  {
    unsigned int type = 9999;
    child->QueryUnsignedAttribute("type", &type);
    if (type != 9999)
    {
      if (((ScriptElementType)type) == SE_ROTATION)
      {
        RotationScriptElement * rse =  addRotationScript();
        if( ! rse->load(child))
         removeScriptElement( (ScriptElement *) rse);

      }

    }


    child = child->NextSiblingElement();
  }
  return true;
}

void Script::reset()
{
  for (std::vector<ScriptElement *>::iterator it = m_script_elements.begin(); it != m_script_elements.end(); it++)
  {
    (*it)->reset();
  }
}
