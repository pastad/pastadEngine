#include "RenderSubsystem.h"
#include "Log.h"
#include "Engine.h"

RenderSubsystem::RenderSubsystem()
{	
}

RenderSubsystem::~RenderSubsystem()
{	
}

bool RenderSubsystem::startUp(GLFWwindow * window)
{
	m_window = window;
	Engine::getLog()->log("RenderSubsystem", "started");
	return true;
}
bool RenderSubsystem::shutDown()
{
	Engine::getLog()->log("RenderSubsystem", "shut down");
	return true;
}