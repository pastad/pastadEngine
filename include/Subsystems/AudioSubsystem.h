#ifndef AUDIOSUBSYSTEM
#define AUDIOSUBSYSTEM

#include "Engine.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Subsystem.h"

#include <string>

// Subsytem for physics,collision operations


class AudioSubsystem : public Subsystem
{
public:
  AudioSubsystem();
  ~AudioSubsystem();

  // from base class
  using Subsystem::systemCheck;


  // --- Additionals ---

  // starts up the render system
  bool startUp();
  bool shutDown();

private:

  
};




#endif //AUDIOSUBSYSTEM