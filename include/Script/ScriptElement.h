#ifndef SCRIPT_ELEMENT_H
#define SCRIPT_ELEMENT_H

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Scene;
class Object;

enum
{
  TRIGGER_ALWAYS,
  TRIGGER_FROM_TO,
  TRIGGER_ON_OFF
};

// class for script elements

class ScriptElement
{

public:
  ScriptElement();
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