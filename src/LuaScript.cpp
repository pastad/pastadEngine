#include "LuaScript.h"

#include "Engine.h"
#include "Log.h"


LuaScript::LuaScript()
{  
  // create the environemtn and open the standard libs
  m_state =  luaL_newstate();
  luaL_openlibs(m_state);
}

LuaScript::~LuaScript()
{ 
 lua_close(m_state); 
}

bool LuaScript::loadFile(std::string file_name)
{
  int res;

  res = luaL_loadfile(m_state, file_name.c_str());

  if ( res != LUA_OK ) 
  {
    Engine::getLog()->log("LuaScript", "couldn't load file");
    return false;
  }

  return true;
}
bool LuaScript::executeMain()
{
  int res = lua_pcall(m_state, 0, LUA_MULTRET, 0);

  if ( res != LUA_OK ) 
  {
    Engine::getLog()->log("LuaScript", "couldn't execute program");
    return false;
  }
  return true;
}
bool LuaScript::callFunction(std::string name)
{
  lua_getglobal(m_state, name.c_str());
  int res =lua_pcall(m_state,0,0,0);

  if ( res != LUA_OK ) 
  {
    Engine::getLog()->log("LuaScript", "couldn't execute program");
    return false;
  }
  return true;
}