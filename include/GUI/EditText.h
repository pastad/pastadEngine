#ifndef EDITTEXT_H
#define EDITTEXT_H

#include "Text.h"

#include <string>

#include <glm/glm.hpp>

// class for displaying and editing text on screen

class TextShader;

class EditText :Text
{

public:
  EditText(unsigned int id);
  EditText(unsigned int id,std::string txt, glm::vec2 pos, float scale, glm::vec3 color);
  ~EditText();

  EditText(const EditText& other) = delete;
  EditText& operator=(const EditText& other) = delete;

  using Text::render;
  using Text::getId;
  using Text::set;
  using Text::setText;
  using Text::getText;

  using Text::setActive;
  using Text::setInactive;

  // returns true if was pressed
  bool wasPressed(float x, float y);

  // returns true if in input mode
  bool isInInputMode();

  // handles the input if active
  bool handleInput(unsigned int key); 

  // getter and setter for descriptor
  void setDescriptor(std::string descriptor);
  std::string getDescriptor();



protected:  

  // the size that can be clicked
  glm::vec2 m_size = glm::vec2(100,10);

  // true if input mode is set (typing ...)
  bool m_input_mode = false;

  // the descriptor to identify the edittext
  std::string m_descriptor = "";
};

#endif // EDITTEXT_H