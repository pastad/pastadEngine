#ifndef PASTADEDITOR_H
#define PASTADEDITOR_H

#include <QtWidgets/QMainWindow>
#include "Engine.h"
#include "ui_PastadEditor.h"


class Object;
class Light;
class ObjectDatabase;
class Scene;
class Script;
class ScriptElement;

class PastadEditor : public QMainWindow
{
    Q_OBJECT

public:
    PastadEditor(QWidget *parent = 0);
    ~PastadEditor();

    static void changeObject(Object * obj);
    static void changeLight(Light * light);
    static void mouseClicked(int btn, bool val);
    static void keyPressed(int key, bool b);

    void setScene(Scene * scene);

    void refreshAll();

    void loadDatatbase(std::string path);

public slots:

    //scene menu
    void on_actionLoadScene_triggered();
    void on_actionSaveScene_triggered();

    //object database menu
    void on_actionNewObjectDatabase_triggered();
    void on_actionLoadObjectDatabase_triggered();
    void on_actionEditObjectDatabase_triggered();
    
    // buttons
    void on_pb_scene_play_clicked();
    void on_pb_scene_pause_clicked();
    void on_pb_scene_reset_clicked();
    void on_pb_refresh_shaders_clicked();

    //setter
    void setTime(std::string time);


    // objects slide -------------------------------------------
    
    //saves
    void savePos();
    void saveRot();
    void saveObjectSpecs();

    //refreshs
    static void refreshSelected();
    static void refreshObjectList();
    void refreshObjectDatabase();

    //buttons
    void on_pb_objects_add_clicked();
    void on_pb_object_remove_clicked();
    
    //list widgets
    void on_lw_objects_active_currentRowChanged(int row);
    void on_lw_objects_chooser_currentRowChanged(int row);

    //line edits
    void on_le_obj_pos_x_editingFinished();
    void on_le_obj_pos_y_editingFinished();
    void on_le_obj_pos_z_editingFinished();
    void on_le_obj_rot_x_editingFinished();
    void on_le_obj_rot_y_editingFinished();
    void on_le_obj_rot_z_editingFinished();

    //radio buttons
    void on_rb_obj_fixed_clicked(bool value);
    void on_rb_obj_apply_physic_clicked(bool value);
    void on_rb_obj_physic_static_clicked(bool value);
    void on_rb_obj_visible_clicked(bool value);
    void on_rb_obj_shadow_only_clicked(bool value);

   
    // lights slide -------------------------------------------
    
    //saves
    void saveLightPos();
    void saveLightRot();

    //refreshs
    void refreshLightList();

    //buttons
    void on_pb_lights_add_clicked();
    void on_pb_light_remove_clicked();

    //list widgets
    void on_lw_lights_active_currentRowChanged(int row);

    //line edits
    void on_le_light_pos_x_editingFinished();
    void on_le_light_pos_y_editingFinished();
    void on_le_light_pos_z_editingFinished();
    void on_le_light_rot_x_editingFinished();
    void on_le_light_rot_y_editingFinished();
    void on_le_light_rot_z_editingFinished();

    // spin box
    void on_sb_light_r_valueChanged(int i);
    void on_sb_light_g_valueChanged(int i);
    void on_sb_light_b_valueChanged(int i);
    void on_sb_light_intensity_valueChanged(double d);


    
    // scripts slide  -------------------------------------------
    
    //refreshs
    void refreshScripts();
    void refreshScriptElement();

    //buttons
    void on_pb_add_script_clicked();
    void on_pb_remove_script_clicked();
    void on_pb_add_successor_clicked();

    //list widgets
    void on_lw_sc_objects_currentRowChanged(int row);
    void on_lw_sc_script_elements_currentRowChanged(int row);

    //line edits
    void on_le_sc_val1_editingFinished();
    void on_le_sc_val2_editingFinished();
    void on_le_sc_val3_editingFinished();

    void on_le_sc_from_editingFinished();
    void on_le_sc_to_editingFinished();
    void on_le_sc_duration_editingFinished();

    //savers
    void saveScriptElement();

    //combo box
    void on_cb_sc_trigger_currentIndexChanged(int idx);


    // engine options slide -------------------------------------------
       
    //combo boxes
    void on_cb_shadow_type_currentIndexChanged(int idx);
    void on_cb_shadow_type_point_currentIndexChanged(int idx);
    void on_cb_anti_aliasing_currentIndexChanged(int idx);

    //radio buttons
    void on_rb_hdr_clicked(bool value);
    void on_rb_bloom_clicked(bool value);
    void on_rb_ssao_clicked(bool value);



private:

    // the ui class 
    static Ui::PastadEditorClass ui;

    // the scene/object/light that is currently edited
    Scene * m_scene;
    static Object * m_object;
    static Object * m_object_scripts;
    static Light * m_light;
    static ScriptElement * m_script_element;

    // the objects/lights currently in the scene (for list selection)
    static std::vector<Object *> m_objects;
    static std::vector<Light *> m_lights;
    static std::vector<ScriptElement*> m_script_elements;

    // true if currently refreshing
    static bool m_refreshing;

    // the chosen database for objects
    ObjectDatabase * m_object_database = nullptr;
};

#endif // PASTADEDITOR_H
