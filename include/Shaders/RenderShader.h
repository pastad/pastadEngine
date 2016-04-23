#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#define MAX_NUM_MATERIALS 20

// the shader that renders

#include <string>
#include <vector>

#include "Shader.h"
#include "Material.h"

class Light;

class RenderShader :  public Shader
{
public:

  RenderShader();
  ~RenderShader(); 

  // loads the shader
  bool load(const std::string path);

  // setters for matrices
  void setViewMatrix( glm::mat4 mvmatrix );
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );

  // sets the render pass
  void setRenderPass(unsigned int pass);

  /// determines if material is only a color
  void setColorOnly(bool color_only);

  // sets the material that should be used
  void setMaterial(std::string name, MaterialColorSpecs specs);

  // sets the material to the shader for the render pass
  void setAllMaterialsForRenderPass();
 
  // sets the lights that should be rendered
  void setLights(std::vector<Light*> * lights);

  // sets the camera for rendering effects
  void setCameraPosition(glm::vec3 pos);

  // sets the view and projection matrices to the identity matrices
  void setIdentityMatrices();

  // resets the materials
  void reset();

  // binds the shader  
  void use();

  // TEST 
  void setTestShadow(glm::mat4 mat);

  // set directional shadow matrix
  int setDirectionalShadowMap(glm::mat4 shadow_mat);

  // resets the shadow mapping
  void resetShadowMapping();

  // sets the pcf for shadow maps
  void setPCF(bool state);

  // enable shadows
  void setStandardShadows(bool state);

  
private:

  // function for setting the lights
  void setDirectionalLight(Light* light, unsigned int pos);
  void setSpotLight(Light* light, unsigned int pos);
  void setPointLight(Light* light, unsigned int pos);

  //holds the materials set for rendering 
  std::map<int, MaterialColorSpecs> m_materials;
  std::map<std::string, int> m_materials_mapping;
  int m_material_number = 0;

  // sets the material for the current material
  void setMaterialIndex(int);

  int m_shadow_map_count;
};

#endif