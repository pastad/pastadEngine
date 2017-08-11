#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QDialog>
#include <QTimer>

class CustomOpenGLWindow;

class Object;
class Light;
class ObjectDatabase;
class Scene;
class Script;
class ScriptElement;

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();
    void update();
    bool running();

    static void changeObject(Object * obj);
    static void refreshObjectList();
    static void refreshLightList();

    static void sceneLoadedCallback();


private:
    static Ui::EditorWindow *ui;
    CustomOpenGLWindow *  m_window;

    int timerId;

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

    static void refreshSelected();




protected:
    void timerEvent(QTimerEvent *event);
private slots:


    void on_pbLoadScene_clicked(bool checked);
    void on_pbSaveScene_clicked(bool checked);

    void on_pb_scene_play_clicked();
    void on_pb_scene_pause_clicked();
    void on_pb_scene_reset_clicked();

    // objects slide -------------------------------------------

    //saves
    void savePos();
    void saveRot();
    void saveObjectSpecs();

    //refreshs
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

    // engine options slide -------------------------------------------

    //combo boxes
    void on_cb_shadow_type_currentIndexChanged(int idx);
    void on_cb_shadow_type_point_currentIndexChanged(int idx);
    void on_cb_anti_aliasing_currentIndexChanged(int idx);

    //radio buttons
    void on_rb_hdr_clicked(bool value);
    void on_rb_bloom_clicked(bool value);
    void on_rb_ssao_clicked(bool value);
    void on_pbDBLoad_clicked(bool checked);
    void on_pbDBNew_clicked(bool checked);
    void on_pbEdit_clicked(bool checked);



    // lights slide -------------------------------------------

    //saves
    void saveLightPos();
    void saveLightRot();



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
    void changeLight(Light * light);


    // scripts slide  -------------------------------------------

    //refreshs
    static void refreshScripts();
    void refreshScriptElement();

    //buttons
    void on_pb_add_script_clicked();
    void on_pb_remove_script_clicked();
    void on_pb_add_successor_clicked();

    //list widgets
   // void on_lw_sc_objects_currentRowChanged(int row);
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

};

#endif // EDITORWINDOW_H
