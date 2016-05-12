#ifndef PLAYER_H
#define PLAYER_H

class Camera;

// the player character

class Player
{
public:
  Player();
  ~Player();

  Player(const Player& other) = delete;
  Player& operator=(const Player& other) = delete;


  // initializes the player
  bool init(Camera * camera);

  // updates the player
  void update();


private:

  // holds the camera pointer for the camera
  Camera * m_camera;

};

#endif