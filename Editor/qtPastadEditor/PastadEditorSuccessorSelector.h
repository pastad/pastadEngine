#ifndef PASTADEDITORSUCCESSORSELECTOR_H
#define PASTADEDITORSUCCESSORSELECTOR_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_SuccessorSelector.h"

#include <vector>

class ScriptElement;

class PastadEditorSuccessorSelector : public QDialog
{
  Q_OBJECT

public:
  PastadEditorSuccessorSelector(QWidget *parent = 0);
  ~PastadEditorSuccessorSelector();

  void setup(std::vector<ScriptElement *> selected, std::vector<ScriptElement *> all, ScriptElement * current);

  void refresh();

public slots:
  void on_pb_ss_add_clicked();
  void on_pb_ss_remove_clicked();

  std::vector<ScriptElement *> getSelected();

private:
  static Ui::form_EditSuccessors ui;

  std::vector<ScriptElement *>  m_others;
  std::vector<ScriptElement *>  m_selected;

};

#endif // PASTADEDITORSUCCESSORSELECTOR_H

