#ifndef RESSOURCEMANAGER
#define RESSOURCEMANAGER_H

#include <string>
#include <map>

class Model;
class Object;

// loads the ressources and stores them to prevent loading multiple times

class RessourceManager
{
public:
	RessourceManager();
	~RessourceManager();

	// returns a new Object for the user
	Object * getObject(std::string path);

private:

	// holds the loaded models
	static std::map<std::string, Model *> m_models;

	// returns the requested model (and loads it if not loaded)
	static Model * loadModel(std::string path);
};

#endif