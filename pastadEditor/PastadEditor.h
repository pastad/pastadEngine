#ifndef PASTADEDITOR_H
#define PASTADEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_PastadEditor.h"

#include "Scene.h"
#include "Object.h"


class PastadEditor : public QMainWindow
{
    Q_OBJECT

public:
    PastadEditor(QWidget *parent = 0);
    ~PastadEditor();

    static void changeObject(Object * obj);
    static void mouseClicked(int btn, bool val);

public slots:
    void on_actionLoadScene_triggered();

    static void refreshSelected();
    void refreshObjectList();
    void savePos();
    void saveRot();
    void saveObjectSpecs();

    void on_pb_objects_add_clicked();

    void on_lw_objects_active_currentRowChanged(int row);

    void on_le_obj_pos_x_editingFinished();
    void on_le_obj_pos_y_editingFinished();
    void on_le_obj_pos_z_editingFinished();
    void on_le_obj_rot_x_editingFinished();
    void on_le_obj_rot_y_editingFinished();
    void on_le_obj_rot_z_editingFinished();
    void on_rb_obj_fixed_clicked(bool value);
    void on_rb_obj_apply_physic_clicked(bool value);
    void on_rb_obj_physic_static_clicked(bool value);
    void on_rb_obj_visible_clicked(bool value);
    void on_rb_obj_shadow_only_clicked(bool value);

    void on_pb_lights_add_clicked();

    void on_pb_scene_play_clicked();
    void on_pb_scene_pause_clicked();

private:
    static Ui::PastadEditorClass ui;

    // the scene/object that is currently edited
    Scene * m_scene;
    static Object * m_object;
    static std::vector<Object *> m_objects;
};

#endif // PASTADEDITOR_H
