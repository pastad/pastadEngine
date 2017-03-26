#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#define MAX_NUM_MATERIALS 100

// the shader that renders

#include <string>
#include <vector>

#include "RenderBaseShader.h"
#include "Material.h"
#include "Engine.h"

class Light;

class RenderShader :  public RenderBaseShader
{
public:

  RenderShader();
  ~RenderShader(); 

  // loads the shader
  bool load(const std::string path);

  // binds the shader  
  void use();


  // uniform setters -----------------------------------------------

  // setters for matrices
  using RenderBaseShader::setViewMatrix;
  using RenderBaseShader::setProjectionMatrix;

  // sets the camera for rendering effects
  void setCameraPosition(glm::vec3 pos);

  // sets the view and projection matrices to the identity matrices
  void setIdentityMatrices();

  // sets the render pass
  void setRenderPass(unsigned int pass);

  // setter for the fog
  void setFog(glm::vec3 color, float factor, float offset);

  // sets the size of the gbuffer
  void setGBufferSize(glm::vec2 size);


  // material and lights -----------------------------------------

  // determines if material is only a color
  void setColorOnly(bool color_only);

  // activates the specular map usage
  void setSpecularMapActive(int active);

  // activates the normal map usage
  void setNormalMapActive(int active);

  // activates the bump map usage
  void setBumpMapActive(int active);

  // activates the opacity map usage
  void setOpacityMapActive(int active);

  // sets the material that should be used
  void setMaterial(std::string name, MaterialColorSpecs specs);

  // sets the material to the shader for the render pass
  void setAllMaterialsForRenderPass();

    // resets the materials
  void reset();  
 
  // sets the lights that should be rendered
  void setLights(std::vector<Light*> * lights);

  std::vector<int> getEmmissiveMaterialIndices();




  // shadow specific ---------------------------------------------

  // set directional shadow matrix
  int setShadowMap(glm::mat4 shadow_mat);

  // returns the number of point shadows set
  int setPointShadow();

  // resets the shadow mapping
  void resetShadowMapping();


  // technique setters -------------------------------------------

    // enable shadows
  void setShadowsDirectional(ShadowTechniqueType tech);
  void setShadowsPoint(ShadowTechniqueType tech);
  void setSSAO(bool val);

  using RenderBaseShader::setAnimation;
  using RenderBaseShader::unsetAnimation;
  using RenderBaseShader::setBones;
  using RenderBaseShader::setInstanced;
  using RenderBaseShader::setNotInstanced; 
  
private:
    // holds the materials set for rendering 
  std::map<int, MaterialColorSpecs> m_materials;
  std::map<std::string, int> m_materials_mapping;
  int m_material_number = 0;

  // number of shadow maps
  int m_shadow_map_count;
  int m_cube_shadow_map_count;

  // function for setting the lights
  void setDirectionalLight(Light* light, unsigned int pos);
  void setSpotLight(Light* light, unsigned int pos);
  void setPointLight(Light* light, unsigned int pos);

  // sets the material for the current material
  void setMaterialIndex(int);


};

#endif