#include "editorwindow.h"
#include "ui_editorwindow.h"

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"
#include "Log.h"

#include "Object.h"
#include "Light.h"
#include "ObjectDatabase.h"
#include "Script.h"
#include "ScriptElement.h"
#include "RotationScriptElement.h"
#include "TranslationScriptElement.h"
#include "openglwindow.h"

#include <PastadEditorObjectDatabase.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QFileDialog>

#include <iostream>
#include <sstream>
#include <exception>
#include <glm/glm.hpp>

Object * EditorWindow::m_object = nullptr;
Object * EditorWindow::m_object_scripts = nullptr;
Light * EditorWindow::m_light = nullptr;
ScriptElement * EditorWindow::m_script_element = nullptr;
std::vector<Object *> EditorWindow::m_objects;
std::vector<Light *> EditorWindow::m_lights;
std::vector<ScriptElement*> EditorWindow::m_script_elements;
bool EditorWindow::m_refreshing;

 Ui::EditorWindow * EditorWindow::ui;

class CustomOpenGLWindow : public OpenGLWindow
{
public:
    CustomOpenGLWindow(QWindow *);

    void initialize() override;
    void render() override;
    bool running();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void wheelEvent ( QWheelEvent * event );



private:
    Engine * m_engine = nullptr;
    int t =0;

    QPoint last_mouse_pos = QPoint(0,0);
    bool mouse_pressed = false;


};

CustomOpenGLWindow::CustomOpenGLWindow(QWindow * parent =0) :OpenGLWindow(parent)
{
}


EditorWindow::EditorWindow(QWidget *parent) :
    QDialog(parent)

{
    ui = new Ui::EditorWindow;
    ui->setupUi(this);

    m_window = new CustomOpenGLWindow(this->windowHandle());
    QWidget * widget =QWidget::createWindowContainer( (QWindow *)m_window, this);

  //  m_window->setFormat();
  //  m_window->resize(640, 480);
  //  m_window->show();
   // widget->resize(800, 600);
      //widget->setFocusPolicy(Qt::TabFocus);
   //   m_window->setParent(  this->windowHandle());
     widget->setParent( ui->verticalLayout->widget());

        ui->verticalLayout->addWidget(widget);
   //   widget->show();
          timerId = startTimer(1);


}
void EditorWindow::update()
{
}
bool EditorWindow::running()
{
    return m_window->running() ;
}

EditorWindow::~EditorWindow()
{
    killTimer(timerId);
    delete ui;
}
void EditorWindow::timerEvent(QTimerEvent *event)
{
  m_window->requestUpdate();
}

void CustomOpenGLWindow::initialize()
{
    m_engine = new Engine();
    if(! m_engine->initialize(900, 680, RENDER_SUBSYSTEM | PHYSIC_SUBSYSTEM , EM_QT_EDITOR, false) )
        std::cout << "enigne start failed" <<std::endl;


    m_engine->setShadowTechniqueDirectional(ST_STANDARD );
    m_engine->setShadowTechniquePoint(ST_STANDARD);

    m_engine->setPostProcessing(PP_FXAA, true);
    m_engine->setPostProcessing(PP_HDR, true);
    m_engine->setPostProcessing(PP_BLOOM, true);

    m_engine->setLoadedFunction(&EditorWindow::sceneLoadedCallback);

    Scene * scene = new Scene();
    scene->load("demo2.xml");

    m_engine->setScene(scene, false);

    EditorWindow::refreshObjectList();
    EditorWindow::refreshLightList();

}

void CustomOpenGLWindow::mouseMoveEvent(QMouseEvent *event)
{
   if(last_mouse_pos.x() !=0.0f && last_mouse_pos.y() != 0.0f)
   {
       if(mouse_pressed)
       {
           QPoint delta = last_mouse_pos - event->pos();
           if(m_engine != nullptr)
                m_engine->getScene()->getCamera()->rotate(delta.x(),delta.y());
       }
   }

   last_mouse_pos = event->pos();
}

void CustomOpenGLWindow::mousePressEvent(QMouseEvent *event)
{
   if(event->button()  == Qt::MouseButton::RightButton )
        mouse_pressed = true;
    if(event->button()  == Qt::MouseButton::LeftButton )
    {
       glm::vec2 pos(last_mouse_pos.x(),last_mouse_pos.y()  );
       Object * obj = Engine::pickObjectAt(pos);
       if(obj != nullptr)
          EditorWindow::changeObject(obj);
    }
}
void CustomOpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
{
   if(event->button()  == Qt::MouseButton::RightButton )
     mouse_pressed = false;
}

void CustomOpenGLWindow::wheelEvent ( QWheelEvent * event )
{
  if( event->delta() != 0 )
  {
      m_engine->getScene()->getCamera()->move(m_engine->getScene()->getCamera()->getDirection() * ( 0.01f * event->delta() ));
      m_engine->getScene()->cameraMoved();
  }

}

void CustomOpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    if(m_engine != nullptr)
    {
        if(event->key() == Qt::Key_W)
        {
           m_engine->getScene()->getCamera()->move(m_engine->getScene()->getCamera()->getDirection() *0.2f);
           m_engine->getScene()->cameraMoved();
        }
        if(event->key() == Qt::Key_S)
        {
           m_engine->getScene()->getCamera()->move(m_engine->getScene()->getCamera()->getDirection() *-0.2f);
           m_engine->getScene()->cameraMoved();
        }
        if(event->key() == Qt::Key_A)
        {
           m_engine->getScene()->getCamera()->move(m_engine->getScene()->getCamera()->getRight() *-0.2f);
           m_engine->getScene()->cameraMoved();
        }
        if(event->key() == Qt::Key_D)
        {
           m_engine->getScene()->getCamera()->move(m_engine->getScene()->getCamera()->getRight() *0.2f);
           m_engine->getScene()->cameraMoved();
        }
    }
}
void CustomOpenGLWindow::keyReleaseEvent(QKeyEvent *event)
{

}
void CustomOpenGLWindow::render()
{
    if(m_engine != nullptr)
    {
        m_engine->update();
        m_engine->render();

        qApp->processEvents();
    }
}
bool CustomOpenGLWindow::running()
{
    return m_engine->running();
}


// scene ---------------------------------------------------------------------------------------------------------
void EditorWindow::on_pbLoadScene_clicked(bool checked)
{
    try
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

            Scene * scene = new Scene();

            if (scene->load(str3) )
            {
              std::cout << "setting scene" << std::endl;
              Engine::setScene(scene, true);
              scene->getCamera()->dontApplyPhysicsDrop();
              scene->getCamera()->setRotationAllowed();
              refreshObjectList();
              refreshLightList();
            }
          }
        }
    }
    catch(std::exception ex)
    {
        std::cout << ex.what() <<std::endl;
    }
}

void EditorWindow::on_pbSaveScene_clicked(bool checked)
{
    try
    {
        std::string file_name = QFileDialog::getSaveFileName(this,
          tr("Save Scene "), QDir::currentPath(), tr("XML Files (*.xml)")).toUtf8();

        if (file_name != "")
        {
          Scene * scene = Engine::getScene();

          if (scene != nullptr)
          {
            scene->save(file_name);
          }
        }
    }
        catch(std::exception ex)
    {
        std::cout << ex.what() <<std::endl;
    }
}

// refresher ---------------------------------------------------------------------------------

void EditorWindow::refreshObjectList()
{
  std::cout << "ref obj list" << std::endl;

 // ui->lw_sc_objects->reset();
  ui->lw_objects_active->reset();
//  ui.lw_sc_objects->clear();
  ui->lw_objects_active->clear();
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
        ui->lw_objects_active->insertItem(ui->lw_objects_active->count(), QString::fromUtf8((*it)->getIdentifier().c_str()));
        //ui->lw_sc_objects->insertItem(ui->lw_sc_objects->count(), QString::fromUtf8((*it)->getIdentifier().c_str()));
      }
    }
  //  scene->releaseLock("refreshObjectList");
  }
  std::cout << "ref obj list end" << std::endl;
}

void EditorWindow::refreshLightList()
{

  ui->lw_lights_active->reset();
  while (ui->lw_lights_active->count()>0)
    ui->lw_lights_active->takeItem(0);

  Scene * scene = Engine::getScene();
  if (scene != nullptr)
  {
    m_lights = scene->getLights();

    for (std::vector<Light *>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
    {
   //   std::cout << "rll" << ui.lw_lights_active->count() << std::endl;
      ui->lw_lights_active->insertItem(ui->lw_lights_active->count(), QString::fromUtf8( (*it)->getDescription().c_str() ));
    }
  }
  std::cout << "rll" << ui->lw_lights_active->count() << std::endl;

}

void EditorWindow::refreshObjectDatabase()
{
  while (ui->lw_objects_chooser->count()>0)
    ui->lw_objects_chooser->takeItem(0);

  if(m_object_database  != nullptr)
  {
    std::vector<ObjectDatabaseEntry> entries = m_object_database->getEntries();
    for (std::vector<ObjectDatabaseEntry>::iterator it = entries.begin(); it != entries.end(); it++)
    {
      ui->lw_objects_chooser->insertItem(ui->lw_objects_chooser->count(), QString::fromUtf8((*it).m_name.c_str()));
    }
  }
}


void EditorWindow::refreshSelected()
{
  if (m_object != nullptr)
  {
    ui->lbl_object_txt->setText(QString::fromUtf8(m_object->getIdentifier().c_str() ) );

    glm::vec3 pos = m_object->getPosition();
    std::stringstream ss;
    ss << pos.x;
    ui->le_obj_pos_x->setText( QString::fromUtf8(ss.str( ).c_str() ) );
    ss.str(""); ss.clear();
    ss << pos.y;
    ui->le_obj_pos_y->setText (QString::fromUtf8(ss.str().c_str() ));
    ss.str(""); ss.clear();
    ss << pos.z;
    ui->le_obj_pos_z->setText(QString::fromUtf8(ss.str().c_str() ));
    ss.str(""); ss.clear();

    glm::vec3 rot = m_object->getRotationDegrees();
    ss << rot.x;
    ui->le_obj_rot_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.y;
    ui->le_obj_rot_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.z;
    ui->le_obj_rot_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();



    ui->rb_obj_fixed->setChecked(m_object->isStaticFlagSet());
    ui->rb_obj_apply_physic->setChecked(m_object->isPhysicsApplied() );
    ui->rb_obj_physic_static->setChecked(m_object->isPhysicsStatic());
    ui->rb_obj_visible->setChecked(m_object->getVisibility() == V_All );
    ui->rb_obj_shadow_only->setChecked(m_object->getVisibility() == V_Shadow);
  }
  if (m_light != nullptr)
  {
    glm::vec3 pos = m_light->getPosition();
    std::stringstream ss;
    ss << pos.x;
    ui->le_light_pos_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << pos.y;
    ui->le_light_pos_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << pos.z;
    ui->le_light_pos_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();

    glm::vec3 rot = m_light->getDirection();
    ss << rot.x;
    ui->le_light_rot_x->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.y;
    ui->le_light_rot_y->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();
    ss << rot.z;
    ui->le_light_rot_z->setText(QString::fromUtf8(ss.str().c_str()));
    ss.str(""); ss.clear();

    glm::vec3 rgb = m_light->getAmbientColor()*255.0f *100.0f;
    ui->sb_light_r->setValue(rgb.x );
    ui->sb_light_g->setValue(rgb.y);
    ui->sb_light_b->setValue(rgb.z);

    ui->lbl_light_txt->setText(QString::fromUtf8( m_light->getDescription().c_str()));

    ui->sb_light_intensity->setValue(m_light->getIntensity() );
  }
}


// object db -----------------------------------------------------------------------------------

void EditorWindow::on_lw_objects_chooser_currentRowChanged(int row)
{
  if( m_object_database != nullptr )
  {
    int cr = ui->lw_objects_chooser->currentRow();
    if ( (cr< m_object_database->getEntries().size() ) && (cr >= 0))
    {
      ObjectDatabaseEntry entry = m_object_database->getEntries()[ui->lw_objects_chooser->currentRow()];
      ui->lbl_objectdb_txt->setText(QString::fromUtf8(entry.m_name.c_str()) );
      ui->tb_objectdb_description->setText(QString::fromUtf8(entry.m_description.c_str()));

      QPixmap pix(entry.m_icon_path.c_str());
      int w = ui->lbl_objectdb_selected->width();
      int h = ui->lbl_objectdb_selected->height();
      ui->lbl_objectdb_selected->setPixmap(pix.scaled(w, h, Qt::IgnoreAspectRatio));

    }
  }
}

void EditorWindow::on_pb_objects_add_clicked()
{
  Scene * scene = Engine::getScene();
  if ( m_object_database != nullptr && (scene != nullptr) )
  {
    ObjectDatabaseEntry entry = m_object_database->getEntries()[ui->lw_objects_chooser->currentRow()];

    if (scene != nullptr )
    {
      Object * obj = m_scene->addObject( entry.m_path, glm::vec3(0, 0, 0), ui->rb_obj_fixed->isChecked() );

       if ( ui->rb_obj_shadow_only->isChecked())
         obj->setVisibility(Visibility::V_Shadow);
       if(!ui->rb_obj_visible->isChecked())
         obj->setVisibility(Visibility::V_None);


       if (ui->rb_obj_apply_physic->isChecked())
         obj->applyPhysics();
       if (ui->rb_obj_physic_static->isChecked() )
         obj->applyPhysicsStatic();


       changeObject(obj);
    }
    refreshSelected();
    refreshObjectList();
  }
}

void EditorWindow::on_pb_object_remove_clicked()
{
  Scene * scene = Engine::getScene();
  if (m_object != nullptr && (scene != nullptr))
  {
    scene->removeObject(m_object);
    m_object = nullptr;
    refreshSelected();
    refreshObjectList();
  }
  else
  {
    Engine::getLog()->log("PastadEditor", "couldn't remove object");
  }
}

// object changer -----------------------------------------------------------------------------------

void EditorWindow::changeObject(Object * obj)
{
    std::cout << "change obj" <<std::endl;
  m_object =obj;
  m_object_scripts = obj;
  refreshSelected();
  refreshObjectList();
}

void EditorWindow::on_lw_objects_active_currentRowChanged(int row)
{
    std::cout << "orc" << row << std::endl;
    Scene * scene = Engine::getScene();
    if (scene != nullptr && (m_objects.size() >0))
    {
      int cr = ui->lw_objects_active->currentRow();
      if (cr < m_objects.size() && (cr >= 0))
      {
        m_object = m_objects[ui->lw_objects_active->currentRow()];
        refreshSelected();
      }
    }
}

void EditorWindow::on_rb_obj_fixed_clicked(bool value)
{
  if( value )
    Engine::getLog()->log(LF_Editor, "PastadEditor", "pressed");
  else
    Engine::getLog()->log(LF_Editor, "PastadEditor", "not pressed");
}

void EditorWindow::on_le_obj_pos_x_editingFinished()
{  savePos();}
void EditorWindow::on_le_obj_pos_y_editingFinished()
{  savePos();}
void EditorWindow::on_le_obj_pos_z_editingFinished()
{  savePos();}
void EditorWindow::on_le_obj_rot_x_editingFinished()
{  saveRot();}
void EditorWindow::on_le_obj_rot_y_editingFinished()
{  saveRot();}
void EditorWindow::on_le_obj_rot_z_editingFinished()
{  saveRot();}
void EditorWindow::on_rb_obj_apply_physic_clicked(bool value)
{  saveObjectSpecs(); }
void EditorWindow::on_rb_obj_physic_static_clicked(bool value)
{  saveObjectSpecs();}
void EditorWindow::on_rb_obj_visible_clicked(bool value)
{  saveObjectSpecs();}
void EditorWindow::on_rb_obj_shadow_only_clicked(bool value)
{  saveObjectSpecs();}

// saver -----------------------------------------------------------------------------------

void EditorWindow::savePos()
{
  if (m_object != nullptr)
  {
    try
    {
      glm::vec3 p;
      p.x = std::stof(ui->le_obj_pos_x->text().toStdString() );
      p.y = std::stof(ui->le_obj_pos_y->text().toStdString() );
      p.z = std::stof(ui->le_obj_pos_z->text().toStdString() );
      m_object->setPosition(p);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save position");
    }
    refreshSelected();
  }
}

void EditorWindow::saveRot()
{
  if (m_object != nullptr)
  {
    try
    {
      glm::vec3 r;
      r.x = std::stof(ui->le_obj_rot_x->text().toStdString());
      r.y = std::stof(ui->le_obj_rot_y->text().toStdString());
      r.z = std::stof(ui->le_obj_rot_z->text().toStdString());

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


void EditorWindow::saveObjectSpecs()
{
  if (m_object != nullptr)
  {
    bool vis = ui->rb_obj_visible->isChecked();
    bool sro = ui->rb_obj_shadow_only->isChecked();

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

    bool ap = ui->rb_obj_apply_physic->isChecked();
    bool aps = ui->rb_obj_physic_static->isChecked();
    bool fixed = ui->rb_obj_fixed->isChecked();

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

// options -----------------------------------------------------------------------------------
void EditorWindow::on_cb_anti_aliasing_currentIndexChanged(int idx)
{
  Engine::setPostProcessing(PostprocessType::PP_FXAA, idx != 0  );
}

void EditorWindow::on_rb_hdr_clicked(bool value)
{
  Engine::setPostProcessing(PostprocessType::PP_HDR, value );
}
void EditorWindow::on_rb_bloom_clicked(bool value)
{
  Engine::setPostProcessing(PostprocessType::PP_BLOOM, value );
}
void EditorWindow::on_rb_ssao_clicked(bool value)
{
  Engine::setShadowTechniqueSSAO(value ?  ST_SSAO : ST_NONE);
}

void EditorWindow::on_cb_shadow_type_currentIndexChanged(int idx)
{
  Engine::setShadowTechniqueDirectional((ShadowTechniqueType)idx);
}
void EditorWindow::on_cb_shadow_type_point_currentIndexChanged(int idx)
{
  Engine::setShadowTechniquePoint((ShadowTechniqueType)idx);
}



void EditorWindow::on_pbDBLoad_clicked(bool checked)
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

void EditorWindow::on_pbDBNew_clicked(bool checked)
{
    std::cout << "new object database" << std::endl;
     if(m_object_database != nullptr)
       delete m_object_database;


     std::string file_name = QFileDialog::getSaveFileName(this,
       tr("New ObjectDatabase "), QDir::currentPath(), tr("XML Files (*.xml)")).toUtf8();
std::cout <<file_name<<std::endl;
     if( file_name != "")
     {
       m_object_database = new ObjectDatabase(file_name);
       refreshObjectDatabase();
     }
}

void EditorWindow::on_pbEdit_clicked(bool checked)
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

}

//lights


void EditorWindow::on_sb_light_r_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.x = ((float) ui->sb_light_r->value() )/ 255.0f;
    m_light->setColor(r);
  }
}
void EditorWindow::on_sb_light_g_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.y = ui->sb_light_g->value() / 255.0f;
    m_light->setColor(r);
  }
}
void EditorWindow::on_sb_light_b_valueChanged(int i)
{
  if (m_light != nullptr)
  {
    glm::vec3 r = m_light->getAmbientColor();
    r.z = ui->sb_light_b->value() / 255.0f;
    m_light->setColor(r);
  }
}

void EditorWindow::on_le_light_pos_x_editingFinished()
{
  saveLightPos();
}
void EditorWindow::on_le_light_pos_y_editingFinished()
{
  saveLightPos();
}
void EditorWindow::on_le_light_pos_z_editingFinished()
{
  saveLightPos();
}
void EditorWindow::on_le_light_rot_x_editingFinished()
{
  saveLightRot();
}
void EditorWindow::on_le_light_rot_y_editingFinished()
{
  saveLightRot();
}
void EditorWindow::on_le_light_rot_z_editingFinished()
{
  saveLightRot();
}

void EditorWindow::on_sb_light_intensity_valueChanged(double d)
{
    if (m_light != nullptr)
    {
        // std::cout << ui.sb_light_intensity->value() << std::endl;
        m_light->setIntensity(ui->sb_light_intensity->value());
    }
}

void EditorWindow::on_pb_light_remove_clicked()
{
    Scene * scene = Engine::getScene();
    if (m_light != nullptr && (scene != nullptr))
    {
      scene->removeLight(m_light);
      m_light = nullptr;

      refreshSelected();
    }
    refreshLightList();
}
void EditorWindow::saveLightRot()
{
  if (m_light != nullptr)
  {
    try
    {
      glm::vec3 r;
      r.x = std::stof(ui->le_light_rot_x->text().toStdString());
      r.y = std::stof(ui->le_light_rot_y->text().toStdString());
      r.z = std::stof(ui->le_light_rot_z->text().toStdString());

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
void EditorWindow::saveLightPos()
{
  if (m_light != nullptr)
  {
    try
    {
      glm::vec3 p;
      p.x = std::stof(ui->le_light_pos_x->text().toStdString());
      p.y = std::stof(ui->le_light_pos_y->text().toStdString());
      p.z = std::stof(ui->le_light_pos_z->text().toStdString());
      m_light->setPosition(p);
    }
    catch (const std::exception&)
    {
      Engine::getLog()->log(LF_Editor, "PastadEditor", "cannot save position");
    }
    refreshSelected();
  }
}
void EditorWindow::on_pb_lights_add_clicked()
{
  std::string sel = ui->cb_lights_type->currentText().toStdString();

  unsigned int type = 0;
  Scene * scene = Engine::getScene();
  if(scene != nullptr)
  {
      if (sel == "directional")
      {
          Light * new_light = scene->addLight();
          if (!new_light->setDirectional(glm::vec3(0, 1, 0), glm::vec3(1, 0.95, 0.9), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 0.3f, ui->rb_lights_cast_shadow->isChecked()))
            scene->removeLight(new_light);
          else
            changeLight(new_light);
         refreshLightList();
      }
      if (sel == "spot")
      {
          Light * new_light = scene->addLight();
          if (!new_light->setSpot(scene->getCamera()->getPosition(), glm::vec3(1, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1, 1, 1), 1.0f, 1.0f, 0.09f, 0.032f, 45.0f, glm::vec2(0, 0),ui->rb_lights_cast_shadow->isChecked()))
            scene->removeLight(new_light);
          else
            changeLight(new_light);
          refreshLightList();
      }
      if (sel == "point")
      {
          Light * new_light = scene->addLight();
          if (!new_light->setPoint(scene->getCamera()->getPosition(), glm::vec3(1, 1, 1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1, 1, 1), 0.5f, 0.1f, 0.09f, 0.032f, ui->rb_lights_cast_shadow->isChecked()))
            scene->removeLight(new_light);
          else
            changeLight(new_light);
          refreshLightList();
      }
  }
}
void EditorWindow::on_lw_lights_active_currentRowChanged(int row)
{

    std::cout <<"lrc" <<  ui->lw_lights_active->currentRow() << ", active "<< ui->lw_lights_active->count()<< std::endl;
    Scene * scene = Engine::getScene();
    if (scene != nullptr  && (m_lights.size() >0) )
    {
      int cr  = ui->lw_lights_active->currentRow();
      if (cr < ui->lw_lights_active->count()   && (cr >=0)  )
      {
        m_light = m_lights[ui->lw_lights_active->currentRow()];
        refreshSelected();
      }
    }
    std::cout << "done" <<std::endl;

}
void EditorWindow::changeLight(Light * light)
{
  m_light = light;
  refreshSelected();
}

void EditorWindow::sceneLoadedCallback()
{
   refreshLightList();
   refreshObjectList();
}
