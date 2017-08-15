/********************************************************************************
** Form generated from reading UI file 'editorwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITORWINDOW_H
#define UI_EDITORWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *pbLoadScene;
    QPushButton *pbSaveScene;
    QTabWidget *tabWidget;
    QWidget *Objects;
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
    QLineEdit *le_obj_rot_z;
    QLineEdit *le_obj_rot_y;
    QLabel *label_8;
    QRadioButton *rb_obj_apply_physic;
    QRadioButton *rb_obj_physic_static;
    QRadioButton *rb_obj_fixed;
    QRadioButton *rb_obj_visible;
    QRadioButton *rb_obj_shadow_only;
    QPushButton *pb_object_remove;
    QLabel *label_32;
    QLineEdit *le_obj_scale_z;
    QLineEdit *le_obj_scale_y;
    QLineEdit *le_obj_scale_x;
    QGroupBox *groupBox_2;
    QListWidget *lw_objects_active;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QLabel *lbl_objectdb_txt;
    QTextBrowser *tb_objectdb_description;
    QPushButton *pb_objects_add;
    QListWidget *lw_objects_chooser;
    QLabel *lbl_objectdb_selected;
    QPushButton *pbDBLoad;
    QPushButton *pbDBNew;
    QPushButton *pbEdit;
    QWidget *tab;
    QGroupBox *groupBox_3;
    QComboBox *cb_lights_type;
    QPushButton *pb_lights_add;
    QRadioButton *rb_lights_cast_shadow;
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
    QDoubleSpinBox *sb_light_intensity;
    QLabel *label_20;
    QLabel *label_21;
    QPushButton *pb_light_remove;
    QListWidget *lw_lights_active;
    QWidget *tab_3;
    QLabel *label_31;
    QRadioButton *rb_ssao;
    QLabel *label_22;
    QComboBox *cb_shadow_type;
    QLabel *label_30;
    QComboBox *cb_shadow_type_point;
    QLabel *label_24;
    QComboBox *cb_anti_aliasing;
    QLabel *label_25;
    QRadioButton *rb_bloom;
    QLabel *label_23;
    QRadioButton *rb_hdr;
    QWidget *tab_4;
    QLineEdit *le_sc_val1;
    QComboBox *cb_sc_type;
    QLabel *label_29;
    QLineEdit *le_sc_from;
    QLabel *label_26;
    QLabel *lbl_sc_x;
    QPushButton *pb_remove_script;
    QLineEdit *le_sc_duration;
    QLabel *lbl_sc_time;
    QLineEdit *le_sc_to;
    QPushButton *pb_add_successor;
    QListWidget *lw_sc_script_elements;
    QLabel *label_27;
    QLabel *lbl_sc_z;
    QLineEdit *le_sc_val3;
    QLineEdit *le_sc_val2;
    QLabel *label_28;
    QPushButton *pb_add_script;
    QComboBox *cb_sc_trigger;
    QLabel *lbl_sc_y;
    QLabel *label_5;
    QPushButton *pb_scene_pause;
    QPushButton *pb_scene_play;
    QPushButton *pb_scene_reset;
    QLabel *lbl_time;
    QPushButton *pbNewScene;

    void setupUi(QDialog *EditorWindow)
    {
        if (EditorWindow->objectName().isEmpty())
            EditorWindow->setObjectName(QStringLiteral("EditorWindow"));
        EditorWindow->setWindowModality(Qt::NonModal);
        EditorWindow->resize(1239, 726);
        EditorWindow->setContextMenuPolicy(Qt::ActionsContextMenu);
        verticalLayoutWidget = new QWidget(EditorWindow);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 30, 901, 681));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        pbLoadScene = new QPushButton(EditorWindow);
        pbLoadScene->setObjectName(QStringLiteral("pbLoadScene"));
        pbLoadScene->setGeometry(QRect(110, 0, 75, 23));
        pbSaveScene = new QPushButton(EditorWindow);
        pbSaveScene->setObjectName(QStringLiteral("pbSaveScene"));
        pbSaveScene->setGeometry(QRect(200, 0, 75, 23));
        tabWidget = new QTabWidget(EditorWindow);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(920, 30, 311, 681));
        Objects = new QWidget();
        Objects->setObjectName(QStringLiteral("Objects"));
        gb_obj = new QGroupBox(Objects);
        gb_obj->setObjectName(QStringLiteral("gb_obj"));
        gb_obj->setGeometry(QRect(10, 10, 281, 271));
        lbl_object_txt = new QLabel(gb_obj);
        lbl_object_txt->setObjectName(QStringLiteral("lbl_object_txt"));
        lbl_object_txt->setGeometry(QRect(40, 20, 201, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        lbl_object_txt->setFont(font);
        label = new QLabel(gb_obj);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 61, 16));
        QFont font1;
        font1.setUnderline(true);
        label->setFont(font1);
        le_obj_pos_x = new QLineEdit(gb_obj);
        le_obj_pos_x->setObjectName(QStringLiteral("le_obj_pos_x"));
        le_obj_pos_x->setGeometry(QRect(20, 60, 71, 21));
        label_2 = new QLabel(gb_obj);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 20, 21));
        le_obj_pos_y = new QLineEdit(gb_obj);
        le_obj_pos_y->setObjectName(QStringLiteral("le_obj_pos_y"));
        le_obj_pos_y->setGeometry(QRect(20, 90, 71, 21));
        label_3 = new QLabel(gb_obj);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 90, 20, 21));
        le_obj_pos_z = new QLineEdit(gb_obj);
        le_obj_pos_z->setObjectName(QStringLiteral("le_obj_pos_z"));
        le_obj_pos_z->setGeometry(QRect(20, 120, 71, 21));
        label_4 = new QLabel(gb_obj);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 120, 20, 21));
        le_obj_rot_x = new QLineEdit(gb_obj);
        le_obj_rot_x->setObjectName(QStringLiteral("le_obj_rot_x"));
        le_obj_rot_x->setGeometry(QRect(100, 60, 71, 21));
        le_obj_rot_z = new QLineEdit(gb_obj);
        le_obj_rot_z->setObjectName(QStringLiteral("le_obj_rot_z"));
        le_obj_rot_z->setGeometry(QRect(100, 120, 71, 21));
        le_obj_rot_y = new QLineEdit(gb_obj);
        le_obj_rot_y->setObjectName(QStringLiteral("le_obj_rot_y"));
        le_obj_rot_y->setGeometry(QRect(100, 90, 71, 21));
        label_8 = new QLabel(gb_obj);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(100, 40, 61, 16));
        label_8->setFont(font1);
        rb_obj_apply_physic = new QRadioButton(gb_obj);
        rb_obj_apply_physic->setObjectName(QStringLiteral("rb_obj_apply_physic"));
        rb_obj_apply_physic->setGeometry(QRect(150, 180, 91, 17));
        rb_obj_apply_physic->setAutoExclusive(false);
        rb_obj_physic_static = new QRadioButton(gb_obj);
        rb_obj_physic_static->setObjectName(QStringLiteral("rb_obj_physic_static"));
        rb_obj_physic_static->setGeometry(QRect(180, 200, 61, 17));
        rb_obj_physic_static->setAutoExclusive(false);
        rb_obj_fixed = new QRadioButton(gb_obj);
        rb_obj_fixed->setObjectName(QStringLiteral("rb_obj_fixed"));
        rb_obj_fixed->setGeometry(QRect(150, 160, 82, 17));
        rb_obj_fixed->setChecked(true);
        rb_obj_fixed->setAutoExclusive(false);
        rb_obj_visible = new QRadioButton(gb_obj);
        rb_obj_visible->setObjectName(QStringLiteral("rb_obj_visible"));
        rb_obj_visible->setGeometry(QRect(150, 220, 82, 17));
        rb_obj_visible->setChecked(true);
        rb_obj_visible->setAutoRepeat(false);
        rb_obj_visible->setAutoExclusive(false);
        rb_obj_shadow_only = new QRadioButton(gb_obj);
        rb_obj_shadow_only->setObjectName(QStringLiteral("rb_obj_shadow_only"));
        rb_obj_shadow_only->setGeometry(QRect(150, 240, 82, 17));
        rb_obj_shadow_only->setAutoExclusive(false);
        pb_object_remove = new QPushButton(gb_obj);
        pb_object_remove->setObjectName(QStringLiteral("pb_object_remove"));
        pb_object_remove->setGeometry(QRect(20, 190, 75, 23));
        label_32 = new QLabel(gb_obj);
        label_32->setObjectName(QStringLiteral("label_32"));
        label_32->setGeometry(QRect(180, 40, 61, 16));
        label_32->setFont(font1);
        le_obj_scale_z = new QLineEdit(gb_obj);
        le_obj_scale_z->setObjectName(QStringLiteral("le_obj_scale_z"));
        le_obj_scale_z->setGeometry(QRect(180, 120, 71, 21));
        le_obj_scale_y = new QLineEdit(gb_obj);
        le_obj_scale_y->setObjectName(QStringLiteral("le_obj_scale_y"));
        le_obj_scale_y->setGeometry(QRect(180, 90, 71, 21));
        le_obj_scale_x = new QLineEdit(gb_obj);
        le_obj_scale_x->setObjectName(QStringLiteral("le_obj_scale_x"));
        le_obj_scale_x->setGeometry(QRect(180, 60, 71, 21));
        groupBox_2 = new QGroupBox(Objects);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 290, 281, 431));
        lw_objects_active = new QListWidget(groupBox_2);
        lw_objects_active->setObjectName(QStringLiteral("lw_objects_active"));
        lw_objects_active->setGeometry(QRect(10, 20, 261, 341));
        tabWidget->addTab(Objects, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 50, 291, 601));
        lbl_objectdb_txt = new QLabel(groupBox);
        lbl_objectdb_txt->setObjectName(QStringLiteral("lbl_objectdb_txt"));
        lbl_objectdb_txt->setGeometry(QRect(30, 20, 131, 16));
        lbl_objectdb_txt->setFont(font);
        tb_objectdb_description = new QTextBrowser(groupBox);
        tb_objectdb_description->setObjectName(QStringLiteral("tb_objectdb_description"));
        tb_objectdb_description->setGeometry(QRect(20, 200, 261, 81));
        pb_objects_add = new QPushButton(groupBox);
        pb_objects_add->setObjectName(QStringLiteral("pb_objects_add"));
        pb_objects_add->setGeometry(QRect(10, 50, 21, 141));
        lw_objects_chooser = new QListWidget(groupBox);
        lw_objects_chooser->setObjectName(QStringLiteral("lw_objects_chooser"));
        lw_objects_chooser->setGeometry(QRect(20, 290, 261, 301));
        lbl_objectdb_selected = new QLabel(groupBox);
        lbl_objectdb_selected->setObjectName(QStringLiteral("lbl_objectdb_selected"));
        lbl_objectdb_selected->setGeometry(QRect(50, 50, 191, 131));
        pbDBLoad = new QPushButton(tab_2);
        pbDBLoad->setObjectName(QStringLiteral("pbDBLoad"));
        pbDBLoad->setGeometry(QRect(10, 10, 75, 23));
        pbDBNew = new QPushButton(tab_2);
        pbDBNew->setObjectName(QStringLiteral("pbDBNew"));
        pbDBNew->setGeometry(QRect(100, 10, 71, 23));
        pbEdit = new QPushButton(tab_2);
        pbEdit->setObjectName(QStringLiteral("pbEdit"));
        pbEdit->setGeometry(QRect(190, 10, 75, 23));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 10, 251, 101));
        cb_lights_type = new QComboBox(groupBox_3);
        cb_lights_type->setObjectName(QStringLiteral("cb_lights_type"));
        cb_lights_type->setGeometry(QRect(10, 30, 81, 22));
        pb_lights_add = new QPushButton(groupBox_3);
        pb_lights_add->setObjectName(QStringLiteral("pb_lights_add"));
        pb_lights_add->setGeometry(QRect(110, 30, 81, 23));
        rb_lights_cast_shadow = new QRadioButton(groupBox_3);
        rb_lights_cast_shadow->setObjectName(QStringLiteral("rb_lights_cast_shadow"));
        rb_lights_cast_shadow->setGeometry(QRect(10, 70, 82, 17));
        gb_light = new QGroupBox(tab);
        gb_light->setObjectName(QStringLiteral("gb_light"));
        gb_light->setGeometry(QRect(10, 120, 261, 261));
        lbl_light_txt = new QLabel(gb_light);
        lbl_light_txt->setObjectName(QStringLiteral("lbl_light_txt"));
        lbl_light_txt->setGeometry(QRect(10, 20, 101, 16));
        lbl_light_txt->setFont(font);
        label_9 = new QLabel(gb_light);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 40, 61, 16));
        label_9->setFont(font1);
        le_light_pos_x = new QLineEdit(gb_light);
        le_light_pos_x->setObjectName(QStringLiteral("le_light_pos_x"));
        le_light_pos_x->setGeometry(QRect(20, 60, 71, 21));
        label_10 = new QLabel(gb_light);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 60, 20, 21));
        le_light_pos_y = new QLineEdit(gb_light);
        le_light_pos_y->setObjectName(QStringLiteral("le_light_pos_y"));
        le_light_pos_y->setGeometry(QRect(20, 90, 71, 21));
        label_11 = new QLabel(gb_light);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 90, 20, 21));
        le_light_pos_z = new QLineEdit(gb_light);
        le_light_pos_z->setObjectName(QStringLiteral("le_light_pos_z"));
        le_light_pos_z->setGeometry(QRect(20, 120, 71, 21));
        label_12 = new QLabel(gb_light);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 120, 20, 21));
        le_light_rot_x = new QLineEdit(gb_light);
        le_light_rot_x->setObjectName(QStringLiteral("le_light_rot_x"));
        le_light_rot_x->setGeometry(QRect(20, 170, 71, 21));
        label_13 = new QLabel(gb_light);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 170, 20, 21));
        le_light_rot_z = new QLineEdit(gb_light);
        le_light_rot_z->setObjectName(QStringLiteral("le_light_rot_z"));
        le_light_rot_z->setGeometry(QRect(20, 230, 71, 21));
        le_light_rot_y = new QLineEdit(gb_light);
        le_light_rot_y->setObjectName(QStringLiteral("le_light_rot_y"));
        le_light_rot_y->setGeometry(QRect(20, 200, 71, 21));
        label_14 = new QLabel(gb_light);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(10, 230, 20, 21));
        label_15 = new QLabel(gb_light);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(10, 200, 20, 21));
        label_16 = new QLabel(gb_light);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(20, 150, 61, 16));
        label_16->setFont(font1);
        sb_light_r = new QSpinBox(gb_light);
        sb_light_r->setObjectName(QStringLiteral("sb_light_r"));
        sb_light_r->setGeometry(QRect(140, 60, 41, 22));
        sb_light_r->setMaximum(255);
        sb_light_r->setValue(0);
        sb_light_r->setDisplayIntegerBase(10);
        sb_light_g = new QSpinBox(gb_light);
        sb_light_g->setObjectName(QStringLiteral("sb_light_g"));
        sb_light_g->setGeometry(QRect(140, 90, 41, 22));
        sb_light_g->setMaximum(255);
        sb_light_b = new QSpinBox(gb_light);
        sb_light_b->setObjectName(QStringLiteral("sb_light_b"));
        sb_light_b->setGeometry(QRect(140, 120, 41, 22));
        sb_light_b->setMaximum(255);
        label_17 = new QLabel(gb_light);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(120, 60, 20, 21));
        label_18 = new QLabel(gb_light);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(120, 90, 20, 21));
        label_19 = new QLabel(gb_light);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(120, 120, 20, 21));
        sb_light_intensity = new QDoubleSpinBox(gb_light);
        sb_light_intensity->setObjectName(QStringLiteral("sb_light_intensity"));
        sb_light_intensity->setGeometry(QRect(140, 170, 51, 22));
        sb_light_intensity->setSingleStep(0.1);
        label_20 = new QLabel(gb_light);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(110, 150, 61, 16));
        label_20->setFont(font1);
        label_21 = new QLabel(gb_light);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(110, 40, 61, 16));
        label_21->setFont(font1);
        pb_light_remove = new QPushButton(gb_light);
        pb_light_remove->setObjectName(QStringLiteral("pb_light_remove"));
        pb_light_remove->setGeometry(QRect(120, 220, 71, 23));
        lw_lights_active = new QListWidget(tab);
        lw_lights_active->setObjectName(QStringLiteral("lw_lights_active"));
        lw_lights_active->setGeometry(QRect(10, 390, 261, 261));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        label_31 = new QLabel(tab_3);
        label_31->setObjectName(QStringLiteral("label_31"));
        label_31->setGeometry(QRect(20, 300, 91, 31));
        rb_ssao = new QRadioButton(tab_3);
        rb_ssao->setObjectName(QStringLiteral("rb_ssao"));
        rb_ssao->setGeometry(QRect(130, 300, 91, 31));
        rb_ssao->setChecked(true);
        rb_ssao->setAutoExclusive(false);
        label_22 = new QLabel(tab_3);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(20, 40, 101, 31));
        cb_shadow_type = new QComboBox(tab_3);
        cb_shadow_type->setObjectName(QStringLiteral("cb_shadow_type"));
        cb_shadow_type->setGeometry(QRect(130, 39, 91, 31));
        label_30 = new QLabel(tab_3);
        label_30->setObjectName(QStringLiteral("label_30"));
        label_30->setGeometry(QRect(20, 80, 91, 31));
        cb_shadow_type_point = new QComboBox(tab_3);
        cb_shadow_type_point->setObjectName(QStringLiteral("cb_shadow_type_point"));
        cb_shadow_type_point->setGeometry(QRect(130, 80, 91, 31));
        label_24 = new QLabel(tab_3);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setGeometry(QRect(20, 220, 91, 31));
        cb_anti_aliasing = new QComboBox(tab_3);
        cb_anti_aliasing->setObjectName(QStringLiteral("cb_anti_aliasing"));
        cb_anti_aliasing->setGeometry(QRect(130, 181, 91, 31));
        label_25 = new QLabel(tab_3);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setGeometry(QRect(20, 260, 61, 31));
        rb_bloom = new QRadioButton(tab_3);
        rb_bloom->setObjectName(QStringLiteral("rb_bloom"));
        rb_bloom->setGeometry(QRect(130, 260, 91, 31));
        rb_bloom->setAutoExclusive(false);
        label_23 = new QLabel(tab_3);
        label_23->setObjectName(QStringLiteral("label_23"));
        label_23->setGeometry(QRect(20, 180, 91, 31));
        rb_hdr = new QRadioButton(tab_3);
        rb_hdr->setObjectName(QStringLiteral("rb_hdr"));
        rb_hdr->setGeometry(QRect(130, 220, 91, 31));
        rb_hdr->setAutoExclusive(false);
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        le_sc_val1 = new QLineEdit(tab_4);
        le_sc_val1->setObjectName(QStringLiteral("le_sc_val1"));
        le_sc_val1->setGeometry(QRect(120, 500, 101, 20));
        cb_sc_type = new QComboBox(tab_4);
        cb_sc_type->setObjectName(QStringLiteral("cb_sc_type"));
        cb_sc_type->setGeometry(QRect(120, 330, 101, 22));
        label_29 = new QLabel(tab_4);
        label_29->setObjectName(QStringLiteral("label_29"));
        label_29->setGeometry(QRect(30, 430, 61, 20));
        le_sc_from = new QLineEdit(tab_4);
        le_sc_from->setObjectName(QStringLiteral("le_sc_from"));
        le_sc_from->setGeometry(QRect(120, 400, 101, 20));
        label_26 = new QLabel(tab_4);
        label_26->setObjectName(QStringLiteral("label_26"));
        label_26->setGeometry(QRect(30, 333, 47, 20));
        lbl_sc_x = new QLabel(tab_4);
        lbl_sc_x->setObjectName(QStringLiteral("lbl_sc_x"));
        lbl_sc_x->setGeometry(QRect(30, 500, 61, 20));
        pb_remove_script = new QPushButton(tab_4);
        pb_remove_script->setObjectName(QStringLiteral("pb_remove_script"));
        pb_remove_script->setGeometry(QRect(150, 290, 101, 23));
        le_sc_duration = new QLineEdit(tab_4);
        le_sc_duration->setObjectName(QStringLiteral("le_sc_duration"));
        le_sc_duration->setGeometry(QRect(120, 460, 101, 20));
        lbl_sc_time = new QLabel(tab_4);
        lbl_sc_time->setObjectName(QStringLiteral("lbl_sc_time"));
        lbl_sc_time->setGeometry(QRect(30, 460, 61, 20));
        le_sc_to = new QLineEdit(tab_4);
        le_sc_to->setObjectName(QStringLiteral("le_sc_to"));
        le_sc_to->setGeometry(QRect(120, 430, 101, 20));
        pb_add_successor = new QPushButton(tab_4);
        pb_add_successor->setObjectName(QStringLiteral("pb_add_successor"));
        pb_add_successor->setGeometry(QRect(120, 590, 101, 23));
        lw_sc_script_elements = new QListWidget(tab_4);
        lw_sc_script_elements->setObjectName(QStringLiteral("lw_sc_script_elements"));
        lw_sc_script_elements->setGeometry(QRect(40, 40, 201, 231));
        label_27 = new QLabel(tab_4);
        label_27->setObjectName(QStringLiteral("label_27"));
        label_27->setGeometry(QRect(30, 360, 47, 20));
        lbl_sc_z = new QLabel(tab_4);
        lbl_sc_z->setObjectName(QStringLiteral("lbl_sc_z"));
        lbl_sc_z->setGeometry(QRect(30, 560, 61, 20));
        le_sc_val3 = new QLineEdit(tab_4);
        le_sc_val3->setObjectName(QStringLiteral("le_sc_val3"));
        le_sc_val3->setGeometry(QRect(120, 560, 101, 20));
        le_sc_val2 = new QLineEdit(tab_4);
        le_sc_val2->setObjectName(QStringLiteral("le_sc_val2"));
        le_sc_val2->setGeometry(QRect(120, 530, 101, 20));
        label_28 = new QLabel(tab_4);
        label_28->setObjectName(QStringLiteral("label_28"));
        label_28->setGeometry(QRect(30, 400, 51, 21));
        pb_add_script = new QPushButton(tab_4);
        pb_add_script->setObjectName(QStringLiteral("pb_add_script"));
        pb_add_script->setGeometry(QRect(40, 290, 101, 23));
        cb_sc_trigger = new QComboBox(tab_4);
        cb_sc_trigger->setObjectName(QStringLiteral("cb_sc_trigger"));
        cb_sc_trigger->setGeometry(QRect(120, 360, 101, 22));
        lbl_sc_y = new QLabel(tab_4);
        lbl_sc_y->setObjectName(QStringLiteral("lbl_sc_y"));
        lbl_sc_y->setGeometry(QRect(30, 530, 61, 20));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(30, 10, 131, 16));
        tabWidget->addTab(tab_4, QString());
        pb_scene_pause = new QPushButton(EditorWindow);
        pb_scene_pause->setObjectName(QStringLiteral("pb_scene_pause"));
        pb_scene_pause->setGeometry(QRect(440, 0, 31, 23));
        pb_scene_play = new QPushButton(EditorWindow);
        pb_scene_play->setObjectName(QStringLiteral("pb_scene_play"));
        pb_scene_play->setGeometry(QRect(400, 0, 31, 23));
        pb_scene_reset = new QPushButton(EditorWindow);
        pb_scene_reset->setObjectName(QStringLiteral("pb_scene_reset"));
        pb_scene_reset->setGeometry(QRect(480, 0, 31, 23));
        lbl_time = new QLabel(EditorWindow);
        lbl_time->setObjectName(QStringLiteral("lbl_time"));
        lbl_time->setGeometry(QRect(530, 0, 61, 20));
        pbNewScene = new QPushButton(EditorWindow);
        pbNewScene->setObjectName(QStringLiteral("pbNewScene"));
        pbNewScene->setGeometry(QRect(20, 0, 75, 23));
        pbNewScene->setCheckable(false);

        retranslateUi(EditorWindow);

        tabWidget->setCurrentIndex(0);
        cb_shadow_type->setCurrentIndex(2);
        cb_shadow_type_point->setCurrentIndex(2);
        cb_anti_aliasing->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(EditorWindow);
    } // setupUi

    void retranslateUi(QDialog *EditorWindow)
    {
        pbLoadScene->setText(QApplication::translate("EditorWindow", "Load Scene", Q_NULLPTR));
        pbSaveScene->setText(QApplication::translate("EditorWindow", "Save Scene", Q_NULLPTR));
        gb_obj->setTitle(QApplication::translate("EditorWindow", "Current Object", Q_NULLPTR));
        lbl_object_txt->setText(QApplication::translate("EditorWindow", "...", Q_NULLPTR));
        label->setText(QApplication::translate("EditorWindow", "Position:", Q_NULLPTR));
        label_2->setText(QApplication::translate("EditorWindow", "x", Q_NULLPTR));
        label_3->setText(QApplication::translate("EditorWindow", "y", Q_NULLPTR));
        label_4->setText(QApplication::translate("EditorWindow", "z", Q_NULLPTR));
        label_8->setText(QApplication::translate("EditorWindow", "Rotation:", Q_NULLPTR));
        rb_obj_apply_physic->setText(QApplication::translate("EditorWindow", "physics object", Q_NULLPTR));
        rb_obj_physic_static->setText(QApplication::translate("EditorWindow", "static", Q_NULLPTR));
        rb_obj_fixed->setText(QApplication::translate("EditorWindow", "fixed", Q_NULLPTR));
        rb_obj_visible->setText(QApplication::translate("EditorWindow", "visible", Q_NULLPTR));
        rb_obj_shadow_only->setText(QApplication::translate("EditorWindow", "shadow only", Q_NULLPTR));
        pb_object_remove->setText(QApplication::translate("EditorWindow", "remove", Q_NULLPTR));
        label_32->setText(QApplication::translate("EditorWindow", "Scale:", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("EditorWindow", "Scene objects", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(Objects), QApplication::translate("EditorWindow", "Objects", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("EditorWindow", "Object Database", Q_NULLPTR));
        lbl_objectdb_txt->setText(QApplication::translate("EditorWindow", "...", Q_NULLPTR));
        pb_objects_add->setText(QApplication::translate("EditorWindow", "A\n"
"D\n"
"D", Q_NULLPTR));
        lbl_objectdb_selected->setText(QString());
        pbDBLoad->setText(QApplication::translate("EditorWindow", "Load", Q_NULLPTR));
        pbDBNew->setText(QApplication::translate("EditorWindow", "New", Q_NULLPTR));
        pbEdit->setText(QApplication::translate("EditorWindow", "Edit", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("EditorWindow", "Object DB", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("EditorWindow", "New light", Q_NULLPTR));
        cb_lights_type->clear();
        cb_lights_type->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "directional", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "spot", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "point", Q_NULLPTR)
        );
        cb_lights_type->setCurrentText(QApplication::translate("EditorWindow", "directional", Q_NULLPTR));
        pb_lights_add->setText(QApplication::translate("EditorWindow", "Add", Q_NULLPTR));
        rb_lights_cast_shadow->setText(QApplication::translate("EditorWindow", "cast shadow", Q_NULLPTR));
        gb_light->setTitle(QApplication::translate("EditorWindow", "Current light", Q_NULLPTR));
        lbl_light_txt->setText(QApplication::translate("EditorWindow", "...", Q_NULLPTR));
        label_9->setText(QApplication::translate("EditorWindow", "Position:", Q_NULLPTR));
        label_10->setText(QApplication::translate("EditorWindow", "x", Q_NULLPTR));
        label_11->setText(QApplication::translate("EditorWindow", "y", Q_NULLPTR));
        label_12->setText(QApplication::translate("EditorWindow", "z", Q_NULLPTR));
        label_13->setText(QApplication::translate("EditorWindow", "x", Q_NULLPTR));
        label_14->setText(QApplication::translate("EditorWindow", "z", Q_NULLPTR));
        label_15->setText(QApplication::translate("EditorWindow", "y", Q_NULLPTR));
        label_16->setText(QApplication::translate("EditorWindow", "Direction:", Q_NULLPTR));
        label_17->setText(QApplication::translate("EditorWindow", "R", Q_NULLPTR));
        label_18->setText(QApplication::translate("EditorWindow", "G", Q_NULLPTR));
        label_19->setText(QApplication::translate("EditorWindow", "B", Q_NULLPTR));
        label_20->setText(QApplication::translate("EditorWindow", "Intensity:", Q_NULLPTR));
        label_21->setText(QApplication::translate("EditorWindow", "Color:", Q_NULLPTR));
        pb_light_remove->setText(QApplication::translate("EditorWindow", "remove", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("EditorWindow", "Lights", Q_NULLPTR));
        label_31->setText(QApplication::translate("EditorWindow", "SSAO", Q_NULLPTR));
        rb_ssao->setText(QString());
        label_22->setText(QApplication::translate("EditorWindow", "Shadows Directional", Q_NULLPTR));
        cb_shadow_type->clear();
        cb_shadow_type->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "off", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "Standard", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "PCF", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "RandomSampling", Q_NULLPTR)
        );
        label_30->setText(QApplication::translate("EditorWindow", "Shadows Point", Q_NULLPTR));
        cb_shadow_type_point->clear();
        cb_shadow_type_point->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "off", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "Standard", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "PCF", Q_NULLPTR)
        );
        label_24->setText(QApplication::translate("EditorWindow", "HDR", Q_NULLPTR));
        cb_anti_aliasing->clear();
        cb_anti_aliasing->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "off", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "FXAA", Q_NULLPTR)
        );
        label_25->setText(QApplication::translate("EditorWindow", "Bloom", Q_NULLPTR));
        rb_bloom->setText(QString());
        label_23->setText(QApplication::translate("EditorWindow", "Anti-aliasing", Q_NULLPTR));
        rb_hdr->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("EditorWindow", "Options", Q_NULLPTR));
        cb_sc_type->clear();
        cb_sc_type->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "rotation", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "translation", Q_NULLPTR)
        );
        label_29->setText(QApplication::translate("EditorWindow", "to", Q_NULLPTR));
        label_26->setText(QApplication::translate("EditorWindow", "type", Q_NULLPTR));
        lbl_sc_x->setText(QApplication::translate("EditorWindow", "x", Q_NULLPTR));
        pb_remove_script->setText(QApplication::translate("EditorWindow", "Remove", Q_NULLPTR));
        lbl_sc_time->setText(QApplication::translate("EditorWindow", "duration", Q_NULLPTR));
        pb_add_successor->setText(QApplication::translate("EditorWindow", "Add successor", Q_NULLPTR));
        label_27->setText(QApplication::translate("EditorWindow", "trigger", Q_NULLPTR));
        lbl_sc_z->setText(QApplication::translate("EditorWindow", "z", Q_NULLPTR));
        label_28->setText(QApplication::translate("EditorWindow", "from", Q_NULLPTR));
        pb_add_script->setText(QApplication::translate("EditorWindow", "Add", Q_NULLPTR));
        cb_sc_trigger->clear();
        cb_sc_trigger->insertItems(0, QStringList()
         << QApplication::translate("EditorWindow", "always", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "from-to", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "on-off", Q_NULLPTR)
         << QApplication::translate("EditorWindow", "wait", Q_NULLPTR)
        );
        lbl_sc_y->setText(QApplication::translate("EditorWindow", "y", Q_NULLPTR));
        label_5->setText(QApplication::translate("EditorWindow", "!! EXPERIMENTAL !!", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("EditorWindow", "Script", Q_NULLPTR));
        pb_scene_pause->setText(QApplication::translate("EditorWindow", "||", Q_NULLPTR));
        pb_scene_play->setText(QApplication::translate("EditorWindow", ">", Q_NULLPTR));
        pb_scene_reset->setText(QApplication::translate("EditorWindow", "|<<", Q_NULLPTR));
        lbl_time->setText(QApplication::translate("EditorWindow", "time", Q_NULLPTR));
        pbNewScene->setText(QApplication::translate("EditorWindow", "New Scene", Q_NULLPTR));
        Q_UNUSED(EditorWindow);
    } // retranslateUi

};

namespace Ui {
    class EditorWindow: public Ui_EditorWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORWINDOW_H
