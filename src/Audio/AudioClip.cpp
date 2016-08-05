#include "AudioClip.h"

#include "Engine.h"
#include "Log.h"



#define SAMPLE_RATE (44100)

AudioClip::AudioClip():m_loaded(false)
{  
}

AudioClip::~AudioClip()
{  
}

bool AudioClip::load(std::string path)
{
  PaError err;

  err = Pa_OpenDefaultStream( & m_stream,
                              0,          /* no input channels */
                              2,          /* stereo output */
                              paFloat32,  /* 32 bit floating point output */
                              SAMPLE_RATE,
                              256,        /* frames per buffer, i.e. the number
                                                 of sample frames that PortAudio will
                                                 request from the callback. Many apps
                                                 may want to use
                                                 paFramesPerBufferUnspecified, which
                                                 tells PortAudio to pick the best,
                                                 possibly changing, buffer size.*/
                              nullptr, /* this is your callback function */
                              nullptr ); /*This is a pointer that will be passed to
                                                 your callback*/
  if( err != paNoError )
  {
    m_loaded = false;
    Engine::getLog()->log("AudioClip", "loading failed");
    return false;
  }
  else
  {
    m_loaded = true;
    return true;
  }
}

void AudioClip::start()
{
  if(m_loaded)
  {
    PaError err = Pa_StartStream( m_stream );
    if( err != paNoError )
    {
      Engine::getLog()->log("AudioClip", "starting failed");
    }
  }
}
void AudioClip::stop()
{
  if(m_loaded)
  {
    PaError err = Pa_StopStream( m_stream );
    if( err != paNoError )
    {      
      Engine::getLog()->log("AudioClip", "stoping failed");
    }
  }
}