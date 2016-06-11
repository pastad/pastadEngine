#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#ifdef __cplusplus
# include <lua5.2/lua.hpp>
#else
# include <lua5.2/lua.h>
# include <lua5.2/lualib.h>
# include <lua5.2/lauxlib.h>
#endif

#include <string>

#include <glm/glm.hpp>

// class for calling lua programs

class LuaScript
{

public:
  LuaScript();
  ~LuaScript();

  LuaScript(const LuaScript& other) = delete;
  LuaScript& operator=(const LuaScript& other) = delete;

  // loads a lua file | returns true for success
  bool loadFile(std::string file_name);

  // executes the file
  bool executeMain();

  // calls the lua function "name"
  bool callFunction(std::string name);

protected:  

  // the state of the lua environment
  lua_State * m_state = nullptr;

};

#endif // LUASCRIPT_H