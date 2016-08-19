#ifndef GUI_H
#define GUI_H

#include <map>
#include <vector>

#include <glm/glm.hpp>

// represents a Graphical User Interface

class TextShader;
class Text;
class ImageShader;
class Image;
class Button;
class EditText;


class Quad;

class GUI
{
public:
  GUI(unsigned int id);
  ~GUI();

  GUI(const GUI& other) = delete;
  GUI& operator=(const GUI& other) = delete;

  // renders the gui 
  void render(TextShader * text_shader, ImageShader* image_shader, Quad * quad);
  
  // setters for activation
  void setActive();
  void setInactive();

  // returns a text bound to this ui
  Text * addText();
  // removes the text
  void removeText(Text * text);

  // returns a edit text bound to this ui
  EditText * addEditText();
  // removes the edittext
  void removeEditText(EditText * text);

  // returns a image bound to this ui
  Image * addImage();  
  // removes the image
  void removeImage(Image * image);

  // returns a button bound to this ui
  Button * addButton();  
  // removes the button
  void removeButton(Button * button);

  // returns the id
  unsigned int getId();

  // returns true if active
  bool isActive();

  // checks if a button in the GUI was pressed
  bool checkButtonPressed(float x, float y);

  // register the callback function if a button is pressed
  void registerButtonPressedCallback( void  (*callback)(Button * btn)   );

   // register the callback function if an edit text is changed
  void registerEditTextCallback( void  (*callback)(EditText * btn)   );

  // adds a child to the gui
  void addChild(GUI * gui);

  // returns true if pos is inside gui | size must be set
  bool isInside(glm::vec2 pos);

  // sets the size of the gui
  void setSizeAndOffset(glm::vec2 size, glm::vec2 offset);

  // called when key is pressed
  void keyWasPressed(unsigned int key);

private:

  // true if active
  bool m_active;

  // represents a unique id for identifying
  unsigned int m_id;

  // holds the texts 
  std::map<int,Text *>  m_texts;
  unsigned int m_text_ids;

  // holds the images 
  std::map<int,Image *>  m_images;
  unsigned int m_image_ids;

  // holds the buttons 
  std::map<int,Button *>  m_buttons;
  unsigned int m_button_ids;

  // holds the edit texts 
  std::map<int,EditText *>  m_edit_texts;
  unsigned int m_edit_text_ids;

  // holds the child guis;
  std::vector<GUI * > m_children;

  // holds the size of the gui
  glm::vec2 m_size;

  // holds the offset of the gui
  glm::vec2 m_offset;

  // stores the callback for the button press
  void (*external_buttonPressedCallback)(Button * btn);

  // stores the callback for the edittext finish
  void (*external_editTextCallback)(EditText * edtxt);
  
};

#endif //GUI_H