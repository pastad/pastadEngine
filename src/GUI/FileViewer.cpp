#include "FileViewer.h"

#include "Engine.h"
#include "Log.h"


#include "Button.h"
#include "Image.h"
#include "Text.h"
#include "EditText.h"
#include <boost/algorithm/string/predicate.hpp>

#include <iostream>
#include <sstream>

#define TOP_OFFSET 60
#define FILE_AMOUNT 28



FileViewer::FileViewer(): GUI( 0 )
{}

FileViewer::~FileViewer()
{}


// initialization/close -------------------------------------------------

bool FileViewer::initialize()
{
  m_back_panel = GUI::addImage();
  m_back_panel->load("resources/panel_square.png");
  glm::vec2 pos = glm::vec2(200,Engine::getWindowHeight() -540.0f);
  m_back_panel->setPosition(pos);

  m_btn_close = GUI::addButton();
  m_btn_close->intitialize("resources/btn.png","",pos+ m_back_panel->getSize()- glm::vec2(30,20),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "close");

  m_btn_ok = GUI::addButton();
  m_btn_ok->intitialize("resources/btn_small_border.png","OK",pos+ glm::vec2(m_back_panel->getSize().x,20)- glm::vec2(50,0),glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), "ok");
  m_btn_ok->setInactive();

  m_btn_up = GUI::addButton();
  m_btn_up->intitialize("resources/btn_up.png","",pos+ glm::vec2(10,m_back_panel->getSize().y-50),glm::vec2(0.6f,0.6f),
    glm::vec3(0,0,0), "up");

  m_btn_down = GUI::addButton();
  m_btn_down->intitialize("resources/btn_down.png","",pos+ glm::vec2(10,40),glm::vec2(0.6f,0.6f),
    glm::vec3(0,0,0), "down");

  m_txt_selected = GUI::addEditText();
  m_txt_selected->set( "default_file", pos +glm::vec2(20,20), 0.22f, glm::vec3(0,0,0));
  m_txt_selected->setDescriptor("path_chosen");

  m_txt_path = GUI::addText();
  m_txt_path->setPosition(pos +glm::vec2(20,m_back_panel->getSize().y-30));
  m_txt_path->setScale(0.3f);
  m_txt_path->setColor(glm::vec3(0,0,0));
  m_txt_path->setText("");

  setSizeAndOffset(m_back_panel->getSize(), pos);

  for(unsigned int c=0; c < FILE_AMOUNT ; c++)
  {
    Button * btn = GUI::addButton();
    std::stringstream ss;
    ss<< "file_btn_"<<c;
    btn->intitialize("resources/btn_large_empty.png","",pos+ glm::vec2(15,m_back_panel->getSize().y-TOP_OFFSET)- glm::vec2(0,15 *c) ,glm::vec2(0.5f,0.5f),
    glm::vec3(0,0,0), ss.str());
    m_files.insert(m_files.end(), btn);
    btn->setTextFloat(TEXT_FLOAT_LEFT);
  }

  m_status = FC_STATUS_NOTACTIVE;
 
  setInactive(); 

  GUI::registerButtonPressedCallback(mouseButtonCallback);
  GUI::registerEditTextCallback(editTextChangedCallback);

  return true;
}

void FileViewer::start()
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

void FileViewer::close(bool positive)
{  
  std::cout << "closing FileViewer ok:"<<positive <<std::endl;
  setInactive();

  boost::filesystem::path p  = m_current_path;
  p /= m_txt_selected->getText();
  m_return_path = p.string();
  std::cout << "FileViewer path:"<<m_return_path <<std::endl;

  if(positive)
    m_status = FC_STATUS_DONE_OK;
  else
    m_status = FC_STATUS_DONE_CANCLED;
}


// refreshs -------------------------------------------------

void FileViewer::refreshButtons()
{

  std::map<std::string,boost::filesystem::path>::iterator it = m_current_path_directories.begin();
  std::map<std::string,boost::filesystem::path>::iterator it2 = m_current_path_files.begin();

  for(int x=0; x<m_view_offset;x++)
  {
    if(x <m_current_path_directories.size())
      it++;
    it2++;
  }
  for(unsigned int c=1; c < FILE_AMOUNT ; c++)
  {
    unsigned int idx = m_view_offset + c-1;
    Button * btn = m_files[c];

    if( idx  < m_current_path_directories.size())
    {
      std::string file_name = it->second.filename().string() ;

      btn->setText("/"+file_name);
      btn->setDescriptor("/"+file_name);
      Engine::getLog()->log("FileViewer", "set dir ",file_name);
      it++;
    }
    else
    {
      if(m_current_path_files.size() > idx- m_current_path_directories.size())
      {
        std::string file_name = it2->second.filename().string() ;

        btn->setText(file_name);
        btn->setDescriptor(file_name);
        it2++;
      }
      else
      {
        btn->setText("");
        btn->setDescriptor("");
      }
    } 
  }
}

void FileViewer::loadPathFiles()
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

      //std::cout << cp.filename().string() <<std::endl;
      if( ! fs::is_directory(cp))
        m_current_path_files.insert(std::pair<std::string,boost::filesystem::path>( cp.filename().string(),  cp) );
      else
        m_current_path_directories.insert(std::pair<std::string,boost::filesystem::path>( cp.filename().string(),  cp) );
    }
  }
}


// callbacks -------------------------------------------------

void FileViewer::mouseButtonCallback(Button * btn)
{
  if(btn->getDescriptor() == "close")
  {  
    ( (FileViewer*)btn->getParent() )->close(false);
  }
  else
  { 
    if( btn->getDescriptor() == "ok")
    {
      ( (FileViewer*)btn->getParent() )->close(true);
    }  
    else
    {
      if( (btn->getDescriptor() == "up") || (btn->getDescriptor() == "down") )
      {
        if( btn->getDescriptor() == "up")        
          ( (FileViewer *) btn->getParent() )->shiftView(-1);        
        else        
          ( (FileViewer *) btn->getParent() )->shiftView(1);
        
      }
      else
      {
        if(btn->getDescriptor() != "")
        {
          if( boost::starts_with(btn->getDescriptor(),"./") )
          {
            ( (FileViewer *) btn->getParent() )->chosenParentDirectory();
          }
          else
          {
            if( boost::starts_with(btn->getDescriptor(),"/") )
            {
              Engine::getLog()->log("FileViewer", "selected directory: ", btn->getDescriptor());
              ( (FileViewer *) btn->getParent() )->chosenDirectory(btn->getDescriptor()) ;
           
            }
            else
            {
              Engine::getLog()->log("FileViewer", "selected file: ", btn->getDescriptor());
              ( (FileViewer *) btn->getParent() )->chosenFile(btn->getDescriptor()) ;
            }
          }
        }
      }  
    }  
  }
}

void FileViewer::editTextChangedCallback(EditText * edit_txt)
{
  if( edit_txt->getDescriptor() == "path_chosen"  )
  {
    Engine::getLog()->log("SceneEditor", "file name typed in");
    ( (FileViewer *) edit_txt->getParent() )->fileTyped();
  }
}

void FileViewer::fileTyped()
{
  m_btn_ok->setActive();
}

void FileViewer::shiftView(int delta)
{  
  int s = m_current_path_directories.size() + m_current_path_files.size();
  if( ! ( (m_view_offset== 0) && (delta == -1) )   && ! ( (m_view_offset+FILE_AMOUNT > s)  && (delta == 1))  )
    m_view_offset+=delta;
  
  std::cout << s << m_view_offset <<std::endl;

  refreshButtons();
}

void FileViewer::chosenDirectory(std::string dir)
{
  m_current_path /= dir;
  loadPathFiles();
  m_view_offset =0;
  refreshButtons();
  m_txt_selected->setText("");
  m_txt_path->setText(m_current_path.string());
  m_btn_ok->setInactive();
}
void FileViewer::chosenParentDirectory()
{
  m_current_path = m_current_path.parent_path();
  loadPathFiles();
  m_view_offset =0;
  refreshButtons();  
  m_txt_selected->setText("");
  m_txt_path->setText(m_current_path.string());
  m_btn_ok->setInactive();
}
void FileViewer::chosenFile(std::string file_name)
{
  m_current_file = m_current_path / file_name;
  m_txt_selected->setText(file_name);
  m_btn_ok->setActive();
}


// getters/setters -------------------------------------------------

std::string FileViewer::getIdentifier()
{
  return m_identifier;
}

void FileViewer::setIdentifier(std::string idf)
{
  m_identifier = idf;
}

unsigned int FileViewer::getStatus()
{
  return m_status;
}

void FileViewer::reset()
{
  m_status = FC_STATUS_NOTACTIVE;
}

std::string  FileViewer::getFileName()
{
  return m_return_path;
}
