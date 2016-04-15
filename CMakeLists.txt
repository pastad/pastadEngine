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
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/Shader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/RenderShader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Scene.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Camera.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Transform.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Object.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Model.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Mesh.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Texture.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/Material.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Structures/IndexedRepresentation.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/RessourceManager.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/external/gl_core_4_.c
   
)
set(HEADERS
   ${HEADERS}
  
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Log.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/Subsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/RenderSubsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems/IOSubsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/Shader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders/RenderShader.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Scene.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Camera.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Transform.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Object.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Model.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Mesh.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Texture.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/Material.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Structures/IndexedRepresentation.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/RessourceManager.h
   ${CMAKE_CURRENT_SOURCE_DIR}/external/gl_core_4_.h

)

find_package(OpenGL REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  {GLFW_INCLUDE_DIRS} {ASSIMP_INCLUDE_DIRS} {SOIL_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystems
${CMAKE_CURRENT_SOURCE_DIR}/include/Shaders
${CMAKE_CURRENT_SOURCE_DIR}/include/Structures
${CMAKE_CURRENT_SOURCE_DIR}/external )

LINK_DIRECTORIES(${GLFW_LIBRARY_DIR} ${ASSIMP_LIBRARY_DIR} ${SOIL_LIBRARY_DIR})

add_executable(Engine main.cpp ${SOURCE} ${HEADERS})

target_link_libraries(Engine  GL glfw3 assimp SOIL  ${GLFW_STATIC_LIBRARIES}  ${OPENGL_LIBRARIES} X11 Xcursor Xinerama Xxf86vm Xi Xrandr pthread dl  )


