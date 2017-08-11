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

#include "Engine.h"
#include "Scene.h"
#include "Camera.h"

int main(int argc, char *argv[])
{
  bool edit = true;
  if(edit)
  {
    QApplication app(argc, argv);

    EditorWindow *  edit_win = new EditorWindow ;

    edit_win->show();

    return app.exec();
  }
  else
  {
    Engine * engine = new Engine();
    engine->initialize(800,600,PHYSIC_SUBSYSTEM ,EM_NORMAL,false);
    Scene * scene = new Scene();
    scene->getCamera()->setRotationAllowed();
    scene->getCamera()->applyPhysicsDrop(1.7f);
    scene->getCamera()->dontAllowUpDownMovement();
    scene->getCamera()->dontAllowUpDownTranslation();
    scene->getCamera()->setMovementCollisionCheck();

    scene->load("demo2f.xml");
    engine->setScene(scene, false);
    engine->run();
    delete engine;
  }
}

