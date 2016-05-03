#ifndef ENGINE_GUI
#define ENGINE_GUI

#include "GUI.h"

// class for the internal engine GUI

class EngineGUI : GUI
{
public:
  EngineGUI();
  ~EngineGUI();

  EngineGUI(const EngineGUI& other) = delete;
  EngineGUI& operator=(const EngineGUI& other) = delete;

  // intializes the GUI
  bool initialize();

  // the things we use
  using GUI::render;  
  using GUI::registerButtonPressedCallback;
  using GUI::checkButtonPressed;

private:

  static Image * m_back_panel;
  static Button * m_tb_camera;
  static Text * m_txt_camera;

  static Button * m_tb_fxaa;
  static Text * m_txt_fxaa;

  static Button * m_tb_shadow_pcf;
  static Text * m_txt_shadow_pcf;

  static Button * m_tb_shadow_rs;
  static Text * m_txt_shadow_rs;

  static Button * m_tb_shadow_standard;
  static Text * m_txt_shadow_standard;


  // callback four mouse button press
  static void mouseButtonCallback(Button * btn);
  
};



#endif // ENGINE_GUI