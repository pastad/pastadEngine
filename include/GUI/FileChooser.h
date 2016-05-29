#ifndef FILE_CHOOSER
#define FILE_CHOOSER

#include "GUI.h"
#include <boost/filesystem.hpp>

// class for a file chooser gui

#define FC_STATUS_NOTACTIVE 0
#define FC_STATUS_ACTIVE 1
#define FC_STATUS_DONE_OK 2
#define FC_STATUS_DONE_CANCLED 3

class FileChooser : GUI
{
public:
  FileChooser();
  ~FileChooser();

  FileChooser(const FileChooser& FileChooser) = delete;
  FileChooser& operator=(const FileChooser& other) = delete;

  // intializes the GUI
  bool initialize();

  // starts  the chooser
  void start();

  // the things we use
  using GUI::render;  
  using GUI::registerButtonPressedCallback;
  using GUI::checkButtonPressed;
  using GUI::isInside;
  using GUI::isActive;

  // returns the status
  unsigned int getStatus();  

  // selects a subdirectory
  void chosenDirectory(std::string dir);

  // jumps up a directory
  void chosenParentDirectory();

  // sets the file in the current directory to chosen one
  void chosenFile(std::string filename);

  // resets the file chooser | ususally when status was read
  void reset();

  // returns the selected file name
  std::string getFileName();

private:

  Image * m_back_panel; 
  Button *m_btn_close;
  Button *m_btn_ok;
  Text * m_txt_selected;
  Text * m_txt_path;

  // callback four mouse button press
  static void mouseButtonCallback(Button * btn);

  using GUI::setActive;
  using GUI::setInactive;

  //  indicates the status of the filechooser
  unsigned int m_status;

  // holds the file selection buttons
  std::vector<Button *> m_files;

  // the current path chosen
  boost::filesystem::path m_current_path;

  // the current file chosen
  boost::filesystem::path m_current_file;
  
  // the files in the cuurent path
  std::vector<boost::filesystem::path> m_current_path_files;
  std::vector<boost::filesystem::path> m_current_path_directories;

  unsigned int m_view_offset = 0;

  // loads the files in m_current_path
  void loadPathFiles();

  // refreshes the file buttons
  void refreshButtons();

  // closes the dialogue
  void close(bool positive);

  
};



#endif // FILE_CHOOSER