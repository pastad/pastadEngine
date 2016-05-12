#include "Player.h"

#include "Camera.h"

Player::Player()
{  
}

Player::~Player()
{  
}

bool Player::init(Camera * camera)
{
  m_camera = camera;

  return true;
}

void Player::update()
{

}