#ifndef EDITOR_H
#define EDITOR_H

#include <QtWidgets/QMainWindow>

namespace Ui 
{
  class MainWindow;
}


class Editor : public QMainWindow
{

  //Q_OBJECT
public:
  //Editor();

  explicit Editor(QWidget *parent = 0);
  ~Editor();

private:
  Ui::MainWindow *m_ui;
};

#endif
