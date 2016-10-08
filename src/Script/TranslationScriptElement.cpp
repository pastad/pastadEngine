#include "TranslationScriptElement.h"
#include "Helper.h"
#include "Object.h"

#include <iostream>

TranslationScriptElement::TranslationScriptElement():ScriptElement(SE_TRANSLATION)
{
}


TranslationScriptElement::~TranslationScriptElement()
{
}
void TranslationScriptElement::setup(glm::vec3 target, float time)
{
  m_target = target;
  m_time = time;
  m_first =true;
}


void TranslationScriptElement::update(float delta, Scene * scene, Object * object)
{
  ScriptElement::updateInternal(delta, scene, object);
  bool do_move = false;
  if (m_trigger == TRIGGER_ALWAYS)
  {
  //  object->rotate(m_rotation_vector*delta);
    do_move = true;
    if (m_first)
    {
      m_first = false;
      m_step =( m_target- object->getPosition() ) / m_time;
    }
  }
  if ((m_trigger == TRIGGER_ON_OFF) || (m_trigger == TRIGGER_FROM_TO) || (m_trigger == TRIGGER_WAIT))
  {
    if (m_execute)
    {
      do_move = true;
      if (m_first)
      {
        m_first = false;
        m_step = (m_target - object->getPosition()) / m_time;
      }
    }
  }
  if (do_move && !m_ended)
  {
    float dist = glm::distance(m_target, object->getPosition());

    std::cout << "up"<< delta << std::endl;
    if (dist > 0.1f)
    {

      std::cout << "exec" <<  std::endl;
      object->move(m_step* delta);
    }
    else
    {
      m_ended = true;
      m_execute = false;
      notifySuccessors();
      
    }
  }
}

void TranslationScriptElement::save(tinyxml2::XMLElement * element)
{
  ScriptElement::save(element);
  Helper::insertToElement(element, m_target);
}


bool TranslationScriptElement::load(tinyxml2::XMLElement * element)
{
  if (!ScriptElement::load(element))
    return false;

  Helper::readFromElement(element, &m_target);

  return true;
}

glm::vec3 TranslationScriptElement::getTarget()
{
  return m_target;
}
void TranslationScriptElement::setTarget(glm::vec3 target)
{
  m_target = target;
  m_first = false;
}

float TranslationScriptElement::getTime()
{
  return m_time;
}