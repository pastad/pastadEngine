#include "Editor\Editor.h"
#include "ui_mainwindow.h"


Editor::Editor(QWidget *parent):QMainWindow(parent), m_ui(new Ui::MainWindow)
{
  m_ui->setupUi(this);
}


Editor::~Editor()
{
  delete m_ui;
}

