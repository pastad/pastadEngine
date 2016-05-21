# pastadEngine

Requirements:
  - GL
  - glfw3
  - assimp
  - SOIL
  - freetype

gl-loader-file generated with https://bitbucket.org/alfonse/glloadgen/wiki/Home 

Some code parts inspired by openGl 4 shading cookbook due to the fact that 
the subjects where studied with it

Basic workflow
  First initialize the engine with the width and height of the screen, the subsystems to be initialized and a indication if fullscreen is intendet. The render and the IO subsystem are
  initialized every time because these subsystems are required for minimal functionallity.

  Then create a Scene object where the objects, lights, etc. can be created from. 
  Also the scene contains a camera and a skybox can be set for it.
  The scene needs to be added to the engine so that it can be rendered and update functions
  can be applied.

  After that the basic main loop could look like this:

  while(engine.running())
  {    
    engine.update();

    // DO SPECIFIC UPDATE

    engine.render();
  }

  In the end the engine should be shut down to clean everything up.
  

Lighting: 
  uses deferred rendering


TODO:

Structural:
  - screen space culling
  - write doc

Graphical:
  - water
  - ambient oclusion
  - bloom effect
  - HDR
  - 3d picking
  - terrain
  - OIT
  - material refactor and improvement

GUI:
  - GUI class
  - Panels

Customizability:
  - setting specifactions (MULTI_SAMPLES,...)







