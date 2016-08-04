#include "ScriptElement.h"

#include "Scene.h"
#include "Object.h"

ScriptElement::ScriptElement(): m_trigger(-1),m_execute(false),m_time_delta_on_off(0.0f),m_ended(false),m_wait_for_trigger(true)
{
  
}
ScriptElement::~ScriptElement()
{
  
}

void ScriptElement::setupAlways()
{
  m_trigger = TRIGGER_ALWAYS;
}
void ScriptElement::setupFromTo(float start , float end)
{
  m_trigger = TRIGGER_FROM_TO;
  m_start_time = start;
  m_end_time = end;
}
void ScriptElement::setupOnOff(float time_span_on , float time_span_off)
{
  m_trigger = TRIGGER_ON_OFF;
  m_time_span_on = time_span_on;
  time_span_off = time_span_off;
}

void ScriptElement::updateInternal(float delta, Scene * scene, Object * object)
{
  if(!m_wait_for_trigger)
  {
    if(m_trigger == TRIGGER_FROM_TO)
    {
      float world_time = scene->getTime();
      if( (world_time >= m_start_time) && ( world_time <= m_end_time) )
        m_execute = true;
      else
        m_execute = false;

      if(world_time > m_end_time)
        m_ended = true;
    }
    if(m_trigger == TRIGGER_ON_OFF)
    {
      if(m_execute)
      {
        if(m_time_span_on < m_time_delta_on_off)
        {
          m_time_delta_on_off-= m_time_span_on;
          m_execute = false;
        }
      }
      else
      {
        if(m_time_span_off < m_time_delta_on_off)
        {
          m_time_delta_on_off-= m_time_span_off;
          m_execute = true;
        }
      }
    }
  }
}

void ScriptElement::setSuccessor(ScriptElement * elem)
{
  elem->waitForTrigger();
  m_successors.push_back(elem);
}

void ScriptElement::notifySuccessors()
{
  for(std::vector<ScriptElement *>::iterator it = m_successors.begin(); it!= m_successors.end(); it++  )
  {
    (*it)->trigger();
  }
}

void ScriptElement::trigger()
{
  m_wait_for_trigger = false;
}
void ScriptElement::waitForTrigger()
{
  m_wait_for_trigger = true;
}

bool ScriptElement::isEnded()
{
  return m_ended;
}