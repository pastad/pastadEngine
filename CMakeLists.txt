cmake_minimum_required (VERSION 2.6)
project (Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE
   ${SOURCE}
   
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Subsystems/Subsystem.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Subsystems/RenderSubsystem.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Subsystems/IOSubsystem.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Subsystems/PhysicSubsystem.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/Shader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/RenderShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/RenderBaseShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/TextShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/ImageShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/SkyboxShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/PostProcessingShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Extension/Terrain.cpp     
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Extension/Skybox.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Camera.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Transform.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Light.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Object.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Model.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Mesh.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/AnimationMesh.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Texture.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/Material.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Model/IndexedRepresentation.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/SceneTreeElement.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/SceneEditor.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene/Scene.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Geometric/BoundingBox.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Geometric/Plane.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Geometric/Ray.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/GBuffer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/DirectionalShadowBuffer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/PointShadowBuffer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/RenderBuffer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/Quad.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Render/JitterTexture.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/GUI.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/EngineGUI.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/Text.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/EditText.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/Image.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/Button.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/FileViewer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Effects/Water.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Effects/Wave.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Script/Script.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Script/ScriptElement.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Script/RotationScriptElement.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Script/ExternalTriggerScriptElement.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/RessourceManager.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Helper.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/LuaScript.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/external/gl_core_4_.c


   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Game.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Player.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Mob.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Mobs.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Inventory.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Item.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Plant.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/Environment.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/EnergyShot.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/game/src/GameMenu.cpp
   
)
set(HEADERS
   ${HEADERS}
  
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Log.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/Subsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/RenderSubsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/IOSubsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/PhysicSubsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/Shader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/RenderShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/RenderBaseShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/TextShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/ImageShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/SkyboxShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/PostProcessingShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Extension/Terrain.h   
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Extension/Skybox.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Camera.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Transform.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Light.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Object.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Model.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Mesh.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/AnimationMesh.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Texture.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/Material.h 
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Model/IndexedRepresentation.h     
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/SceneTreeElement.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/SceneEditor.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Scene/Scene.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometric/BoundingBox.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometric/Plane.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Geometric/Ray.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/GBuffer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/DirectionalShadowBuffer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/PointShadowBuffer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/RenderBuffer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/Quad.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Render/JitterTexture.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/GUI.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/EngineGUI.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/Text.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/EditText.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/Image.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/Button.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/GUI/FileViewer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Effects/Water.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Effects/Wave.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Script/Script.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Script/ScriptElement.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Script/RotationScriptElement.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Script/ExternalTriggerScriptElement.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/RessourceManager.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Helper.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/LuaScript.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/gl_core_4_.h

   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Game.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Player.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Mobs.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Mob.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Inventory.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Item.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Plant.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/Environment.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/EnergyShot.h
   ${CMAKE_CURRENT_SOURCE_DIR}/game/include/GameMenu.h


)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
find_package(Boost REQUIRED)
find_package(Lua51 REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} {GLFW_INCLUDE_DIRS} {ASSIMP_INCLUDE_DIRS} {SOIL_INCLUDE_DIRS} {BOOST_INCLUDE_DIRS}  ${LUA_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems
${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders
${CMAKE_CURRENT_SOURCE_DIR}/include/Structures
${CMAKE_CURRENT_SOURCE_DIR}/include/Geometric
${CMAKE_CURRENT_SOURCE_DIR}/include/Render
${CMAKE_CURRENT_SOURCE_DIR}/include/Model
${CMAKE_CURRENT_SOURCE_DIR}/include/Scene
${CMAKE_CURRENT_SOURCE_DIR}/include/Extension
${CMAKE_CURRENT_SOURCE_DIR}/include/GUI
${CMAKE_CURRENT_SOURCE_DIR}/include/Effects
${CMAKE_CURRENT_SOURCE_DIR}/include/Script
${CMAKE_CURRENT_SOURCE_DIR}/external
${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2


${CMAKE_CURRENT_SOURCE_DIR}/game/include
 )

LINK_DIRECTORIES(${GLFW_LIBRARY_DIR} ${FREETYPE_LIBRARY_DIRS} ${ASSIMP_LIBRARY_DIR} ${SOIL_LIBRARY_DIR} ${Boost_LIBRARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2 ${LUA_LIBRARY_DIR}) 
 
add_executable(Engine main.cpp ${SOURCE} ${HEADERS})

target_link_libraries(Engine tinyxml2  GL glfw3 assimp boost_system boost_filesystem SOIL freetype lua5.1  ${LUA_LIBRARIES} ${GLFW_STATIC_LIBRARIES}  ${GLFW_STATIC_LIBRARIES}   ${OPENGL_LIBRARIES} X11 Xcursor Xinerama Xxf86vm Xi Xrandr pthread dl  )


