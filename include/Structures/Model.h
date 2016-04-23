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
class Material;
class RenderShader;

class Model
{
public:

	// constructor with path to model
	Model(std::string path);
	~Model();	
	Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;

	// returns a new Instance pointers of the model
	Object * getInstance();

	//loads the model
	bool load();

	// renders the model
	void render(RenderShader * render_shader);

	// renders the model without material
	void renderWithoutMaterial();

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

	// holds the materials that are applied to the model
	std::map<int,Material*> m_materials;

	// the global inverse transform 
	aiMatrix4x4 m_GlobalInverseTransform;

	// matrices to render
	std::vector<glm::mat4> m_render_matrices;


	// processes the scene
	void processScene();

	// processes the materials of the model 
	void processMaterials(std::string directory);

};

#endif