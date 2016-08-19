#include "Skybox.h"

#include <vector>
#include <sstream>

#include <SOIL.h>
#include <glm/gtc/type_ptr.hpp>

#include "Engine.h"
#include "Log.h"

#include "SkyboxShader.h"


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

Skybox::Skybox(): m_light_strength(1.0f)
{
}

Skybox::~Skybox()
{
  gl::DeleteVertexArrays(1, &m_vao);
}


bool Skybox::load(const std::string basef)
{
  m_base_file = basef;

  gl::DeleteVertexArrays(1, &m_vao);
  gl::GenVertexArrays(1, &m_vao);
  gl::GenBuffers(1, &m_vbo);

  gl::BindVertexArray(m_vao);
  gl::BindBuffer(gl::ARRAY_BUFFER, m_vbo);
  gl::BufferData(gl::ARRAY_BUFFER, sizeof(m_verts), &m_verts, gl::STATIC_DRAW);

  gl::EnableVertexAttribArray(0);

  gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 3 * sizeof(GLfloat), (GLvoid*)0);

  gl::BindVertexArray(0);

  gl::GenTextures(1,&m_texture);

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
  gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_texture);
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

    gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl::RGB,
                  width, height, 0, gl::RGB, gl::UNSIGNED_BYTE, image);


    SOIL_free_image_data(image);
  }
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_MIN_FILTER, gl::LINEAR);

  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE);
  gl::TexParameteri(gl::TEXTURE_CUBE_MAP, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE);

  gl::BindTexture(gl::TEXTURE_CUBE_MAP, 0);
  Engine::getLog()->log("Skybox", "loaded");

  return true;
}

void Skybox::render(SkyboxShader * shader)
{
  shader->use();
  shader->setLightStrength(m_light_strength);

  gl::DepthFunc(gl::LEQUAL);

  gl::BindVertexArray(m_vao);
  gl::ActiveTexture(gl::TEXTURE0);

  gl::BindTexture(gl::TEXTURE_CUBE_MAP, m_texture);
  gl::DrawArrays(gl::TRIANGLES, 0, 36);

  gl::BindVertexArray(0);

  gl::DepthFunc(gl::LESS);

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

void Skybox::setLightStrength(float strength)
{
  m_light_strength = strength;
}