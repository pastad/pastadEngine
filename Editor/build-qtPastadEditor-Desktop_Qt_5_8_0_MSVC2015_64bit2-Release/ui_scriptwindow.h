/********************************************************************************
** Form generated from reading UI file 'scriptwindow'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTWINDOW_H
#define UI_SCRIPTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_form_EditObjectDatabase
{
public:
    QPushButton *pushButton;

    void setupUi(QWidget *form_EditObjectDatabase)
    {
        if (form_EditObjectDatabase->objectName().isEmpty())
            form_EditObjectDatabase->setObjectName(QStringLiteral("form_EditObjectDatabase"));
        form_EditObjectDatabase->setWindowModality(Qt::ApplicationModal);
        form_EditObjectDatabase->resize(476, 502);
        pushButton = new QPushButton(form_EditObjectDatabase);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(100, 100, 75, 23));

        retranslateUi(form_EditObjectDatabase);

        QMetaObject::connectSlotsByName(form_EditObjectDatabase);
    } // setupUi

    void retranslateUi(QWidget *form_EditObjectDatabase)
    {
        form_EditObjectDatabase->setWindowTitle(QApplication::translate("form_EditObjectDatabase", "Form", Q_NULLPTR));
        pushButton->setText(QApplication::translate("form_EditObjectDatabase", "script", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class form_EditObjectDatabase: public Ui_form_EditObjectDatabase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTWINDOW_H
