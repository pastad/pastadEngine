#include "gl_core_4_.hpp"



#include "editorwindow.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qquickwindow.h>

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QWidget>
#include <QApplication>
#include <QDialog>

#include <QtCore/qmath.h>

#include <iostream>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
  //  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

   // QObject *topLevel = engine.rootObjects().value(0);
   // QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
   // window->show();
    EditorWindow *  edit_win = new EditorWindow ;


    edit_win->show();


    return app.exec();
}

