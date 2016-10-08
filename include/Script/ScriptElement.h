#ifndef SCRIPT_ELEMENT_H
#define SCRIPT_ELEMENT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "tinyxml2.h"

class Scene;
class Object;

enum ScriptElementType
{
  SE_ROTATION,
  SE_TRANSLATION,
  SE_EXTERNAL
};

enum
{
  TRIGGER_ALWAYS,
  TRIGGER_FROM_TO,
  TRIGGER_ON_OFF, 
  TRIGGER_WAIT
};

// class for script elements

class ScriptElement
{

public:
  ScriptElement(ScriptElementType type);
  ~ScriptElement();

  ScriptElement(const ScriptElement& other) = delete;
  ScriptElement& operator=(const ScriptElement& other) = delete;

  // the update
  virtual  void update(float delta, Scene * scene, Object * object) =0;

  // setups
  void setupAlways();
  void setupFromTo(float start , float end);
  void setupOnOff(float m_time_span_on , float m_time_span_off);

  // elem will be triggered after this one has ended
  void setSuccessor(ScriptElement * elem);

  // starts its succeeders
  void notifySuccessors();

  // triggers the element
  void trigger();

  // tells the script to wait for trigger
  void waitForTrigger();

  // true if script has ended
  bool isEnded();
    
  // returns the type of the script element
  ScriptElementType getType();

  // saves the element base
  void save(tinyxml2::XMLElement * element);

  // loads the element base
  bool load(tinyxml2::XMLElement * element);

  // returns a short description string
  std::string getDescription();

  // returns the successors
  std::vector< ScriptElement *> getSuccessors();

  // returns the set trigger
  int getTrigger();

  // time getters
  float getStartTime();
  float getEndTime();
  float getTimeSpanOn();
  float getTimeSpanOff();

  // resets the script element
  void reset();

private:
  ScriptElementType m_type;

protected:  
  
  
  // the trigger
  int m_trigger;

  // start to end for FROM_TO
  float m_start_time;
  float m_end_time;

  // length FROM REPEATED
  float m_time_span_on;
  float m_time_span_off;

  // true if it should be executed
  bool m_execute;

  // the time for on off
  float m_time_delta_on_off;

  // true of script has ended
  bool m_ended;

  // true if we should wait for a trigger
  bool m_wait_for_trigger;

  // the scripts that triggered afterwards
  std::vector<ScriptElement * > m_successors;

  // internal update
  void updateInternal(float delta, Scene * scene, Object * object);



};

#endif // SCRIPT_ELEMENT_H