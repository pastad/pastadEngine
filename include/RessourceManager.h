#ifndef RESSOURCEMANAGER
#define RESSOURCEMANAGER_H

#include <string>
#include <map>

class Model;

// loads the ressources and stores them to prevent loading twice

class RessourceManager
{
public:
	RessourceManager();
	~RessourceManager();

	// returns the requested model (and loads it if not loaded)
	static Model * loadModel(std::string path);

private:

	// holds the loaded models
	static std::map<std::string, Model *> m_models;
};

#endif