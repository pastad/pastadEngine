#include "PastadEditor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <QtWidgets/QLineEdit>

#include "Engine.h"
#include "IOSubsystem.h"
#include "Log.h"

Object * PastadEditor::m_object;
Ui::PastadEditorClass  PastadEditor::ui;
std::vector<Object *> PastadEditor::m_objects;

PastadEditor::PastadEditor(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  refreshObjectList();
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
    changeObject(obj);
  }
}
void PastadEditor::changeObject(Object * obj)
{
  m_object =obj;
  refreshSelected();
}

void PastadEditor::on_actionLoadScene_triggered()
{
  std::cout << "load scene" <<std::endl;
  Scene * scene = new Scene();
 
  if( scene->load("island-scene.xml") )
  {
    std::cout << "setting scene" << std::endl;
    Engine::setScene(scene, true);
    m_scene = scene;
  }
}

//objects

void PastadEditor::refreshObjectList()
{
  ui.lw_objects_active->clear();
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    scene->acquireLock("refreshObjectList");
    m_objects = scene->getObjects();
   
    for (std::vector<Object *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
    {
      ui.lw_objects_active->insertItem(ui.lw_objects_active->count(),   QString::fromUtf8( (*it)->getIdentifier().c_str()));
    }
    scene->releaseLock("refreshObjectList");
  }
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
}

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


void PastadEditor::on_lw_objects_active_currentRowChanged(int row)
{
  std::cout << row << std::endl;
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    m_object = m_objects[ui.lw_objects_active->currentRow()];
    refreshSelected();
  }
}

void PastadEditor::on_pb_objects_add_clicked()
{
  Scene * scene = Engine::getScene();
  if(scene != nullptr)  
    m_object = scene->addObject("game/models/bush2.obj", glm::vec3(0,0,0), false);  
  refreshSelected();
  refreshObjectList();
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
  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
  
  }
}

// scene time

void PastadEditor::on_pb_scene_play_clicked()
{

}
void PastadEditor::on_pb_scene_pause_clicked()
{

}