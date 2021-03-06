# pastadEngine - Windows 

Video : [https://www.youtube.com/watch?v=K5UyU4VL4TI](https://www.youtube.com/watch?v=K5UyU4VL4TI)

Screenshots:

![Alt text](/Release/screenshot.PNG?raw=true "Optional Title")
![Alt text](/Release/screenshot_4.PNG?raw=true "Optional Title")


This is a rendering engine project. It includes an editor to maipulate the scene (place object, lights, ... )
It is based on C++ and OpenGL. 

Requirements for compilation: 
  - C++
  - GL
  - glfw3
  - assimp
  - SOIL     (place in external)
  - freetype
  - tinyxml2 (place in external)  
 

gl-loader-file generated with https://bitbucket.org/alfonse/glloadgen/wiki/Home 

Some code parts inspired by openGl 4 shading cookbook due to the fact that 
the subjects where studied with it.

Basic workflow for using the engine:
  First initialize the engine with the width and height of the screen, the subsystems to be initialized and a indication if fullscreen is intendet. The render and the IO subsystem are
  initialized every time because these subsystems are required for minimal functionallity.

  engine.initialize("game/engine_config.xml"); or e.g. engine.initialize(1240, 720, RENDER_SUBSYSTEM , true, false);
   
  Then create a Scene object where the objects, lights, etc. can be created from. 
  Also the scene contains a camera and a skybox can be set for it.
  The scene needs to be added to the engine so that it can be rendered and update functions
  can be applied.

  Scene *  scene = new Scene();
	scene->load("island-scene.xml");
	engine.setScene(scene, false);

  After that the basic main loop could look like this:

  while(engine.running())
  {    
    // Do your update here
    engine.update();
    engine.render();
  }

  or this 

  engine.run(); 

  with a callback to a update(float time_delta) function registered before. 
  
  ... 
  void update(float d)
  {
    ...
  }
  ...
  engine.setUpdateFunction(&update);
  ...

  In the end the engine should be shut down to clean everything up.

  engine.shutDown();  




Done:
  - DeferredLighting
  - Shadows (Point/Spot/Directional)
    - PCF (for all light types)
    - RandomSampling ( for Directional/Spot)
  - FXAA
  - SSAO
  - Basic Scripting
  - Simple Physics(falling)
  - Picking
  - Objects
    - Skybox
    - AnimatedObjects (Bone-Animation)
  - SceneEditor
    - Add/Delete Lights
    - Add/Delete Objects    
  - Scene
    - Loading/Saveing
    - Object/Light- Management
    
Partially done ( improvements should be done) :

  - GUI 
    - EditText
    - Image
    - Button
  - SceneEditor
    - Add Skybox

  - HDR ( basic |should be improved later ) 

TODO:

Structural:
  - screen space culling ( material flickering when activated )
  - write doc
  
Graphical:
  - water
  - bloom effect ( basic|needs major improvement)
  - terrain
  - OIT

GUI:
  - usability improvement

Customizability:
  - setting specifactions (MULTI_SAMPLES,...)


"personal note for xcompile  ./../../lib/mxe/usr/bin/x86_64-w64-mingw32.static-cmake ."





