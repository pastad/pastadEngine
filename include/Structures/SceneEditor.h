#ifndef SCENE_EDITOR
#define SCENE_EDITOR

#include "GUI.h"

#include <glm/glm.hpp>

// class for editing the scene

class Light;
class Object;

class SceneEditor : GUI
{
public:
  SceneEditor();
  ~SceneEditor();

  SceneEditor(const SceneEditor& other) = delete;
  SceneEditor& operator=(const SceneEditor& other) = delete;

  // intializes the GUI
  bool initialize();

  // updates the editor
  void update();

  // the things we use
  using GUI::render;  
  using GUI::registerButtonPressedCallback;
  using GUI::checkButtonPressed;
  using GUI::setInactive;

private:
  static Image * m_back_panel_edit;

  // btns and texts for adding deleting lights
  static Button * m_btn_del_light;
  static Text * m_txt_del_light;

  static Button * m_btn_add_light;
  static Text * m_txt_add_light;

  static Button * m_btn_add_spot_light;
  static Text * m_txt_add_spot_light;

  static Button * m_btn_add_object;
  static Text * m_txt_add_object;

  // movement and rotation buttons
  static Button * m_btn_x_plus;
  static Button * m_btn_x_minus;
  static Button * m_btn_y_plus;
  static Button * m_btn_y_minus;
  static Button * m_btn_z_plus;
  static Button * m_btn_z_minus;

  static Button * m_btn_rot1_plus;
  static Button * m_btn_rot1_minus;
  static Button * m_btn_rot2_plus;
  static Button * m_btn_rot2_minus;

  static Text * m_txt_xz_coords;
  static Text * m_txt_y_coords;



  // selected light for edit mode
  static Light * m_edit_selected_light;

  // selected object for edit mode
  static Object * m_edit_selected_object;

  // callback four mouse button press
  static void mouseButtonCallback(Button * btn);

  // moves the active light or object
  static void moveActive(glm::vec3 dir);
  
};



#endif // SCENE_EDITOR