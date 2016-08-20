#include "SoundManager.h"

#include "Game.h"

#include <iostream>

std::map<std::string, sf::SoundBuffer *> SoundManager::m_buffers;
std::vector<sf::Sound *> SoundManager::m_sounds;


SoundManager::SoundManager()
{
}
SoundManager::~SoundManager()
{
}

void SoundManager::addAndPlaySound(std::string path)
{
  std::map<std::string, sf::SoundBuffer *>::iterator it = m_buffers.find(path);
  if(it != m_buffers.end())
  {
    sf::Sound* sound = new sf::Sound();
    sound->setBuffer(*it->second);
    sound->play();
    sound->setLoop(false);
    sound->setVolume(SOUND_EFFECT_VOLUME);
    m_sounds.push_back(sound);
  }
  else
  {
    sf::SoundBuffer * buf = new sf::SoundBuffer();
    if (buf == nullptr)
        std::cout << "t" << std::endl;
    if (!buf->loadFromFile(path))
    {
      std::cout << "sound couldn't be loaded" <<std::endl;
    }
    else
    {
      std::pair<std::string, sf::SoundBuffer *> pai(path,buf);
      m_buffers.insert(pai);
      sf::Sound* sound = new sf::Sound();
      sound->setBuffer(*buf);
      sound->play();
      sound->setLoop(false);
      sound->setVolume(SOUND_EFFECT_VOLUME);
      m_sounds.push_back(sound);
    }
  
  }
}

void SoundManager::update()
{
  for(std::vector<sf::Sound *>::iterator it = m_sounds.begin(); it != m_sounds.end();)
  {
    float pos = (*it)->getPlayingOffset().asSeconds();
    float max_pos = (*it)->getBuffer()->getDuration().asSeconds();

    if(pos >= max_pos)
    {
      delete (*it);
      m_sounds.erase(it);
    }
    else
      it++;
  }
}