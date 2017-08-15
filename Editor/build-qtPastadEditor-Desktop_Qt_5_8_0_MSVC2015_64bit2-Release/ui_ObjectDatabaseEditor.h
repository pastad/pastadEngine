/********************************************************************************
** Form generated from reading UI file 'ObjectDatabaseEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTDATABASEEDITOR_H
#define UI_OBJECTDATABASEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_EditObjectDatabase
{
public:
    QListWidget *lw_objects_chooser;
    QPushButton *pb_objectdb_new;
    QPushButton *pb_objectdb_remove;
    QPushButton *pb_objectdb_apply;
    QPushButton *pb_objectdb_path;
    QLineEdit *le_objectdb_txt;
    QPushButton *pb_objectdb_icon_path;
    QLabel *lbl_objectdb_path;
    QLabel *lbl_objectdb_icon_path;
    QLabel *lbl_objectdb_path_3;
    QLabel *lbl_objectdb_path_4;
    QLabel *lbl_objectdb_path_5;
    QLabel *lbl_objectdb_path_6;
    QPlainTextEdit *pt_objectdb_description;
    QLabel *lbl_test;

    void setupUi(QWidget *form_EditObjectDatabase)
    {
        if (form_EditObjectDatabase->objectName().isEmpty())
            form_EditObjectDatabase->setObjectName(QStringLiteral("form_EditObjectDatabase"));
        form_EditObjectDatabase->setWindowModality(Qt::ApplicationModal);
        form_EditObjectDatabase->resize(476, 502);
        lw_objects_chooser = new QListWidget(form_EditObjectDatabase);
        lw_objects_chooser->setObjectName(QStringLiteral("lw_objects_chooser"));
        lw_objects_chooser->setGeometry(QRect(280, 190, 171, 291));
        pb_objectdb_new = new QPushButton(form_EditObjectDatabase);
        pb_objectdb_new->setObjectName(QStringLiteral("pb_objectdb_new"));
        pb_objectdb_new->setGeometry(QRect(20, 10, 75, 23));
        pb_objectdb_remove = new QPushButton(form_EditObjectDatabase);
        pb_objectdb_remove->setObjectName(QStringLiteral("pb_objectdb_remove"));
        pb_objectdb_remove->setGeometry(QRect(180, 10, 75, 23));
        pb_objectdb_apply = new QPushButton(form_EditObjectDatabase);
        pb_objectdb_apply->setObjectName(QStringLiteral("pb_objectdb_apply"));
        pb_objectdb_apply->setGeometry(QRect(100, 10, 75, 23));
        pb_objectdb_path = new QPushButton(form_EditObjectDatabase);
        pb_objectdb_path->setObjectName(QStringLiteral("pb_objectdb_path"));
        pb_objectdb_path->setGeometry(QRect(410, 100, 31, 20));
        le_objectdb_txt = new QLineEdit(form_EditObjectDatabase);
        le_objectdb_txt->setObjectName(QStringLiteral("le_objectdb_txt"));
        le_objectdb_txt->setGeometry(QRect(100, 60, 301, 20));
        pb_objectdb_icon_path = new QPushButton(form_EditObjectDatabase);
        pb_objectdb_icon_path->setObjectName(QStringLiteral("pb_objectdb_icon_path"));
        pb_objectdb_icon_path->setGeometry(QRect(410, 130, 31, 20));
        lbl_objectdb_path = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_path->setObjectName(QStringLiteral("lbl_objectdb_path"));
        lbl_objectdb_path->setGeometry(QRect(100, 100, 291, 20));
        lbl_objectdb_icon_path = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_icon_path->setObjectName(QStringLiteral("lbl_objectdb_icon_path"));
        lbl_objectdb_icon_path->setGeometry(QRect(100, 130, 291, 16));
        lbl_objectdb_path_3 = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_path_3->setObjectName(QStringLiteral("lbl_objectdb_path_3"));
        lbl_objectdb_path_3->setGeometry(QRect(20, 60, 47, 13));
        lbl_objectdb_path_4 = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_path_4->setObjectName(QStringLiteral("lbl_objectdb_path_4"));
        lbl_objectdb_path_4->setGeometry(QRect(20, 100, 47, 13));
        lbl_objectdb_path_4->setFrameShape(QFrame::NoFrame);
        lbl_objectdb_path_5 = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_path_5->setObjectName(QStringLiteral("lbl_objectdb_path_5"));
        lbl_objectdb_path_5->setGeometry(QRect(20, 130, 47, 13));
        lbl_objectdb_path_5->setFrameShape(QFrame::NoFrame);
        lbl_objectdb_path_6 = new QLabel(form_EditObjectDatabase);
        lbl_objectdb_path_6->setObjectName(QStringLiteral("lbl_objectdb_path_6"));
        lbl_objectdb_path_6->setGeometry(QRect(10, 310, 61, 16));
        pt_objectdb_description = new QPlainTextEdit(form_EditObjectDatabase);
        pt_objectdb_description->setObjectName(QStringLiteral("pt_objectdb_description"));
        pt_objectdb_description->setGeometry(QRect(100, 310, 141, 131));
        lbl_test = new QLabel(form_EditObjectDatabase);
        lbl_test->setObjectName(QStringLiteral("lbl_test"));
        lbl_test->setGeometry(QRect(100, 160, 141, 121));
        lbl_test->raise();
        lw_objects_chooser->raise();
        pb_objectdb_new->raise();
        pb_objectdb_remove->raise();
        pb_objectdb_apply->raise();
        pb_objectdb_path->raise();
        le_objectdb_txt->raise();
        pb_objectdb_icon_path->raise();
        lbl_objectdb_path->raise();
        lbl_objectdb_icon_path->raise();
        lbl_objectdb_path_3->raise();
        lbl_objectdb_path_4->raise();
        lbl_objectdb_path_5->raise();
        lbl_objectdb_path_6->raise();
        pt_objectdb_description->raise();

        retranslateUi(form_EditObjectDatabase);

        QMetaObject::connectSlotsByName(form_EditObjectDatabase);
    } // setupUi

    void retranslateUi(QWidget *form_EditObjectDatabase)
    {
        form_EditObjectDatabase->setWindowTitle(QApplication::translate("form_EditObjectDatabase", "Database Editor", Q_NULLPTR));
        pb_objectdb_new->setText(QApplication::translate("form_EditObjectDatabase", "New", Q_NULLPTR));
        pb_objectdb_remove->setText(QApplication::translate("form_EditObjectDatabase", "Remove", Q_NULLPTR));
        pb_objectdb_apply->setText(QApplication::translate("form_EditObjectDatabase", "Apply", Q_NULLPTR));
        pb_objectdb_path->setText(QApplication::translate("form_EditObjectDatabase", "Set", Q_NULLPTR));
        pb_objectdb_icon_path->setText(QApplication::translate("form_EditObjectDatabase", "Set", Q_NULLPTR));
        lbl_objectdb_path->setText(QApplication::translate("form_EditObjectDatabase", "Not set", Q_NULLPTR));
        lbl_objectdb_icon_path->setText(QApplication::translate("form_EditObjectDatabase", "Not set", Q_NULLPTR));
        lbl_objectdb_path_3->setText(QApplication::translate("form_EditObjectDatabase", "Name", Q_NULLPTR));
        lbl_objectdb_path_4->setText(QApplication::translate("form_EditObjectDatabase", "Path", Q_NULLPTR));
        lbl_objectdb_path_5->setText(QApplication::translate("form_EditObjectDatabase", "Icon path", Q_NULLPTR));
        lbl_objectdb_path_6->setText(QApplication::translate("form_EditObjectDatabase", "Description", Q_NULLPTR));
        lbl_test->setText(QApplication::translate("form_EditObjectDatabase", "llllabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class form_EditObjectDatabase: public Ui_form_EditObjectDatabase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTDATABASEEDITOR_H
