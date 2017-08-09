#include "PastadEditor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <QtWidgets/QLineEdit>
#include <QtWidgets\qfiledialog.h>

#include <qopenglcontext.h>
#include <qopenglwidget.h>

#include "Engine.h"
#include "RenderSubsystem.h"
#include "IOSubsystem.h"
#include "Log.h"

#include "Scene.h"
#include "Object.h"
#include "Light.h"
#include "ObjectDatabase.h"
#include "Script.h"
#include "ScriptElement.h"
#include "RotationScriptElement.h"
#include "TranslationScriptElement.h"

#include "PastadEditorObjectDatabase.h"
#include "PastadEditorSuccessorSelector.h"

#include "EngineRequest.h"

Object * PastadEditor::m_object = nullptr;
Object * PastadEditor::m_object_scripts = nullptr;
Light * PastadEditor::m_light = nullptr;
ScriptElement * PastadEditor::m_script_element = nullptr;
Ui::PastadEditorClass  PastadEditor::ui;
std::vector<Object *> PastadEditor::m_objects;
std::vector<Light *> PastadEditor::m_lights;
std::vector<ScriptElement*> PastadEditor::m_script_elements;


bool PastadEditor::m_refreshing = false;

PastadEditor::PastadEditor(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  refreshObjectList();
  refreshLightList();

  ui.le_sc_val1->setVisible(false);
  ui.le_sc_val2->setVisible(false);
  ui.le_sc_val3->setVisible(false);

  loadDatatbase("object_database.xml");
}

PastadEditor::~PastadEditor()
{
}

void PastadEditor::mouseClicked(int btn, bool val)
{
  if (btn == 0)
  {    
    glm::vec2 pos = IOSubsystem::getMouseCoordinates();
    Object * obj = Engine::pickObjectAt(pos);
    if(obj != nullptr)
      changeObject(obj);
    Light * light = Engine::pickLightAt(pos);
    if(light != nullptr)
      changeLight(light);
  }
}

void PastadEditor::keyPressed(int key, bool b)
{
  std::cout << key<< std::endl;
  if (m_object != nullptr && ui.tabWidget->currentIndex() ==0 )
  {
    
    if(key == 324 )
      m_object->setPosition(m_object->getPosition() + glm::vec3(0.1,0,0));
    if (key == 326)
      m_object->setPosition(m_object->getPosition() + glm::vec3(-0.1, 0, 0));
    if (key == 328)
      m_object->setPosition(m_object->getPosition() + glm::vec3(0, 0, 0.1));
    if (key == 322)
      m_object->setPosition(m_object->getPosition() + glm::vec3(0, 0, -0.1));
    if (key == 334)
      m_object->setPosition(m_object->getPosition() + glm::vec3(0, 0.1, 0));
    if (key == 333)
      m_object->setPosition(m_object->getPosition() + glm::vec3(0, -0.1, 0));

    if (key == 321)
      m_object->setRotation( m_object->getRotationDegrees() + glm::vec3(0,0,1));
    if (key == 323)
      m_object->setRotation(m_object->getRotationDegrees() + glm::vec3(0, 0, -1));
   
    if (key == 327)
      m_object->setRotation(m_object->getRotationDegrees() + glm::vec3(1, 0, 0));
    if (key == 329)
      m_object->setRotation(m_object->getRotationDegrees() + glm::vec3(-1, 0, 0));

    if (key == 320)
      m_object->setRotation(m_object->getRotationDegrees() + glm::vec3(0, 1, 0));
    if (key == 330)
      m_object->setRotation(m_object->getRotationDegrees() + glm::vec3(0, -1, 0));

    
  }
  if (m_light != nullptr&& ui.tabWidget->currentIndex() == 1)
  {
    if (key == 324)
      m_light->move(glm::vec3(0.1, 0, 0));
    if (key == 326)
      m_light->move(glm::vec3(-0.1, 0, 0));
    if (key == 328)
      m_light->move(glm::vec3(0, 0, 0.1));
    if (key == 322)
      m_light->move(glm::vec3(0, 0, -0.1));
    if (key == 334)
      m_light->move(glm::vec3(0, 0.1, 0));
    if (key == 333)
      m_light->move(glm::vec3(0, -0.1, 0));
    

    if (key == 327)
      m_light->rotate(glm::vec2(1,0));
    if (key == 329)
      m_light->rotate(glm::vec2(-1, 0));
    if (key == 321)
      m_light->rotate(glm::vec2(0, 1));
    if (key == 323)
      m_light->rotate(glm::vec2(0, -1));

  }
}


void PastadEditor::changeObject(Object * obj)
{
  m_object =obj;
  m_object_scripts = obj;
  refreshSelected();
  refreshObjectList();
}
void PastadEditor::changeLight(Light * light)
{
  m_light = light;
  refreshSelected();
  //ui.openGLWidget->context.

}

// menu

void PastadEditor::on_actionLoadScene_triggered()
{
  QString file_name = QFileDialog::getOpenFileName(this,
    tr("Model file path "), QDir::currentPath(), tr("Model Files (*.*)"));
  if (file_name.toUtf8() == "")
    file_name = QString("Not set");
  else
  {
    std::size_t found = file_name.toStdString().find(QDir::currentPath().toStdString());
    if (found != std::string::npos)
    {
      std::cout << QDir::currentPath().toStdString().size() << std::endl;
      std::string str3 = file_name.toStdString().substr(QDir::currentPath().toStdString().size() + 1);

      LoadSceneRequest * lsr = new LoadSceneRequest(str3);
      Engine::addRequest((EngineRequest *)lsr);
    }
  }
  ui.openGLWidget->makeCurrent();

}
void PastadEditor::on_actionNewObjectDatabase_triggered()
{
  std::cout << "new object database" << std::endl;
  if(m_object_database != nullptr)
    delete m_object_database;


  std::string file_name = QFileDialog::getSaveFileName(this,
    tr("New ObjectDatabase "), QDir::currentPath(), tr("XML Files (*.xml)")).toUtf8();
  
  if( file_name != "")
  {    
    m_object_database = new ObjectDatabase(file_name);
    refreshObjectDatabase();
  }
}
void PastadEditor::on_actionSaveScene_triggered()
{
  std::string file_name = QFileDialog::getSaveFileName(this,
    tr("Save Scene "), QDir::currentPath(), tr("XML Files (*.xml)")).toUtf8();

  if (file_name != "")
  {
    Scene * scene = Engine::getScene();

    if (scene != nullptr)
    {
      scene->acquireLock("peSaveScene");
      Engine::getRenderSubsystem()->acquireRenderLock("saveScene");
      scene->save(file_name);

      Engine::getRenderSubsystem()->releaseRenderLock("saveScene");
      scene->releaseLock("peSaveScene");
    }
  }

}



void PastadEditor::on_actionEditObjectDatabase_triggered()
{
  if(m_object_database != nullptr)
  {
    char *argv[] = { NULL };
    int argc = sizeof(argv) / sizeof(char*) - 1;
   // QApplication in_app(argc, argv);
    PastadEditorObjectDatabase peod(this);
    peod.setObjectDatabase(m_object_database);
    peod.exec();

    m_object_database->save();
    refreshObjectDatabase();
  }

 // in_app.exec();
}
void PastadEditor::on_actionLoadObjectDatabase_triggered()
{
  std::cout << "load object database" << std::endl;
  if (m_object_database != nullptr)
    delete m_object_database;

  std::string file_name = QFileDialog::getOpenFileName(this,
    tr("Load ObjectDatabase "), QDir::currentPath(), tr("XML Files (*.xml)")).toUtf8();
  if (file_name != "")
  {
    m_object_database = new ObjectDatabase("object_database.xml");
    m_object_database->load();
    refreshObjectDatabase();
  }
}
void PastadEditor::loadDatatbase(std::string path)
{
  if (m_object_database != nullptr)
    delete m_object_database;

  if (path != "")
  {
    m_object_database = new ObjectDatabase(path);
    m_object_database->load();
    refreshObjectDatabase();
  }
}


//refreshers

void PastadEditor::refreshObjectList()
{
  std::cout << "ref obj list" << std::endl;
 // while (ui.lw_objects_active->count()>0)
 //   ui.lw_objects_active->takeItem(0);

  //while (ui.lw_sc_objects->count()>0)
  //  ui.lw_sc_objects->takeItem(0);
  ui.lw_sc_objects->reset();
  ui.lw_objects_active->reset();
  ui.lw_sc_objects->clear();
  ui.lw_objects_active->clear();
  std::cout << "ref obj list del" << std::endl;
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
   // scene->acquireLock("refreshObjectList");
    m_objects = scene->getObjects();
   
    for (std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
    {
      if ( (*it) != nullptr)
      {
        ui.lw_objects_active->insertItem(ui.lw_objects_active->count(), QString::fromUtf8((*it)->getIdentifier().c_str()));
        ui.lw_sc_objects->insertItem(ui.lw_sc_objects->count(), QString::fromUtf8((*it)->getIdentifier().c_str()));
      }
    }
  //  scene->releaseLock("refreshObjectList");
  }
  std::cout << "ref obj list end" << std::endl;
}

void PastadEditor::refreshLightList()
{
  m_refreshing = true;
  ui.lw_lights_active->reset();
  while (ui.lw_lights_active->count()>0)
    ui.lw_lights_active->takeItem(0);

  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    scene->acquireLock("refreshLightList");
    m_lights = scene->getLights();

    for (std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
    {
   //   std::cout << "rll" << ui.lw_lights_active->count() << std::endl;
      ui.lw_lights_active->insertItem(ui.lw_lights_active->count(), QString::fromUtf8( (*it)->getDescription().c_str() ));
    }
    scene->releaseLock("refreshLightList");
  }
  std::cout << "rll" << ui.lw_lights_active->count() << std::endl;
  m_refreshing = false;
}

void PastadEditor::refreshSelected()
{
  if (m_object != nullptr)
  {
    ui.lbl_object_txt->setText(QString::fromUtf8(m_object->getIdentifier().c_str() ) );

    glm::vec3 pos = m_object->getPosition();
    std::stringstream ss;
    ss << pos.x;
    ui.le_obj_pos_x->setText( QString::fromUtf8(ss.str( ).c_str() ) );
    ss.str(""); ss.clear();
    ss << pos.y;
    ui.le_obj_pos_y->setText (QString::fromUtf8(ss.str().c_str() ));
    ss.str(""); ss.clear();
    ss << pos.z;
    ui.le_obj_pos_z->setText(QString::fromUtf8(ss.str().c_str() ));
    ss.str(""); ss.clear();

    glm::vec3 rot = m_object->getRotationDegrees();
    ss << rot.x;
    ui.le_obj_rot_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.y;
    ui.le_obj_rot_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.z;
    ui.le_obj_rot_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();



    ui.rb_obj_fixed->setChecked(m_object->isStaticFlagSet());
    ui.rb_obj_apply_physic->setChecked(m_object->isPhysicsApplied() );
    ui.rb_obj_physic_static->setChecked(m_object->isPhysicsStatic());
    ui.rb_obj_visible->setChecked(m_object->getVisibility() == V_All );
    ui.rb_obj_shadow_only->setChecked(m_object->getVisibility() == V_Shadow);
  }
  if (m_light != nullptr)
  {
    glm::vec3 pos = m_light->getPosition();
    std::stringstream ss;
    ss << pos.x;
    ui.le_light_pos_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << pos.y;
    ui.le_light_pos_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << pos.z;
    ui.le_light_pos_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();

    glm::vec3 rot = m_light->getDirection();
    ss << rot.x;
    ui.le_light_rot_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.y;
    ui.le_light_rot_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.z;
    ui.le_light_rot_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();

    glm::vec3 rgb = m_light->getAmbientColor()*255.0f *100.0f;
    ui.sb_light_r->setValue(rgb.x );
    ui.sb_light_g->setValue(rgb.y);
    ui.sb_light_b->setValue(rgb.z);

    ui.lbl_light_txt->setText(QString::fromUtf8( m_light->getDescription().c_str()));

    ui.sb_light_intensity->setValue(m_light->getIntensity() );
  }
}

void PastadEditor::refreshObjectDatabase()
{
  while (ui.lw_objects_chooser->count()>0)
    ui.lw_objects_chooser->takeItem(0);

  if(m_object_database  != nullptr)
  {
    std::vector<ObjectDatabaseEntry> entries = m_object_database->getEntries();
    for (std::vector<ObjectDatabaseEntry>::iterator it = entries.begin(); it != entries.end(); it++)
    {
      ui.lw_objects_chooser->insertItem(ui.lw_objects_chooser->count(), QString::fromUtf8((*it).m_name.c_str()));
    }
  }
}

void PastadEditor::refreshScripts()
{
  while (ui.lw_sc_script_elements->count()>0)
    ui.lw_sc_script_elements->takeItem(0);

  if (m_object_scripts != nullptr)
  {
    Script * script =  m_object_scripts->getScript();
    if(script != nullptr)
    {
      std::vector<ScriptElement *> elems =  script->getScriptElements();
      m_script_elements = elems;   

      std::cout << "ref list" << std::endl;
      for (std::vector<ScriptElement *>::iterator it = elems.begin(); it != elems.end(); it++)
      {
        ui.lw_sc_script_elements->insertItem(ui.lw_sc_script_elements->count(), QString::fromUtf8( (*it)->getDescription().c_str() ));
        std::cout<< (*it) <<std::endl;
      }
    }

  }
}

void PastadEditor::refreshScriptElement()
{
  m_refreshing = true;
  ui.le_sc_val1->setVisible(false);
  ui.le_sc_val2->setVisible(false);
  ui.le_sc_val3->setVisible(false);
  ui.lbl_sc_x->setVisible(false);
  ui.lbl_sc_y->setVisible(false);
  ui.lbl_sc_z->setVisible(false);
  ui.le_sc_from->setVisible(false);
  ui.le_sc_to->setVisible(false);

  ui.lbl_sc_time->setVisible(false);
  ui.le_sc_duration->setVisible(false);
 

  std::cout << "ref" << std::endl;
  std::cout << m_script_element << std::endl;

  if (m_script_element != nullptr)
  {
    if (m_script_element->getType() == SE_ROTATION)
    {
      ui.le_sc_val1->setVisible(true);
      ui.le_sc_val2->setVisible(true);
      ui.le_sc_val3->setVisible(true);
      ui.lbl_sc_x->setVisible(true);
      ui.lbl_sc_y->setVisible(true);
      ui.lbl_sc_z->setVisible(true);

      glm::vec3 vec =  ( (RotationScriptElement *)  m_script_element )->getRotationVector();

      std::stringstream ss;
      ss << vec.x;
      ui.le_sc_val1->setText(QString::fromUtf8( ss.str().c_str() ) );
      ss.clear(); ss.str("");
      ss << vec.y;
      ui.le_sc_val2->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
      ss << vec.z;
      ui.le_sc_val3->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
    }

    if (m_script_element->getType() == SE_TRANSLATION)
    {
      ui.le_sc_val1->setVisible(true);
      ui.le_sc_val2->setVisible(true);
      ui.le_sc_val3->setVisible(true);
      ui.lbl_sc_x->setVisible(true);
      ui.lbl_sc_y->setVisible(true);
      ui.lbl_sc_z->setVisible(true);

      ui.lbl_sc_time->setVisible(true);
      ui.le_sc_duration->setVisible(true);


      glm::vec3 vec = ((TranslationScriptElement *)m_script_element)->getTarget();

      std::stringstream ss;
      ss << vec.x;
      ui.le_sc_val1->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
      ss << vec.y;
      ui.le_sc_val2->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
      ss << vec.z;
      ui.le_sc_val3->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");

      ss << ((TranslationScriptElement *)m_script_element)->getTime();;
      ui.le_sc_duration->setText(QString::fromUtf8(ss.str().c_str()));

    }

    if ((m_script_element->getTrigger() == TRIGGER_FROM_TO) || (m_script_element->getTrigger() == TRIGGER_ON_OFF))
    {
      ui.le_sc_from->setVisible(true);
      ui.le_sc_to->setVisible(true);   
      
      float a,e;

      if (m_script_element->getTrigger() == TRIGGER_FROM_TO)
      {
        a = m_script_element->getStartTime();
        e = m_script_element->getEndTime();
      }
      if (m_script_element->getTrigger() == TRIGGER_ON_OFF)
      {
        a = m_script_element->getTimeSpanOn();
        e = m_script_element->getTimeSpanOff();
      }
      std::stringstream ss;
      ss << a;
      ui.le_sc_from->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
      ss << e;
      ui.le_sc_to->setText(QString::fromUtf8(ss.str().c_str()));
      ss.clear(); ss.str("");
    }
    std::cout << m_script_element->getTrigger() <<std::endl;
    ui.cb_sc_trigger->setCurrentIndex(m_script_element->getTrigger());
  }
  else
  {
    ui.le_sc_val1->setVisible(false);
    ui.le_sc_val2->setVisible(false);
    ui.le_sc_val3->setVisible(false);
 

    ui.le_sc_val1->setText(QString::fromUtf8(""));   
    ui.le_sc_val2->setText(QString::fromUtf8(""));
    ui.le_sc_val3->setText(QString::fromUtf8(""));
  }
  m_refreshing = false;

}

void PastadEditor::refreshAll()
{
  m_object = nullptr;
  m_object_scripts = nullptr;
  m_light = nullptr;
  m_script_element =  nullptr;
  m_objects = std::vector<Object*>();
  m_lights = std::vector<Light*>();
  m_script_elements = std::vector<ScriptElement*>();


  refreshLightList();
  refreshObjectList();
  refreshScripts();
  refreshScriptElement();
  refreshSelected();
}

// savers

void PastadEditor::savePos()
{
  if (m_object != nullptr)
  {
    try
    {
      glm::vec3 p;
      p.x = std::stof(ui.le_obj_pos_x->text().toStdString() );
      p.y = std::stof(ui.le_obj_pos_y->text().toStdString() );
      p.z = std::stof(ui.le_obj_pos_z->text().toStdString() );
      m_object->setPosition(p);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save position");
    }
    refreshSelected();
  }
}
void PastadEditor::saveLightPos()
{
  if (m_light != nullptr)
  {
    try
    {
      glm::vec3 p;
      p.x = std::stof(ui.le_light_pos_x->text().toStdString());
      p.y = std::stof(ui.le_light_pos_y->text().toStdString());
      p.z = std::stof(ui.le_light_pos_z->text().toStdString());
      m_light->setPosition(p);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save position");
    }
    refreshSelected();
  }
}
void PastadEditor::saveRot()
{
  if (m_object != nullptr)
  {
    try
    {
      glm::vec3 r;
      r.x = std::stof(ui.le_obj_rot_x->text().toStdString());
      r.y = std::stof(ui.le_obj_rot_y->text().toStdString());
      r.z = std::stof(ui.le_obj_rot_z->text().toStdString());

      bool valid = true;

      if( (r.x < 0.0f ) ||  (r.x>360.0f) )
        valid = false;
      if ((r.y < 0.0f) || (r.y>360.0f))
        valid = false;
      if ((r.z < 0.0f) || (r.z>360.0f))
        valid = false;

      if(valid)
       m_object->setRotation(r);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save rotation");
    }
    refreshSelected();
  }
}
void PastadEditor::saveLightRot()
{
  if (m_light != nullptr)
  {
    try
    {
      glm::vec3 r;
      r.x = std::stof(ui.le_light_rot_x->text().toStdString());
      r.y = std::stof(ui.le_light_rot_y->text().toStdString());
      r.z = std::stof(ui.le_light_rot_z->text().toStdString());

      bool valid = true;

      if ((r.x < -1.0f) || (r.x>1.0f))
        valid = false;
      if ((r.y < -1.0f) || (r.y>1.0f))
        valid = false;
      if ((r.z < -1.0f) || (r.z>1.0f))
        valid = false;

      if (valid)
        m_light->setDirection(r);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save rotation");
    }
    refreshSelected();
  }
}


void PastadEditor::saveObjectSpecs()
{
  if (m_object != nullptr)
  {
    bool vis = ui.rb_obj_visible->isChecked();
    bool sro = ui.rb_obj_shadow_only->isChecked();

    if (sro)
    {
      m_object->setVisibility(V_Shadow);
    }
    else
    {
      if(!vis)
        m_object->setVisibility(V_None);
      else
        m_object->setVisibility(V_All);
    }
    
    bool ap = ui.rb_obj_apply_physic->isChecked();
    bool aps = ui.rb_obj_physic_static->isChecked();
    bool fixed = ui.rb_obj_fixed->isChecked();

    if (fixed)
    {
      // is fixed
      if (aps)
      {
        // and physics static 
        m_object->applyPhysics();
        m_object->applyPhysicsStatic();
        m_object->setStaticFlag();
      }
      else
      {
        // isnt physic static
        if (ap)
        {
          // so it is a physics obj but not physic static => cant be fixed
          m_object->applyPhysics();
          m_object->dontApplyPhysicsStatic();
          m_object->unsetStaticFlag();
        }
        else
        {
          // is a fixed object without physics
          m_object->dontApplyPhysics();
          m_object->dontApplyPhysicsStatic();
          m_object->setStaticFlag();
        }
      }
    }
    else
    {
      // it isn't fixed
      if( aps  )
      { 
        // and physics static -> so it should be fixed
        m_object->applyPhysics();
        m_object->applyPhysicsStatic();
        m_object->setStaticFlag();
      }
      else
      {
        // and isn't physic static
        if(ap)
        {
          // dynamic physic object
          m_object->applyPhysics();
          m_object->dontApplyPhysicsStatic();
          m_object->unsetStaticFlag();
        }
        else
        {
          // a dynamic object without physic
          m_object->dontApplyPhysics();
          m_object->dontApplyPhysicsStatic();
          m_object->unsetStaticFlag();
        }
      }
    }
  }  

  refreshSelected();
}




// click and changes


void PastadEditor::on_lw_objects_chooser_currentRowChanged(int row)
{
  std::cout <<"orcc"<< row << std::endl;
  if( m_object_database != nullptr )
  {
    int cr = ui.lw_objects_chooser->currentRow();
    std::cout << "orcc _" << cr << std::endl;
    if ( (cr< m_object_database->getEntries().size() ) && (cr >= 0))
    {
      ObjectDatabaseEntry entry = m_object_database->getEntries()[ui.lw_objects_chooser->currentRow()];
      ui.lbl_objectdb_txt->setText(QString::fromUtf8(entry.m_name.c_str()) );
      ui.tb_objectdb_description->setText(QString::fromUtf8(entry.m_description.c_str()));
 
      QPixmap pix(entry.m_icon_path.c_str());
      int w = ui.lbl_objectdb_selected->width();
      int h = ui.lbl_objectdb_selected->height();
      ui.lbl_objectdb_selected->setPixmap(pix.scaled(w, h, Qt::IgnoreAspectRatio));
 
    }
  }
}

void PastadEditor::on_lw_objects_active_currentRowChanged(int row)
{
  std::cout << "orc" << row << std::endl;
  Scene * scene = Engine::getScene();
  if (scene != nullptr && (m_objects.size() >0))
  {
    int cr = ui.lw_objects_active->currentRow();
    if (cr < m_objects.size() && (cr >= 0))
    {
      m_object = m_objects[ui.lw_objects_active->currentRow()];
      refreshSelected();
    }
  }
}

void PastadEditor::on_lw_sc_objects_currentRowChanged(int row)
{
  std::cout << "sorc" << row << std::endl;
  Scene * scene = Engine::getScene();
  if (scene != nullptr && (m_objects.size() >0))
  {
    int cr = ui.lw_sc_objects->currentRow();
    if (cr < m_objects.size() && (cr >= 0))
    {
      m_object_scripts = m_objects[ui.lw_sc_objects->currentRow()];
      refreshScripts();
    }
  }
}

void PastadEditor::on_lw_lights_active_currentRowChanged(int row)
{
  if(!m_refreshing)
  {
    std::cout <<"lrc" <<  ui.lw_lights_active->currentRow() << ", active "<< ui.lw_lights_active->count()<< std::endl;
    Scene * scene = Engine::getScene();
    if (scene != nullptr  && (m_lights.size() >0) )
    { 
      int cr  = ui.lw_lights_active->currentRow();
      if (cr < ui.lw_lights_active->count()   && (cr >=0)  )
      {
        m_light = m_lights[ui.lw_lights_active->currentRow()];
        refreshSelected();
      }
    }
    std::cout << "done" <<std::endl;
  }
}

void PastadEditor::on_lw_sc_script_elements_currentRowChanged(int row)
{
  std::cout << "serc" << row << std::endl;
  Scene * scene = Engine::getScene();
  if (scene != nullptr && (m_script_elements.size() >0))
  {
    int cr = ui.lw_sc_script_elements->currentRow();
    if (cr < m_script_elements.size() && (cr >= 0))
    {
      m_script_element = m_script_elements[ui.lw_sc_script_elements->currentRow()];
      refreshScriptElement();
    }
  }
}

void PastadEditor::on_pb_objects_add_clicked()
{
  Scene * scene = Engine::getScene();
  if ( m_object_database != nullptr && (scene != nullptr) )
  {
    ObjectDatabaseEntry entry = m_object_database->getEntries()[ui.lw_objects_chooser->currentRow()];

    if (scene != nullptr )
    {    
     // Engine::requestObjectLoad(entry.m_path);
      AddObjectRequest * aor = new AddObjectRequest(entry.m_path, ui.rb_obj_fixed->isChecked(), ui.rb_obj_shadow_only->isChecked(), ui.rb_obj_visible->isChecked(), ui.rb_obj_apply_physic->isChecked(), ui.rb_obj_physic_static->isChecked());
      Engine::addRequest( (EngineRequest *) aor );
    }
   // refreshSelected();
   // refreshObjectList();
  }
}

void PastadEditor::on_rb_obj_fixed_clicked(bool value)
{
  if( value )
    Engine::getLog()->log(LF_Editor, "PastadEditor", "pressed");
  else
    Engine::getLog()->log(LF_Editor, "PastadEditor", "not pressed");
}

void PastadEditor::on_le_obj_pos_x_editingFinished()
{  savePos();}
void PastadEditor::on_le_obj_pos_y_editingFinished()
{  savePos();}
void PastadEditor::on_le_obj_pos_z_editingFinished()
{  savePos();}
void PastadEditor::on_le_obj_rot_x_editingFinished()
{  saveRot();}
void PastadEditor::on_le_obj_rot_y_editingFinished()
{  saveRot();}
void PastadEditor::on_le_obj_rot_z_editingFinished()
{  saveRot();}
void PastadEditor::on_rb_obj_apply_physic_clicked(bool value)
{  saveObjectSpecs(); }
void PastadEditor::on_rb_obj_physic_static_clicked(bool value)
{  saveObjectSpecs();}
void PastadEditor::on_rb_obj_visible_clicked(bool value)
{  saveObjectSpecs();}
void PastadEditor::on_rb_obj_shadow_only_clicked(bool value)
{  saveObjectSpecs();}


// lights

void PastadEditor::on_pb_lights_add_clicked()
{  
  std::string sel = ui.cb_lights_type->currentText().toStdString();

  unsigned int type = 0;
  if (sel == "directional")
    type = LIGHT_DIRECTIONAL;
  if (sel == "spot")
    type = LIGHT_SPOT;
  if (sel == "point")
    type = LIGHT_POINT;

  AddLightRequest * alr = new AddLightRequest(type , ui.rb_lights_cast_shadow->isChecked());
  Engine::addRequest((EngineRequest *)alr);
}

// scene time

void PastadEditor::on_pb_scene_play_clicked()
{
  Scene * scene = Engine::getScene();
  if(scene != nullptr)
  {
    scene->acquireLock("scenePlay");
    scene->setTimeAdvance(1.0f);
    scene->releaseLock("scenePlay");
  }
}
void PastadEditor::on_pb_scene_pause_clicked()
{
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    scene->acquireLock("scenePause");
    scene->setTimeAdvance(0.0f);
    scene->releaseLock("scenePause");
  }
}
void PastadEditor::on_pb_scene_reset_clicked()
{
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    scene->acquireLock("scenePause");
    scene->setTime(0.0f);
    for (std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
    {
      Script * script = (*it)->getScript();
      if (script != nullptr)
      {
        script->reset();
      }
      (*it)->resetToStart();
    }

    scene->releaseLock("scenePause");
  }
}

void  PastadEditor::on_pb_refresh_shaders_clicked()
{ 
  RefresShaderRequest * rsr = new RefresShaderRequest();
  Engine::addRequest( (EngineRequest *) rsr);
}

void PastadEditor::on_sb_light_r_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.x = ((float) ui.sb_light_r->value() )/ 255.0f;
    m_light->setColor(r);
  }
}
void PastadEditor::on_sb_light_g_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.y = ui.sb_light_g->value() / 255.0f;
    m_light->setColor(r);
  }
}
void PastadEditor::on_sb_light_b_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.z = ui.sb_light_b->value() / 255.0f;
    m_light->setColor(r);
  }
}

void PastadEditor::on_le_light_pos_x_editingFinished()
{
  saveLightPos();
}
void PastadEditor::on_le_light_pos_y_editingFinished()
{
  saveLightPos();
}
void PastadEditor::on_le_light_pos_z_editingFinished()
{
  saveLightPos();
}
void PastadEditor::on_le_light_rot_x_editingFinished()
{
  saveLightRot();
}
void PastadEditor::on_le_light_rot_y_editingFinished()
{
  saveLightRot();
}
void PastadEditor::on_le_light_rot_z_editingFinished()
{
  saveLightRot();
}

void PastadEditor::on_sb_light_intensity_valueChanged(double d)
{
  if (m_light != nullptr)
  {
   // std::cout << ui.sb_light_intensity->value() << std::endl;
    m_light->setIntensity(ui.sb_light_intensity->value());
  }
}

void PastadEditor::on_pb_object_remove_clicked()
{
  Scene * scene = Engine::getScene();
  if (m_object != nullptr && (scene != nullptr))
  {
    RemoveObjectRequest * rmr = new RemoveObjectRequest(m_object);
    Engine::addRequest((EngineRequest *)rmr);
   // scene->removeObject(m_object);
    m_object = nullptr;
    refreshSelected();
  //  refreshObjectList();
  }
  else
  {
    Engine::getLog()->log("PastadEditor", "couldn't remove object");
  }
}
void PastadEditor::on_pb_light_remove_clicked()
{
  Scene * scene = Engine::getScene();
  if (m_light != nullptr && (scene != nullptr))
  {
    RemoveLightRequest * rmr = new RemoveLightRequest(m_light);
    Engine::addRequest( (EngineRequest *) rmr  );
    m_light = nullptr;
    refreshSelected(); 
  }
}

void PastadEditor::on_cb_shadow_type_currentIndexChanged(int idx)
{
//  std::cout << idx <<std::endl;
  SetShadowTechniqueDirectionalRequest * sstr = new SetShadowTechniqueDirectionalRequest( (ShadowTechniqueType) idx);
  Engine::addRequest( (EngineRequest *) sstr );
}
void PastadEditor::on_cb_shadow_type_point_currentIndexChanged(int idx)
{
  //  std::cout << idx <<std::endl;
  SetShadowTechniquePointRequest * sstr = new SetShadowTechniquePointRequest((ShadowTechniqueType)idx);
  Engine::addRequest((EngineRequest *)sstr);
}

void PastadEditor::on_cb_sc_trigger_currentIndexChanged(int idx)
{
  if( m_script_element != nullptr && !m_refreshing)
  {  
    std::cout << "set new trigger" <<std::endl;
    if(idx == 0)
      m_script_element->setupAlways();
    if(idx == 1)
      m_script_element->setupFromTo(0,0);
    if(idx == 2)
      m_script_element->setupOnOff(0, 0);
    refreshScripts();
    refreshScriptElement();
  }
}

void PastadEditor::on_cb_anti_aliasing_currentIndexChanged(int idx)
{
  if( idx == 0)
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_FXAA, false);
    Engine::addRequest((EngineRequest *)spptr);
  }
  else
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_FXAA, true);
    Engine::addRequest((EngineRequest *)spptr);
  }
}

void PastadEditor::on_rb_hdr_clicked(bool value)
{
  if (value)
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_HDR, true);
    Engine::addRequest((EngineRequest *)spptr);
  }
  else
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_HDR, false);
    Engine::addRequest((EngineRequest *)spptr);
  }
}
void PastadEditor::on_rb_bloom_clicked(bool value)
{
  if (value)
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_BLOOM, true);
    Engine::addRequest((EngineRequest *)spptr);
  }
  else
  {
    SetPPTechniqueRequest * spptr = new SetPPTechniqueRequest(PP_BLOOM, false);
    Engine::addRequest((EngineRequest *)spptr);
  }
}
void PastadEditor::on_rb_ssao_clicked(bool value)
{
  SetShadowTechniqueAdditionalRequest * sstr = new SetShadowTechniqueAdditionalRequest( value ?  ST_SSAO : ST_NONE);
  Engine::addRequest((EngineRequest *)sstr);
}

void PastadEditor::on_pb_add_script_clicked()
{
  if (m_object_scripts != nullptr)
  {
    m_object_scripts->acquireLockHard();
    Script * script = m_object_scripts->getScript();
    if(script == nullptr)
      script = m_object_scripts->addScript();

    ScriptElement *se =nullptr;
    
    if (ui.cb_sc_type->currentIndex() == 0)
    {
      RotationScriptElement * rse = script->addRotationScript();
      se = (ScriptElement *)rse;
    }
    if (ui.cb_sc_type->currentIndex() == 1)
    {
      TranslationScriptElement * tse = script->addTranslationScript();
      se = (ScriptElement *)tse;
    }


    if( ui.cb_sc_trigger->currentIndex() == 0)
    {
      se->setupAlways();
    }
    if (ui.cb_sc_trigger->currentIndex() == 1)
    {    
      se->setupFromTo(0,0);
    }
    if (ui.cb_sc_trigger->currentIndex() == 2)
    {
      se->setupOnOff(0,0);    
    }

    m_script_element = se;

    refreshScripts();  
    refreshScriptElement();

    m_object_scripts->releaseLock();
  }
}
void PastadEditor::on_pb_add_successor_clicked()
{
  if (m_script_element != nullptr)
  {
    PastadEditorSuccessorSelector pess(this);
    pess.setup( m_script_element->getSuccessors(), m_script_elements, m_script_element);
    pess.exec();

    std::vector<ScriptElement *> ses = pess.getSelected();

    for (std::vector<ScriptElement *>::iterator it = ses.begin(); it != ses.end(); it++)
    {
      m_script_element->setSuccessor( (*it) );
      (*it)->waitForTrigger();
    }
  }
  


}


void PastadEditor::on_pb_remove_script_clicked()
{
  if (m_object_scripts != nullptr)
  {
    m_object_scripts->acquireLockHard();
    
    Script * script = m_object_scripts->getScript();

    if (script != nullptr)
    {
      if (m_script_element != nullptr)
      {
        script->removeScriptElement(m_script_element);
        m_script_element = nullptr;
      }
    }

    m_object_scripts->releaseLock();
  }
  refreshScripts();
  refreshScriptElement();
}

void PastadEditor::on_le_sc_val1_editingFinished()
{
  saveScriptElement();
}

void PastadEditor::on_le_sc_val2_editingFinished()
{
  saveScriptElement();
}

void PastadEditor::on_le_sc_val3_editingFinished()
{
  saveScriptElement();
}

void PastadEditor::on_le_sc_from_editingFinished()
{
  saveScriptElement();
}
void PastadEditor::on_le_sc_to_editingFinished()
{
  saveScriptElement();
}
void PastadEditor::on_le_sc_duration_editingFinished()
{
  saveScriptElement();
}

void PastadEditor::saveScriptElement()
{
  if (m_script_element != nullptr)
  {
    try
    {
      if (m_script_element->getType() == SE_ROTATION)
      {
    
          glm::vec3 p;
          p.x = std::stof(ui.le_sc_val1->text().toStdString());
          p.y = std::stof(ui.le_sc_val2->text().toStdString());
          p.z = std::stof(ui.le_sc_val3->text().toStdString());
          ( (RotationScriptElement *) m_script_element )->setup(p);      
      } 
      if (m_script_element->getType() == SE_TRANSLATION)
      {
        glm::vec3 p;
        p.x = std::stof(ui.le_sc_val1->text().toStdString());
        p.y = std::stof(ui.le_sc_val2->text().toStdString());
        p.z = std::stof(ui.le_sc_val3->text().toStdString());

        float t =  std::stof(ui.le_sc_duration->text().toStdString());

        ((TranslationScriptElement *)m_script_element)->setup(p, t);
      }

      if (m_script_element->getTrigger() == TRIGGER_ALWAYS)
      {
        m_script_element->setupAlways();
      }
      if (m_script_element->getTrigger() == TRIGGER_FROM_TO)
      {
        float a = std::stof(ui.le_sc_from->text().toStdString());
        float e = std::stof(ui.le_sc_to->text().toStdString());
        m_script_element->setupFromTo(a, e);
      }
      if (m_script_element->getTrigger() == TRIGGER_ON_OFF)
      {
        float a = std::stof(ui.le_sc_from->text().toStdString());
        float e = std::stof(ui.le_sc_to->text().toStdString());
        m_script_element->setupOnOff(a, e);
      }

      refreshScriptElement();
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save rot vector");
    }
  }
}


void PastadEditor::setTime(std::string time)
{
  ui.lbl_time->setText(QString::fromUtf8(time.c_str()));
}
void PastadEditor::setScene(Scene * scene)
{
  refreshLightList();
  refreshObjectList(); 
}
