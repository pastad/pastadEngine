#ifndef AUDIOCLIP
#define AUDIOCLIP

#include "Engine.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "portaudio.h"

#include "Subsystem.h"

#include <string>

//  class manages an audio clip


class AudioClip 
{
public:
  AudioClip();
  ~AudioClip();

  // returns true if clip was loaded successfuly
  bool load(std::string path);

  // start the clip
  void start();

  // ends the clip
  void stop();

private:

  // true if clip was loaded
  bool m_loaded;

  // the stream of the clip
  PaStream * m_stream;
};




#endif //AUDIOCLIP