#include "ExternalTriggerScriptElement.h"

#include "Object.h"

#include <iostream>

ExternatTriggerScriptElement::ExternatTriggerScriptElement(): ScriptElement()
{
  external_callback = nullptr;
}

ExternatTriggerScriptElement::~ExternatTriggerScriptElement()
{}

void ExternatTriggerScriptElement::update(float delta, Scene * scene, Object * object)
{
  ScriptElement::updateInternal(delta,scene,object);
  if(m_trigger == TRIGGER_ALWAYS)
  {
    m_ended = true;
    if(external_callback != nullptr)
      external_callback();
  }
  if(  (m_trigger == TRIGGER_ON_OFF) || (m_trigger == TRIGGER_FROM_TO) )
  {
    if(m_execute)
    {
      m_ended = true;
      if(external_callback != nullptr)
        external_callback();
    }
  }
}

void ExternatTriggerScriptElement::setup( void  (*callback)() )
{
  external_callback = callback;
}