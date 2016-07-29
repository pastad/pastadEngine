#ifndef WAVE_H
#define WAVE_H

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

// class to represent a wave 

class Wave
{
public:
  Wave(glm::vec2 direction, float length, float speed, float amplitude);
  ~Wave();

  Wave(const Wave& other) = delete;
  Wave& operator=(const Wave& other) = delete;

  // the getters of the wave
  glm::vec2 getDirection();
  float getAmplitude();
  float getFrequency();
  float getPhaseConstant();

private:

  // the direction of the wave  
  glm::vec2 m_direction;

  // the frequency and phase constant that will be computed  
  float m_frequency;
  float m_phase_constant;

  // user params amplitude, length and speed
  float m_amplitude;
  float m_length;
  float m_speed;

  // recomputes the wave params
  void refresh();

  
};

#endif // WAVE_H