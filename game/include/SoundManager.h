#ifndef SOUND_MANAGER
#define SOUND_MANAGER


#include <vector>
#include <map>
#include <string>

#include <SFML/Audio.hpp>


class GUI;
class Button;

//  manages the sfml sounds

class SoundManager
{
public:
  SoundManager( );
  ~SoundManager();

  SoundManager(const SoundManager& other) = delete;
  SoundManager& operator=(const SoundManager& other) = delete;

  // adds a sound and starts it immediately
  static void addAndPlaySound(std::string path);

  // updates the manager | deletes sound etc
  static void update();

private:

  // the buffers for the sounds
  static std::map<std::string, sf::SoundBuffer *> m_buffers;
    
  // the sounds to be managed
  static std::vector<sf::Sound *> m_sounds;
};

#endif // SOUND_MANAGER