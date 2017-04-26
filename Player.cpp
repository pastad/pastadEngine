#include "Player.h"

#include "include\Subsystems\IOSubsystem.h"
#include "include\Model\Object.h"
#include "include\Structures\Camera.h"

Player::Player(Object * obj, Camera * cam)
{
  m_transform = new Transform();
  m_object = obj;
  m_camera = cam;
  m_camera->rotate(0, 45.0f);

}


Player::~Player()
{
  delete m_transform;
}


void Player::update(float delta)
{

  glm::vec2 mouse_delta = IOSubsystem::getMouseDelta();
  mouse_delta *= 0.5f;

  m_object->rotate(glm::vec3(0, mouse_delta.x, 0));

  m_camera->lookAt(m_object->getPosition(), glm::vec2(-mouse_delta.x , 0), 30.0f);

  glm::vec3 dir = m_camera->getDirection();
  dir.y = 0.0f;

  if (IOSubsystem::isKeyPressed('W'))
  {
    m_object->move( dir * delta*50.0f);
  }
}