#ifndef GUI_H
#define GUI_H

#include <vector>

// represents a Graphical User Interface

class TextShader;
class Text;

class GUI
{
public:
  GUI(unsigned int id);
  ~GUI();

  GUI(const GUI& other) = delete;
  GUI& operator=(const GUI& other) = delete;

  // renders the gui 
  void render(TextShader * text_shader);
  
  // setters for activation
  void setActive();
  void setInactive();

private:

  // true if active
  bool m_active;

  // represents a unique id for identifying
  unsigned int m_id;

  // holds the texts 
  std::vector<Text *>  m_texts;
  
};

#endif //GUI_H