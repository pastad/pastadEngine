/********************************************************************************
** Form generated from reading UI file 'SuccessorSelectorg18700.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SUCCESSORSELECTORG18700_H
#define SUCCESSORSELECTORG18700_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_EditSuccessors
{
public:
    QListWidget *lw_ss_script_element_chooser;
    QListWidget *lw_ss_successors;
    QPushButton *pb_ss_add;
    QPushButton *pb_ss_remove;

    void setupUi(QWidget *form_EditSuccessors)
    {
        if (form_EditSuccessors->objectName().isEmpty())
            form_EditSuccessors->setObjectName(QStringLiteral("form_EditSuccessors"));
        form_EditSuccessors->resize(452, 348);
        lw_ss_script_element_chooser = new QListWidget(form_EditSuccessors);
        lw_ss_script_element_chooser->setObjectName(QStringLiteral("lw_ss_script_element_chooser"));
        lw_ss_script_element_chooser->setGeometry(QRect(30, 10, 171, 331));
        lw_ss_successors = new QListWidget(form_EditSuccessors);
        lw_ss_successors->setObjectName(QStringLiteral("lw_ss_successors"));
        lw_ss_successors->setGeometry(QRect(260, 10, 171, 331));
        pb_ss_add = new QPushButton(form_EditSuccessors);
        pb_ss_add->setObjectName(QStringLiteral("pb_ss_add"));
        pb_ss_add->setGeometry(QRect(220, 50, 31, 101));
        pb_ss_remove = new QPushButton(form_EditSuccessors);
        pb_ss_remove->setObjectName(QStringLiteral("pb_ss_remove"));
        pb_ss_remove->setGeometry(QRect(220, 170, 31, 101));

        retranslateUi(form_EditSuccessors);

        QMetaObject::connectSlotsByName(form_EditSuccessors);
    } // setupUi

    void retranslateUi(QWidget *form_EditSuccessors)
    {
        form_EditSuccessors->setWindowTitle(QApplication::translate("form_EditSuccessors", "Form", 0));
        pb_ss_add->setText(QApplication::translate("form_EditSuccessors", ">", 0));
        pb_ss_remove->setText(QApplication::translate("form_EditSuccessors", "<", 0));
    } // retranslateUi

};

namespace Ui {
    class form_EditSuccessors: public Ui_form_EditSuccessors {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SUCCESSORSELECTORG18700_H
