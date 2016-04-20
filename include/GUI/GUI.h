#ifndef GUI_H
#define GUI_H

#include <map>

// represents a Graphical User Interface

class TextShader;
class Text;
class ImageShader;
class Image;
class Button;


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
  Text * getText();
  // removes the text
  void removeText(Text * text);

  // returns a image bound to this ui
  Image * getImage();  
  // removes the image
  void removeImage(Image * image);

  // returns a button bound to this ui
  Button * getButton();  
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

  // stores the callback for the button press
  void (*external_buttonPressedCallback)(Button * btn);
  
};

#endif //GUI_H