#include "editor.h"
#include "ui_editor.h"

Editor::Editor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Editor)
{
   // ui->setFixedSize (this->maximumSize());
    this->setFixedSize(this->maximumSize());
    this->setMinimumSize(this->maximumSize());
    this->setMaximumSize(this->maximumSize());
    ui->setupUi(this);
    // ui->setFixedSize (this->maximumSize());
}

Editor::~Editor()
{
    delete ui;
}

void Editor::on_pushButton_clicked()
{

}

void Editor::on_le_obj_pos_x_editingFinished()
{

}

void Editor::on_le_obj_pos_y_editingFinished()
{
}

void Editor::on_le_obj_pos_z_editingFinished()
{

}

void Editor::on_le_obj_rot_x_editingFinished()
{

}

void Editor::on_le_obj_rot_y_editingFinished()
{

}

void Editor::on_le_obj_rot_z_editingFinished()
{

}

void Editor::on_rb_obj_fixed_clicked(bool checked)
{

}

void Editor::on_rb_obj_apply_physic_clicked(bool checked)
{

}

void Editor::on_rb_obj_physic_static_clicked(bool checked)
{

}

void Editor::on_pb_objects_add_clicked(bool checked)
{

}

void Editor::on_rb_obj_visible_clicked(bool checked)
{

}

void Editor::on_rb_object_shadow_only_clicked(bool checked)
{

}


void Editor::on_pb_scene_play_clicked(bool checked)
{

}

void Editor::on_pb_scene_pause_clicked(bool checked)
{
ui->le_obj_pos_x->setText(QString("Test"));
}
