#ifndef FILE_VIEWER
#define FILE_VIEWER

#include "GUI.h"
#include <boost/filesystem.hpp>

// class for a file chooser gui

#define FC_STATUS_NOTACTIVE 0
#define FC_STATUS_ACTIVE 1
#define FC_STATUS_DONE_OK 2
#define FC_STATUS_DONE_CANCLED 3

class FileViewer : GUI
{
public:
  FileViewer();
  ~FileViewer();

  FileViewer(const FileViewer& FileViewer) = delete;
  FileViewer& operator=(const FileViewer& other) = delete;

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

  // altivates the ok button for file saving
  void fileTyped();

  // sets the file in the current directory to chosen one
  void chosenFile(std::string filename);

  // resets the file chooser | ususally when status was read
  void reset();

  // shifts the file view up or down
  void shiftView(int delta);

  // returns the selected file name
  std::string getFileName();

  // identifier getter setter
  std::string getIdentifier();
  void setIdentifier(std::string);

protected:

  Image * m_back_panel; 
  Button *m_btn_close;
  Button *m_btn_ok;
  Button *m_btn_up;
  Button *m_btn_down; 
  EditText * m_txt_selected;
  Text * m_txt_path;

  // callback four mouse button press
  static void mouseButtonCallback(Button * btn);

  // calback fot edit text changed
  static void editTextChangedCallback(EditText * edit_txt);

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
  std::map<std::string,boost::filesystem::path> m_current_path_files;
  std::map<std::string,boost::filesystem::path> m_current_path_directories;

  // the dir/file offset
  unsigned int m_view_offset = 0;

  // the identifier for the fc
  std::string m_identifier;

  // the path that will be returned
  std::string m_return_path;

  // loads the files in m_current_path
  void loadPathFiles();

  // refreshes the file buttons
  void refreshButtons();

  // closes the dialogue
  void close(bool positive);

  
};



#endif // FILE_VIEWER