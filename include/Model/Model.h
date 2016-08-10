#ifndef MODEL
#define MODEL_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>

// the model holds the loaded data of the model
// not for the user to be manipulated

class Object;
class Mesh;
class AnimationMesh;
class Material;
class RenderShader;
class RenderBaseShader;
class PointShadowShader;
class BoundingBox;

class Model
{
public:

	// constructor with path to model
	Model(std::string path, bool instaced);
	~Model();	
	Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;

	// returns a new Instance pointers of the model
	Object * getInstance();

	// removes the instance from the model
	void removeInstance( Object * obj);

	//loads the model
	bool load();

	// render functions for the scene
	void render(RenderBaseShader * render_shader, bool with_material);
	void render(RenderBaseShader * render_shader, std::vector<Object *> objects, bool with_material);

	// returns the assimp scene that the model belontgs
	const aiScene * getAssimpScene();

	// returns true if animated
	bool isAnimated();

	//returns the objects
	std::vector<Object *> getObjects();

	// returns the bb of the model
	BoundingBox * getBoundingBox();

  // returns the meshes
  std::vector<Mesh*> getMeshes();

  // returns all materials
  std::map<int,Material *> getMaterials();

  // rebuffer if we have changed our objects
  void refreshBufferedMatrices(std::vector<Object *> instances);


private:

	// holds pointers to the instances
	std::vector<Object *> m_instances;

	// contains the path to the model
	std::string m_path;

	// true if animated model
	bool m_animated;

	// holds the "scene"  (model data from assimp)
	const aiScene * m_scene;

	// holds the importer used to import the model
	Assimp::Importer m_importer;

	// holds the meshes of the model
	std::vector<Mesh*> m_meshes;

	// holds the animation meshes of the model
	std::vector<AnimationMesh*> m_animation_meshes;

	// holds the materials that are applied to the model
	std::map<int,Material*> m_materials;

	// the global inverse transform 
	aiMatrix4x4 m_GlobalInverseTransform;

	// matrices to render
	std::vector<glm::mat4> m_render_matrices;

	// determines if instancation should be enabled
	bool m_instanced;	

	// the bounding box of the model
	BoundingBox * m_bounding_box = nullptr;

	// the number of instanced matrices
	unsigned int m_num_instanced_buffered_matrices;


	// processes the scene
	void processScene();

	// processes the materials of the model 
	void processMaterials(std::string directory);

	// animation functions
	void readNodeHeirarchy(float animation_time, const aiNode* pNode, const glm::mat4& parentTransform);
	void calcInterpolatedScaling(aiVector3D& Out, float animation_time, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& Out, float animation_time, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedPosition(aiVector3D& Out, float animation_time, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float animation_time, const aiNodeAnim* pNodeAnim);
	unsigned int findRotation(float animation_time, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float animation_time, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

	glm::mat4 initScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
  glm::mat4 initRotateTransform(float RotateX, float RotateY, float RotateZ);
  glm::mat4 initTranslationTransform(float x, float y, float z);

};

#endif