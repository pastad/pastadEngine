/********************************************************************************
** Form generated from reading UI file 'PastadEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASTADEDITOR_H
#define UI_PASTADEDITOR_H

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

QT_BEGIN_NAMESPACE

class Ui_PastadEditorClass
{
public:
    QAction *actionLoadScene;
    QWidget *centralWidget;
    QPushButton *pb_scene_pause;
    QPushButton *pb_scene_play;
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
    QRadioButton *rb_obj_shadow_only;
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
    QLabel *lbl_time;
    QMenuBar *menuBar;
    QMenu *menuScene;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PastadEditorClass)
    {
        if (PastadEditorClass->objectName().isEmpty())
            PastadEditorClass->setObjectName(QStringLiteral("PastadEditorClass"));
        PastadEditorClass->resize(724, 860);
        actionLoadScene = new QAction(PastadEditorClass);
        actionLoadScene->setObjectName(QStringLiteral("actionLoadScene"));
        centralWidget = new QWidget(PastadEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pb_scene_pause = new QPushButton(centralWidget);
        pb_scene_pause->setObjectName(QStringLiteral("pb_scene_pause"));
        pb_scene_pause->setGeometry(QRect(50, 10, 31, 23));
        pb_scene_play = new QPushButton(centralWidget);
        pb_scene_play->setObjectName(QStringLiteral("pb_scene_play"));
        pb_scene_play->setGeometry(QRect(10, 10, 31, 23));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 40, 671, 761));
        tabWidget->setLayoutDirection(Qt::LeftToRight);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gb_obj = new QGroupBox(tab);
        gb_obj->setObjectName(QStringLiteral("gb_obj"));
        gb_obj->setGeometry(QRect(20, 10, 251, 261));
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
        le_obj_rot_x->setGeometry(QRect(20, 170, 71, 21));
        label_5 = new QLabel(gb_obj);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 170, 20, 21));
        le_obj_rot_z = new QLineEdit(gb_obj);
        le_obj_rot_z->setObjectName(QStringLiteral("le_obj_rot_z"));
        le_obj_rot_z->setGeometry(QRect(20, 230, 71, 21));
        le_obj_rot_y = new QLineEdit(gb_obj);
        le_obj_rot_y->setObjectName(QStringLiteral("le_obj_rot_y"));
        le_obj_rot_y->setGeometry(QRect(20, 200, 71, 21));
        label_6 = new QLabel(gb_obj);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 230, 20, 21));
        label_7 = new QLabel(gb_obj);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 200, 20, 21));
        label_8 = new QLabel(gb_obj);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 150, 61, 16));
        label_8->setFont(font1);
        rb_obj_apply_physic = new QRadioButton(gb_obj);
        rb_obj_apply_physic->setObjectName(QStringLiteral("rb_obj_apply_physic"));
        rb_obj_apply_physic->setGeometry(QRect(130, 170, 91, 17));
        rb_obj_apply_physic->setAutoExclusive(false);
        rb_obj_physic_static = new QRadioButton(gb_obj);
        rb_obj_physic_static->setObjectName(QStringLiteral("rb_obj_physic_static"));
        rb_obj_physic_static->setGeometry(QRect(160, 190, 61, 17));
        rb_obj_physic_static->setAutoExclusive(false);
        rb_obj_fixed = new QRadioButton(gb_obj);
        rb_obj_fixed->setObjectName(QStringLiteral("rb_obj_fixed"));
        rb_obj_fixed->setGeometry(QRect(130, 150, 82, 17));
        rb_obj_fixed->setAutoExclusive(false);
        rb_obj_visible = new QRadioButton(gb_obj);
        rb_obj_visible->setObjectName(QStringLiteral("rb_obj_visible"));
        rb_obj_visible->setGeometry(QRect(130, 210, 82, 17));
        rb_obj_visible->setAutoExclusive(false);
        rb_obj_shadow_only = new QRadioButton(gb_obj);
        rb_obj_shadow_only->setObjectName(QStringLiteral("rb_obj_shadow_only"));
        rb_obj_shadow_only->setGeometry(QRect(130, 230, 82, 17));
        rb_obj_shadow_only->setAutoExclusive(false);
        pb_object_remove = new QPushButton(gb_obj);
        pb_object_remove->setObjectName(QStringLiteral("pb_object_remove"));
        pb_object_remove->setGeometry(QRect(130, 110, 75, 23));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(290, 10, 361, 711));
        lv_objects_chooser = new QListView(groupBox);
        lv_objects_chooser->setObjectName(QStringLiteral("lv_objects_chooser"));
        lv_objects_chooser->setGeometry(QRect(20, 220, 321, 471));
        pb_objects_add = new QPushButton(groupBox);
        pb_objects_add->setObjectName(QStringLiteral("pb_objects_add"));
        pb_objects_add->setGeometry(QRect(200, 30, 91, 23));
        f_objects_selected = new QFrame(groupBox);
        f_objects_selected->setObjectName(QStringLiteral("f_objects_selected"));
        f_objects_selected->setGeometry(QRect(20, 70, 151, 141));
        f_objects_selected->setAutoFillBackground(true);
        f_objects_selected->setFrameShape(QFrame::StyledPanel);
        f_objects_selected->setFrameShadow(QFrame::Raised);
        lbl_objectdb_txt = new QLabel(groupBox);
        lbl_objectdb_txt->setObjectName(QStringLiteral("lbl_objectdb_txt"));
        lbl_objectdb_txt->setGeometry(QRect(30, 40, 131, 16));
        lbl_objectdb_txt->setFont(font);
        tb_ojectdb_description = new QTextBrowser(groupBox);
        tb_ojectdb_description->setObjectName(QStringLiteral("tb_ojectdb_description"));
        tb_ojectdb_description->setGeometry(QRect(180, 70, 161, 141));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 280, 261, 441));
        lw_objects_active = new QListWidget(groupBox_2);
        lw_objects_active->setObjectName(QStringLiteral("lw_objects_active"));
        lw_objects_active->setGeometry(QRect(10, 20, 241, 401));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gb_light = new QGroupBox(tab_2);
        gb_light->setObjectName(QStringLiteral("gb_light"));
        gb_light->setGeometry(QRect(10, 10, 221, 261));
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
        sb_light_g = new QSpinBox(gb_light);
        sb_light_g->setObjectName(QStringLiteral("sb_light_g"));
        sb_light_g->setGeometry(QRect(140, 90, 41, 22));
        sb_light_b = new QSpinBox(gb_light);
        sb_light_b->setObjectName(QStringLiteral("sb_light_b"));
        sb_light_b->setGeometry(QRect(140, 120, 41, 22));
        label_17 = new QLabel(gb_light);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(120, 60, 20, 21));
        label_18 = new QLabel(gb_light);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(120, 90, 20, 21));
        label_19 = new QLabel(gb_light);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(120, 120, 20, 21));
        doubleSpinBox = new QDoubleSpinBox(gb_light);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setGeometry(QRect(140, 170, 51, 22));
        label_20 = new QLabel(gb_light);
        label_20->setObjectName(QStringLiteral("label_20"));
        label_20->setGeometry(QRect(110, 150, 61, 16));
        label_20->setFont(font1);
        label_21 = new QLabel(gb_light);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(110, 40, 61, 16));
        label_21->setFont(font1);
        lw_lights_active = new QListWidget(tab_2);
        lw_lights_active->setObjectName(QStringLiteral("lw_lights_active"));
        lw_lights_active->setGeometry(QRect(10, 280, 221, 231));
        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(240, 10, 201, 101));
        cb_lights_type = new QComboBox(groupBox_3);
        cb_lights_type->setObjectName(QStringLiteral("cb_lights_type"));
        cb_lights_type->setGeometry(QRect(10, 30, 81, 22));
        pb_lights_add = new QPushButton(groupBox_3);
        pb_lights_add->setObjectName(QStringLiteral("pb_lights_add"));
        pb_lights_add->setGeometry(QRect(110, 30, 81, 23));
        rb_lights_cast_shadow = new QRadioButton(groupBox_3);
        rb_lights_cast_shadow->setObjectName(QStringLiteral("rb_lights_cast_shadow"));
        rb_lights_cast_shadow->setGeometry(QRect(10, 70, 82, 17));
        tabWidget->addTab(tab_2, QString());
        lbl_time = new QLabel(centralWidget);
        lbl_time->setObjectName(QStringLiteral("lbl_time"));
        lbl_time->setGeometry(QRect(90, 10, 47, 13));
        PastadEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PastadEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 724, 21));
        menuScene = new QMenu(menuBar);
        menuScene->setObjectName(QStringLiteral("menuScene"));
        PastadEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PastadEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PastadEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PastadEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PastadEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuScene->menuAction());
        menuScene->addAction(actionLoadScene);

        retranslateUi(PastadEditorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(PastadEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *PastadEditorClass)
    {
        PastadEditorClass->setWindowTitle(QApplication::translate("PastadEditorClass", "PastadEditor", 0));
        actionLoadScene->setText(QApplication::translate("PastadEditorClass", "Load", 0));
        pb_scene_pause->setText(QApplication::translate("PastadEditorClass", "||", 0));
        pb_scene_play->setText(QApplication::translate("PastadEditorClass", ">", 0));
        gb_obj->setTitle(QApplication::translate("PastadEditorClass", "Current Object", 0));
        lbl_object_txt->setText(QApplication::translate("PastadEditorClass", "...", 0));
        label->setText(QApplication::translate("PastadEditorClass", "Position:", 0));
        label_2->setText(QApplication::translate("PastadEditorClass", "x", 0));
        label_3->setText(QApplication::translate("PastadEditorClass", "y", 0));
        label_4->setText(QApplication::translate("PastadEditorClass", "z", 0));
        label_5->setText(QApplication::translate("PastadEditorClass", "x", 0));
        label_6->setText(QApplication::translate("PastadEditorClass", "z", 0));
        label_7->setText(QApplication::translate("PastadEditorClass", "y", 0));
        label_8->setText(QApplication::translate("PastadEditorClass", "Rotation:", 0));
        rb_obj_apply_physic->setText(QApplication::translate("PastadEditorClass", "apply physics", 0));
        rb_obj_physic_static->setText(QApplication::translate("PastadEditorClass", "static", 0));
        rb_obj_fixed->setText(QApplication::translate("PastadEditorClass", "fixed", 0));
        rb_obj_visible->setText(QApplication::translate("PastadEditorClass", "visible", 0));
        rb_obj_shadow_only->setText(QApplication::translate("PastadEditorClass", "shadow only", 0));
        pb_object_remove->setText(QApplication::translate("PastadEditorClass", "remove", 0));
        groupBox->setTitle(QApplication::translate("PastadEditorClass", "Object Database", 0));
        pb_objects_add->setText(QApplication::translate("PastadEditorClass", "Add", 0));
        lbl_objectdb_txt->setText(QApplication::translate("PastadEditorClass", "...", 0));
        groupBox_2->setTitle(QApplication::translate("PastadEditorClass", "Scene objects", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("PastadEditorClass", "Objects", 0));
        gb_light->setTitle(QApplication::translate("PastadEditorClass", "Current light", 0));
        lbl_light_txt->setText(QApplication::translate("PastadEditorClass", "...", 0));
        label_9->setText(QApplication::translate("PastadEditorClass", "Position:", 0));
        label_10->setText(QApplication::translate("PastadEditorClass", "x", 0));
        label_11->setText(QApplication::translate("PastadEditorClass", "y", 0));
        label_12->setText(QApplication::translate("PastadEditorClass", "z", 0));
        label_13->setText(QApplication::translate("PastadEditorClass", "x", 0));
        label_14->setText(QApplication::translate("PastadEditorClass", "z", 0));
        label_15->setText(QApplication::translate("PastadEditorClass", "y", 0));
        label_16->setText(QApplication::translate("PastadEditorClass", "Direction:", 0));
        label_17->setText(QApplication::translate("PastadEditorClass", "R", 0));
        label_18->setText(QApplication::translate("PastadEditorClass", "G", 0));
        label_19->setText(QApplication::translate("PastadEditorClass", "B", 0));
        label_20->setText(QApplication::translate("PastadEditorClass", "Intensity:", 0));
        label_21->setText(QApplication::translate("PastadEditorClass", "Color:", 0));
        groupBox_3->setTitle(QApplication::translate("PastadEditorClass", "New light", 0));
        cb_lights_type->clear();
        cb_lights_type->insertItems(0, QStringList()
         << QApplication::translate("PastadEditorClass", "directional", 0)
         << QApplication::translate("PastadEditorClass", "spot", 0)
         << QApplication::translate("PastadEditorClass", "point", 0)
        );
        cb_lights_type->setCurrentText(QApplication::translate("PastadEditorClass", "directional", 0));
        pb_lights_add->setText(QApplication::translate("PastadEditorClass", "Add", 0));
        rb_lights_cast_shadow->setText(QApplication::translate("PastadEditorClass", "cast shadow", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("PastadEditorClass", "Lights", 0));
        lbl_time->setText(QApplication::translate("PastadEditorClass", "time", 0));
        menuScene->setTitle(QApplication::translate("PastadEditorClass", "Scene", 0));
    } // retranslateUi

};

namespace Ui {
    class PastadEditorClass: public Ui_PastadEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASTADEDITOR_H
