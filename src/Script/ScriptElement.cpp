#include "ScriptElement.h"

#include "Scene.h"
#include "Object.h"

#include <iostream>
#include <sstream>

ScriptElement::ScriptElement(ScriptElementType type): m_type(type),m_trigger(-1),m_execute(false),m_time_delta_on_off(0.0f),m_ended(false),m_wait_for_trigger(true)
{
  
}
ScriptElement::~ScriptElement()
{
  
}

ScriptElementType ScriptElement::getType()
{
  return m_type;
}

void ScriptElement::setupAlways()
{
  m_trigger = TRIGGER_ALWAYS;
  std::cout << "trigger" << m_trigger<<std::endl;
}
void ScriptElement::setupFromTo(float start , float end)
{
  m_trigger = TRIGGER_FROM_TO;
  m_start_time = start;
  m_end_time = end;
  std::cout << "trigger" << m_trigger << std::endl;
}
void ScriptElement::setupOnOff(float time_span_on , float time_span_off)
{
  m_trigger = TRIGGER_ON_OFF;
  m_time_span_on = time_span_on;
  m_time_span_off = time_span_off;

  m_execute = true;
  std::cout << "trigger" << m_trigger << std::endl;
}

void ScriptElement::updateInternal(float delta, Scene * scene, Object * object)
{
  if(m_wait_for_trigger)
  {
    if(m_trigger == TRIGGER_FROM_TO)
    {
      float world_time = scene->getTime();
    //  std::cout << "" << world_time << std::endl;
      if( (world_time >= m_start_time) && ( world_time <= m_end_time) && ( !m_execute) )
      {
        std::cout<< "triggered at" << world_time <<std::endl;
        m_execute = true;
      }
   //   else
  //      m_execute = false;

      if(world_time > m_end_time && (m_execute) )
      {
        m_ended = true;
        m_execute = false;
        std::cout << "notify successors at" << world_time << std::endl;
        notifySuccessors();
      }
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
        else
          m_time_delta_on_off += delta;
      }
      else
      {
        if(m_time_span_off < m_time_delta_on_off)
        {
          m_time_delta_on_off-= m_time_span_off;
          m_execute = true;
        }
        else
          m_time_delta_on_off += delta;
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
  std::cout << "triggered successor" << std::endl;
  m_wait_for_trigger = false;
  m_execute = true;
}
void ScriptElement::waitForTrigger()
{
  m_wait_for_trigger = true;
  m_trigger = TRIGGER_WAIT;
}

bool ScriptElement::isEnded()
{
  return m_ended;
}

void ScriptElement::save(tinyxml2::XMLElement * element)
{
  element->SetAttribute("type", getType());
  element->SetAttribute("trigger", m_trigger );
  if (m_trigger == TRIGGER_FROM_TO)
  {
    element->SetAttribute("from", m_start_time);
    element->SetAttribute("end", m_end_time);
  }
  if (m_trigger == TRIGGER_ON_OFF)
  {
    element->SetAttribute("on", m_start_time);
    element->SetAttribute("off", m_end_time);
  }
}
bool ScriptElement::load(tinyxml2::XMLElement * element)
{
  unsigned int trigger = 9999;

  element->QueryUnsignedAttribute("trigger", &trigger);
  if (trigger != 9999)
  {
    m_trigger = trigger;
        
    if (m_trigger == TRIGGER_ALWAYS)
    {
      setupAlways();
    }
    if (m_trigger == TRIGGER_FROM_TO)
    {
      float f,t;
      element->QueryFloatAttribute("from", &f);
      element->QueryFloatAttribute("to", &t);
      setupFromTo(f,t);
    }
    if (m_trigger == TRIGGER_ON_OFF)
    {
      float on,off;
      element->QueryFloatAttribute("on", &on);
      element->QueryFloatAttribute("off", &off);
      setupOnOff(on,off);
    }

  }
  else
    return false;

  return true;
}

std::string ScriptElement::getDescription()
{
  std::stringstream ss;
  if( m_type == SE_ROTATION )
    ss << "RotationScriptElement ";
  
  if( m_trigger == TRIGGER_ALWAYS )
    ss << " always";
  if (m_trigger == TRIGGER_FROM_TO)
    ss << " from-to";
  if (m_trigger == TRIGGER_ON_OFF)
    ss << " on-off";

  return ss.str();
}
std::vector< ScriptElement *> ScriptElement::getSuccessors()
{
  return m_successors;
}
int  ScriptElement::getTrigger()
{
  return m_trigger;
}



float ScriptElement::getStartTime()
{
  return m_start_time;
}
float ScriptElement::getEndTime()
{
  return m_end_time;
}
float ScriptElement::getTimeSpanOn()
{
  return m_time_span_on;
}
float ScriptElement::getTimeSpanOff()
{
  return m_time_span_off;
}

void ScriptElement::reset()
{
  m_ended = false;
  m_execute = false;
  m_wait_for_trigger = true;
}