#ifndef EDITOR_H
#define EDITOR_H

// CODE GENERATED BY QT Creator

#include <QtWidgets/QMainWindow>

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class Scene;
class Object;
class Light;



class Editor : public QMainWindow
{

public:
  //Editor();

  explicit Editor(QWidget *parent = 0);
  ~Editor();

  // sets the scene for the editor
  void setScene(Scene * scene);

  // object/light selected | triggered by engine
  void engineObjectSelected(Object * object);
  void engineLightSelected(Light * light);

  public slots:
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

  void on_pb_scene_play_clicked(bool checked);

  void on_pb_scene_pause_clicked();

  // refreshes current selected object/light
  void refreshObject();
  void refreshLight();


  void setupUi(QMainWindow *Editor);
  void retranslateUi(QMainWindow *Editor);

  QAction *actionLoad;
  QAction *actionSave;
  QWidget *centralWidget;
  QTabWidget *tabWidget;
  QWidget *tab;
  QGroupBox *gb_obj;
  QLabel *lbl_object_txt;
  QLabel *label;
  QLineEdit *le_obj_pos_x;
  QLabel *label_2;
  QLineEdit *le_obj_pos_y;
  QLabel *label_3;
  QLineEdit *le_obj_pos_z;
  QLabel *label_4;
  QLineEdit *le_obj_rot_x;
  QLabel *label_5;
  QLineEdit *le_obj_rot_z;
  QLineEdit *le_obj_rot_y;
  QLabel *label_6;
  QLabel *label_7;
  QLabel *label_8;
  QRadioButton *rb_obj_apply_physic;
  QRadioButton *rb_obj_physic_static;
  QRadioButton *rb_obj_fixed;
  QRadioButton *rb_obj_visible;
  QRadioButton *rb_object_shadow_only;
  QPushButton *pb_object_remove;
  QGroupBox *groupBox;
  QListView *lv_objects_chooser;
  QPushButton *pb_objects_add;
  QFrame *f_objects_selected;
  QLabel *lbl_objectdb_txt;
  QTextBrowser *tb_ojectdb_description;
  QGroupBox *groupBox_2;
  QListWidget *lw_objects_active;
  QWidget *tab_2;
  QGroupBox *gb_light;
  QLabel *lbl_light_txt;
  QLabel *label_9;
  QLineEdit *le_light_pos_x;
  QLabel *label_10;
  QLineEdit *le_light_pos_y;
  QLabel *label_11;
  QLineEdit *le_light_pos_z;
  QLabel *label_12;
  QLineEdit *le_light_rot_x;
  QLabel *label_13;
  QLineEdit *le_light_rot_z;
  QLineEdit *le_light_rot_y;
  QLabel *label_14;
  QLabel *label_15;
  QLabel *label_16;
  QSpinBox *sb_light_r;
  QSpinBox *sb_light_g;
  QSpinBox *sb_light_b;
  QLabel *label_17;
  QLabel *label_18;
  QLabel *label_19;
  QDoubleSpinBox *doubleSpinBox;
  QLabel *label_20;
  QLabel *label_21;
  QListWidget *lw_lights_active;
  QGroupBox *groupBox_3;
  QComboBox *cb_lights_type;
  QPushButton *pb_lights_add;
  QRadioButton *rb_lights_cast_shadow;
  QPushButton *pb_scene_play;
  QPushButton *pb_scene_pause;
  QMenuBar *menuBar;
  QMenu *menuSceneEditor;
  QToolBar *mainToolBar;
  QStatusBar *statusBar;

private:
  // the scene to edit
  static Scene * m_scene ;

  // the current selected object/light
  static Object * m_object ;
  static Light * m_light ;



  // callback
  static void mouseButtonCallback(int, bool);


};

#endif