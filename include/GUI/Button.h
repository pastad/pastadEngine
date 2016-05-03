#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include <glm/glm.hpp>

#include "Image.h"

// class to represent a button (image + text + functionallity)

class Text;
class TextShader;

class Button : public Image
{
public:
  Button(unsigned int id);
  ~Button();

  Button(const Button& other) = delete;
  Button& operator=(const Button& other) = delete;

  // returns the id of the image
  using Image::getId;
  using Image::setActive;
  using Image::setInactive;



  // initializers for normal and toggled
  bool intitialize(std::string path, std::string text, glm::vec2 p, glm::vec2 scale,
    glm::vec3 text_color, std::string descriptor);
  bool intitializeWithToggle(std::string path1,std::string path2,
  glm::vec2 p, glm::vec2 scale, std::string descriptor, bool state);

  // loads the image and stores the text
  bool load(std::string path, std::string text);

  // renders the image and the text using the shader
  void render(ImageShader * image_shader, TextShader * text_shader, Quad * quad);

  // sets the position and scale of the button
  void setPositionAndScale(glm::vec2 position, glm::vec2 scale);

  // text setters
  void setText(std::string text);
  void setTextColor(glm::vec3 color);

  // getter setter for descriptor
  void setDescriptor(std::string text);
  std::string getDescriptor();

  // true if the button was pressed
  bool wasPressed(float x, float y);

  // sets the image
  bool setImage(std::string path);

  //return true if toggled and true if enabled
  bool isToggled();
  bool isTogglEnabled();

  // sets the toggle state
  void togglOn();
  void togglOff();

private:

  // the text of the button
  Text * m_text;

  // descriptor  to identify button | for user
  std::string m_descriptor;

  // determines if it is able to toggle
  bool m_toggl_enable;

  // determines the toggle state
  bool m_toggl;

  // the paths in case of toggle
  std::string m_path_image_on;
  std::string m_path_image_off;

  
};

#endif // BUTTON_H