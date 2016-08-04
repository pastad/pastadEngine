#include "Skybox.h"

#include <vector>
#include <sstream>

#include <SOIL.h>
#include <glm/gtc/type_ptr.hpp>

#include "Engine.h"
#include "Log.h"


GLfloat Skybox::m_verts[108] =
{
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

Skybox::Skybox()
{
}

Skybox::~Skybox()
{
  glDeleteVertexArrays(1, &m_vao);
}


bool Skybox::load(const std::string basef)
{
  m_base_file = basef;

  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_verts), &m_verts, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

  glBindVertexArray(0);

  glGenTextures(1,&m_texture);

  int width,height;
  unsigned char* image;

  std::vector<std::string> texs;
  std::stringstream ss;
  ss <<basef << "_rt.jpg";
  texs.push_back(ss.str());
  ss.str(""); ss <<basef << "_lf.jpg";
  texs.push_back(ss.str());
  ss.str(""); ss <<basef << "_up.jpg";
  texs.push_back(ss.str());
  ss.str(""); ss <<basef << "_dn.jpg";
  texs.push_back(ss.str());
  ss.str(""); ss <<basef << "_bk.jpg";
  texs.push_back(ss.str());
  ss.str(""); ss <<basef << "_ft.jpg";
  texs.push_back(ss.str());
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
  for(GLuint i = 0; i < texs.size(); i++)
  { 
    image = SOIL_load_image(texs[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    if(image == nullptr )
    {
      Engine::getLog()->log("Skybox", "skybox couldn't be loaded :", texs[i]);
      
      Engine::getLog()->log("Skybox","skybox couldn't be loaded :", SOIL_last_result() );
      return false;
    }

    Engine::getLog()->log("Skybox", "skybox loaded :", texs[i]);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                  width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);


    SOIL_free_image_data(image);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  Engine::getLog()->log("Skybox", "loaded");

  return true;
}

void Skybox::render()
{
  glDepthFunc(GL_LEQUAL);

  glBindVertexArray(m_vao);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);

  glDepthFunc(GL_LESS);

}

void Skybox::save(tinyxml2::XMLNode * parent, tinyxml2::XMLDocument* document)
{
  tinyxml2::XMLElement * element_skybox = document->NewElement("Skybox");
  parent->InsertEndChild(element_skybox);
  element_skybox->SetAttribute("Path", m_base_file.c_str());
}
bool Skybox::load( tinyxml2::XMLElement *  element)
{
  if( element != nullptr)
  {
    m_base_file =std::string( element->Attribute("Path") );
    return load(m_base_file);
  }
  return false;
}