cmake_minimum_required (VERSION 2.6)
project (Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Subsystem.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/RenderSubsystem.cpp
   
)
set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Engine.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Log.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/Subsystem.h
   ${CMAKE_CURRENT_SOURCE_DIR}/include/RenderSubsystem.h

)




find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)

LINK_DIRECTORIES(${GLFW_LIBRARY_DIR})

add_executable(Engine main.cpp ${SOURCE} ${HEADERS})

target_link_libraries(Engine  GL glfw3   ${GLEW_LIBRARY} ${GLFW_LIBRARIES}  ${OPENGL_LIBRARIES} Xcursor Xinerama Xxf86vm Xi Xrandr  pthread dl  )


