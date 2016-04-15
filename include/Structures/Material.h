#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include<map>
#include<string>

#include <glm/glm.hpp>

class Texture;

struct MaterialColorSpecs
{
  // the colors of the material used for lighting
  glm::vec3 m_ambient_color;
  glm::vec3 m_diffuse_color;
  glm::vec3 m_specular_color;

  // controls how shini the model is
  float m_shininess;

  // the transparency value
  float m_opacity;

};

class Material
{
public:
  Material();
  Material(aiMaterial * material, std::string dir);
  ~Material();

  Material(const Material& other) = delete;
  Material& operator=(const Material& other) = delete;

  // binds the material        
  void bind(int unit);

  // returns true if the material is transparent
  bool isTransparent();


protected:

private:

  // holds pointersto the texures
  std::map<int,Texture*> m_textures;

  MaterialColorSpecs m_material_specs;

  unsigned int m_diffuse_texture_count = 0;
};

#endif // MATERIAL_H