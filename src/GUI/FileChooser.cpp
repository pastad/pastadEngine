#include "FileChooser.h"

#include "Engine.h"
#include "Log.h"


#include "Button.h"
#include "Image.h"
#include "Text.h"
#include <boost/algorithm/string/predicate.hpp>

#include <iostream>
#include <sstream>

#define TOP_OFFSET 60
#define FILE_AMOUNT 30



FileChooser::FileChooser(): GUI( 0 )
{}


FileChooser::~FileChooser()
{}

bool FileChooser::initialize()
{
  m_back_panel = GUI::getImage();
  m_back_panel->load("resources/panel_square.png");
  glm::vec2 pos = glm::vec2(200,Engine::getWindowHeight() -540.0f);
  m_back_panel->setPosition(pos);

  m_btn_close = GUI::getButton();
  m_btn_close->intitialize("resources/btn.png","",pos+ m_back_panel->getSize()- glm::vec2(30,20),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "close");

  m_btn_ok = GUI::getButton();
  m_btn_ok->intitialize("resources/btn_small_border.png","OK",pos+ glm::vec2(m_back_panel->getSize().x,20)- glm::vec2(50,0),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "ok");
  m_btn_ok->setInactive();

  m_txt_selected = GUI::getText();
  m_txt_selected->setPosition(pos +glm::vec2(20,20));
  m_txt_selected->setScale(0.3f);
  m_txt_selected->setColor(glm::vec3(0,0,0));
  m_txt_selected->setText("");

  m_txt_path = GUI::getText();
  m_txt_path->setPosition(pos +glm::vec2(20,m_back_panel->getSize().y-30));
  m_txt_path->setScale(0.3f);
  m_txt_path->setColor(glm::vec3(0,0,0));
  m_txt_path->setText("");

  GUI::registerButtonPressedCallback(mouseButtonCallback);
  setSizeAndOffset(m_back_panel->getSize(), pos);

  for(unsigned int c=0; c < FILE_AMOUNT ; c++)
  {
    Button * btn = GUI::getButton();
    std::stringstream ss;
    ss<< "file_btn_"<<c;
    btn->intitialize("resources/btn_large_empty.png","",pos+ glm::vec2(15,m_back_panel->getSize().y-TOP_OFFSET)- glm::vec2(0,15 *c) ,glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), ss.str());
    m_files.insert(m_files.end(), btn);
    btn->setTextFloat(TEXT_FLOAT_LEFT);
  }

  m_status = FC_STATUS_NOTACTIVE;
 
  setInactive(); 


  return true;
}

unsigned int FileChooser::getStatus()
{
  return m_status;
}
void FileChooser::reset()
{
  m_status = FC_STATUS_NOTACTIVE;
}
std::string  FileChooser::getFileName()
{
  return m_current_file.string();
}

void FileChooser::start()
{   
  m_files[0]->setDescriptor("./");
  m_files[0]->setText("./");
  m_current_path = boost::filesystem::path( boost::filesystem::current_path() );
  m_txt_path->setText(m_current_path.string());
  loadPathFiles();
  setActive();
  refreshButtons();
  m_btn_ok->setInactive();
  m_status = FC_STATUS_ACTIVE;
}
void FileChooser::close(bool positive)
{  
  std::cout << "closing file chooser ok:"<<positive <<std::endl;
  setInactive();
  if(positive)
    m_status = FC_STATUS_DONE_OK;
  else
    m_status = FC_STATUS_DONE_CANCLED;
}

void FileChooser::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "close")
  {  
    ( (FileChooser*)btn->getParent() )->close(false);
  }
  else
  {
    if(btn->getDescriptor() == "ok")
    {
      ( (FileChooser*)btn->getParent() )->close(true);
    }
    else
    { 
      if(btn->getDescriptor() != "")
      {
        if( boost::starts_with(btn->getDescriptor(),"./") )
        {
          ( (FileChooser *) btn->getParent() )->chosenParentDirectory();
        }
        else
        {
          if( boost::starts_with(btn->getDescriptor(),"/") )
          {
            Engine::getLog()->log("FileChooser", "selected directory: ", btn->getDescriptor());
            ( (FileChooser *) btn->getParent() )->chosenDirectory(btn->getDescriptor()) ;
         
          }
          else
          {
            Engine::getLog()->log("FileChooser", "selected file: ", btn->getDescriptor());
            ( (FileChooser *) btn->getParent() )->chosenFile(btn->getDescriptor()) ;
          }
        }
      }
    }
  }
}

void FileChooser::chosenDirectory(std::string dir)
{
  m_current_path /= dir;
  loadPathFiles();
  m_view_offset =0;
  refreshButtons();
  m_txt_selected->setText("");
  m_txt_path->setText(m_current_path.string());
  m_btn_ok->setInactive();
}
void FileChooser::chosenParentDirectory()
{
  m_current_path = m_current_path.parent_path();
  loadPathFiles();
  m_view_offset =0;
  refreshButtons();  
  m_txt_selected->setText("");
  m_txt_path->setText(m_current_path.string());
  m_btn_ok->setInactive();
}
void FileChooser::chosenFile(std::string file_name)
{
  m_current_file = m_current_path / file_name;
  m_txt_selected->setText(file_name);
  m_btn_ok->setActive();
}

void FileChooser::loadPathFiles()
{
  m_current_path_files.clear();
  m_current_path_directories.clear();
  if (!m_current_path.empty())
  {
    namespace fs = boost::filesystem;

    fs::path apk_path(m_current_path);
    fs::directory_iterator end;

    for (fs::directory_iterator i(apk_path); i != end; ++i)
    {
        const fs::path cp = (*i);
        if( ! fs::is_directory(cp))
          m_current_path_files.push_back(cp);
        else
          m_current_path_directories.push_back(cp);  
    }
  }
}
void FileChooser::refreshButtons()
{

  for(unsigned int c=1; c < FILE_AMOUNT ; c++)
  {
    unsigned int idx = m_view_offset + c;
    Button * btn = m_files[c];

    if( idx  < m_current_path_directories.size())
    {
      std::string file_name = m_current_path_directories[idx].filename().string() ;

      btn->setText("/"+file_name);
      btn->setDescriptor("/"+file_name);
      Engine::getLog()->log("FileChooser", "set dir ",file_name);
    }
    else
    {
      if(m_current_path_files.size() > idx)
      {
        std::string file_name = m_current_path_files[ idx - m_current_path_directories.size() ].filename().string() ;

        btn->setText(file_name);
        btn->setDescriptor(file_name);
      }
      else
      {
        btn->setText("");
        btn->setDescriptor("");
      }
    } 
  }
}