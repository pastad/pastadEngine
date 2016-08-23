#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

namespace Ui {
class Editor;
}

class Editor : public QMainWindow
{
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = 0);
    ~Editor();

private slots:
    void on_pushButton_clicked();

    void on_le_obj_pos_x_editingFinished();

    void on_le_obj_pos_y_editingFinished();

    void on_le_obj_pos_z_editingFinished();

    void on_le_obj_rot_x_editingFinished();

    void on_le_obj_rot_y_editingFinished();

    void on_le_obj_rot_z_editingFinished();

    void on_rb_obj_fixed_clicked(bool checked);

    void on_rb_obj_apply_physic_clicked(bool checked);

    void on_rb_obj_physic_static_clicked(bool checked);

    void on_pb_objects_add_clicked(bool checked);

    void on_rb_obj_visible_clicked(bool checked);

    void on_rb_object_shadow_only_clicked(bool checked);

    void on_lv_objects_chooser_indexesMoved(const QModelIndexList &indexes);

private:
    Ui::Editor *ui;
};

#endif // EDITOR_H
