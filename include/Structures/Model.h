#ifndef MODEL
#define MODEL_H

// the model holds the loaded data of the model
// not for the user to be manipulated

#include <vector>

class Object;

class Model
{
public:

	// constructor with path to model
	Model(std::string path);
	~Model();	

	// returns a new Instance pointers of the model
	Object * getInstance();

	//loads the model
	bool load();

	// renders the model
	void render();

private:

	// holds pointers to the instances
	std::vector<Object *> m_instances;

	// contains the path to the model
	std::string m_path;

	// true if animated model
	bool m_animated;

	// processes the scene
	void processScene();
};

#endif