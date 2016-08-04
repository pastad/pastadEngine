#ifndef EXTERNAL_TRIGGER_SCRIPT_ELEMENT_H
#define EXTERNAL_TRIGGER_SCRIPT_ELEMENT_H

#include "ScriptElement.h"

// class for script elements

class ExternatTriggerScriptElement : ScriptElement
{

public:
  ExternatTriggerScriptElement();
  ~ExternatTriggerScriptElement();

  ExternatTriggerScriptElement(const ExternatTriggerScriptElement& other) = delete;
  ExternatTriggerScriptElement& operator=(const ExternatTriggerScriptElement& other) = delete;

  // setup
  void setup(void  (*callback)());
  
  // the update
  void update(float delta, Scene * scene, Object * object) ;

  using ScriptElement::setupAlways;
  using ScriptElement::setupFromTo;
  using ScriptElement::setupOnOff;

protected:  
   void (*external_callback)();
 
};

#endif // EXTERNAL_TRIGGER_SCRIPT_ELEMENT_H