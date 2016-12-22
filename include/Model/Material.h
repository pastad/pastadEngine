#ifndef MATERIAL_H
#define MATERIAL_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <map>
#include <string>

#include <glm/glm.hpp>

class Texture;
class RenderShader;

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

  // if the material is emmissive
  float m_emmissive;
};

class Material
{
public:
  Material();
  Material(aiMaterial * material, std::string dir);
  ~Material();

  Material(const Material& other) = delete;
  Material& operator=(const Material& other) = delete;

  // binds the material starting at unit to the render shader      
  void bind(int unit, RenderShader * render_shader );

  // returns true if the material is transparent
  bool isTransparent();

  // emmissive getter setters
  void setEmmissive(float val);

protected:

private:

  // the name of the material
  std::string m_name;

  // pointers to the diffuse texures and their strengths
  std::map<int,Texture*> m_diffuse_textures;
  std::map<int, int > m_diffuse_texture_strengths;

  // the normal texture of the material
  Texture* m_normal_texture = nullptr;

  // the key texture of the material
  Texture* m_opacity_texture = nullptr;

  // the material color specs for lightinf
  MaterialColorSpecs m_material_specs;

  // determines if base texture is set
  bool m_base_texture_set;

  // true if material emmits
  bool m_emmissive;
};

#endif // MATERIAL_H