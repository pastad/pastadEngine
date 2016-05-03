#ifndef RENDER_SHADER_H
#define RENDER_SHADER_H

#define MAX_NUM_MATERIALS 20

// the shader that renders

#include <string>
#include <vector>

#include "Shader.h"
#include "Material.h"
#include "Engine.h"

class Light;

class RenderShader :  public Shader
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
  void setViewMatrix( glm::mat4 mvmatrix );
  void setProjectionMatrix( glm::mat4 projmatrix );
  void setNormalMatrix( glm::mat4 normalmatrix );

  // sets the camera for rendering effects
  void setCameraPosition(glm::vec3 pos);

  // sets the view and projection matrices to the identity matrices
  void setIdentityMatrices();

  // sets the render pass
  void setRenderPass(unsigned int pass);


  // material and lights -----------------------------------------

  /// determines if material is only a color
  void setColorOnly(bool color_only);

  // sets the material that should be used
  void setMaterial(std::string name, MaterialColorSpecs specs);

  // sets the material to the shader for the render pass
  void setAllMaterialsForRenderPass();

    // resets the materials
  void reset();  
 
  // sets the lights that should be rendered
  void setLights(std::vector<Light*> * lights);


  // shadow specific ---------------------------------------------

  // set directional shadow matrix
  int setShadowMap(glm::mat4 shadow_mat);

  // returns the number of point shadows set
  int setPointShadow();

  // resets the shadow mapping
  void resetShadowMapping();


  // technique setters -------------------------------------------

    // enable shadows
  void setShadows(ShadowTechniqueType tech);

 
  
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