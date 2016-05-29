#ifndef SCENE_EDITOR
#define SCENE_EDITOR

#include "GUI.h"

#include <glm/glm.hpp>

// class for editing the scene

class Light;
class Object;
class FileChooser;

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
  using GUI::keyWasPressed;

  // starts adding an object
  void startObjectAdding();

private:
  static Image * m_back_panel_edit;

  // btns and texts for adding deleting lights
  static Button * m_btn_del_light;
  static Text * m_txt_del_light;

  static Button * m_btn_add_light;
  static Text * m_txt_add_light;

  static Button * m_btn_add_spot_light;
  static Text * m_txt_add_spot_light;

  static Button * m_btn_add_directional_light;
  static Text * m_txt_add_directional_light;

  static Button * m_btn_add_object;
  static Text * m_txt_add_object;

  static EditText * m_edittxt_x_coord;
  static EditText * m_edittxt_y_coord;
  static EditText * m_edittxt_z_coord;

  static EditText * m_edittxt_x_rot;
  static EditText * m_edittxt_y_rot;
  static EditText * m_edittxt_z_rot;

  static EditText * m_edittxt_light_color;


  // movement and rotation buttons
  static Button * m_btn_x_plus;
  static Button * m_btn_x_minus;
  static Button * m_btn_y_plus;
  static Button * m_btn_y_minus;
  static Button * m_btn_z_plus;
  static Button * m_btn_z_minus;

  static Button * m_btn_rot_x_plus;
  static Button * m_btn_rot_x_minus;
  static Button * m_btn_rot_y_plus;
  static Button * m_btn_rot_y_minus;
  static Button * m_btn_rot_z_plus;
  static Button * m_btn_rot_z_minus;

  static Text * m_txt_coords;
  static Text * m_txt_rotation;

  // true if extended
  static bool m_expanded;

  // the file chooser for the editor
  static FileChooser * m_file_chooser;

  // selected light for edit mode
  static Light * m_edit_selected_light;

  // selected object for edit mode
  static Object * m_edit_selected_object;

  // callback four mouse button press
  static void mouseButtonCallback(Button * btn);

  // callback for edit text finish
  static void editTextChangedCallback(EditText * edit_txt);

  // moves the active light or object
  static void moveActive(glm::vec3 dir);

  // rotates the active light or object
  static void rotateActive(glm::vec3 delta);

  // refreshes the text
  static void refreshText(glm::vec3 p, glm::vec3 r);

  // set or unsets the object light specific buttons,etc...
  static void setObjectSpecifics(bool on);
  static void setLightSpecifics(bool on);

  
};



#endif // SCENE_EDITOR