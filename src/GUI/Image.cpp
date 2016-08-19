#include "Image.h"

#include "ImageShader.h"
#include "RessourceManager.h"
#include "Texture.h"
#include "Quad.h"
#include "Engine.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


Image::Image(unsigned int id):m_id(id), m_active(true)
{  
  setPosition(glm::vec2(0,0));
  setScale(glm::vec2(1.0f,1.0f));
  m_texture = nullptr;
}

Image::~Image()
{  
}


// load/render -------------------------------------------------

bool Image::load(std::string path)
{
  m_texture = RessourceManager::loadTexture(path);  
  return true;
}

void Image::render(ImageShader * image_shader, Quad * quad)
{
  if( (m_texture != nullptr) && m_active )
  {
    image_shader->use();
    image_shader->setScreenSize(glm::vec2(Engine::getWindowWidth(),Engine::getWindowHeight()));
    image_shader->setPosition(m_position );
    image_shader->setModelSize(  m_texture->getSize() * m_scale );
    m_texture->bind(0);
    quad->render();
  }
}


// getter/setters -------------------------------------------------

// pos/scale/size/id

void Image::setPosition(glm::vec2 position)
{
  m_position = position;
}
  
void Image::setScale(glm::vec2 scale)
{
  m_scale = scale;
}

glm::vec2 Image::getSize()
{
  return  m_texture->getSize();
}

unsigned int Image::getId()
{
  return m_id;
}

// active

void Image::setActive()
{
  m_active = true;
}

void Image::setInactive()
{
  m_active = false;
}

bool Image::isActive()
{
  return m_active;
}
