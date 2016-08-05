#include "AudioSubsystem.h"

#include "Engine.h"
#include "Log.h"

#include "portaudio.h"

#include <iostream>

#define EPSILON 0.000001

AudioSubsystem::AudioSubsystem():Subsystem("AudioSubsystem")
{
}

AudioSubsystem::~AudioSubsystem()
{
}

bool AudioSubsystem::startUp()
{
  if(!m_initialized)
  {
    int error = Pa_Initialize();

    if( error != paNoError )
    {
      Engine::getLog()->log("AudioSubsystem", "couldn't be started");
      printf(  "PortAudio error: %s\n", Pa_GetErrorText( error ) );
      return false;
    }    

    m_initialized = true;
    Engine::getLog()->log("AudioSubsystem", "started");
    return true;
  }
  return false;
}

bool AudioSubsystem::shutDown()
{
  if(m_initialized)
  {
    int error = Pa_Terminate( );
    if( error != paNoError )
    {
      Engine::getLog()->log("AudioSubsystem", "couldn't be shut down");
      printf(  "PortAudio error: %s\n", Pa_GetErrorText( error ) );
      return false;
    }  

    Engine::getLog()->log(m_name,"shut down");
    m_initialized = false;
    return true;
  } 

  return false;
}