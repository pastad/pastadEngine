#include "Wave.h"

#include "Log.h"
#include "Engine.h"

#define M_PI  3.14159265358979323846264338327950288


#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

Wave::Wave(glm::vec2 direction, float length, float speed, float amplitude): m_direction(direction), m_length(length), m_speed(speed), m_amplitude(amplitude)
{ 
  refresh();
}

Wave::~Wave()
{ 
}

void Wave::refresh()
{
  m_frequency =  (2.0f * M_PI) / m_length;
  std::cout << m_frequency<<std::endl;
  m_phase_constant = m_speed * m_frequency;
}

glm::vec2 Wave::getDirection()
{
  return m_direction;
}
float Wave::getAmplitude()
{
  return m_amplitude;
}
float Wave::getFrequency()
{
  return m_frequency;
}
float Wave::getPhaseConstant()
{
  return m_phase_constant;
}
