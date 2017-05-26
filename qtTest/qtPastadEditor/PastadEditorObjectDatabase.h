#ifndef PASTADEDITOROBJECTDATABASE_H
#define PASTADEDITOROBJECTDATABASE_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_ObjectDatabaseEditor.h"

#include "ObjectDatabase.h"

class PastadEditorObjectDatabase : public QDialog
{
  Q_OBJECT

public:
  PastadEditorObjectDatabase(QWidget *parent = 0);
  ~PastadEditorObjectDatabase();

  void setObjectDatabase(ObjectDatabase * db);

  void refreshList();
  void refreshEntry();

public slots:

  void on_pb_objectdb_new_clicked();
  void on_pb_objectdb_apply_clicked();
  void on_pb_objectdb_remove_clicked();

  void on_lw_objects_chooser_currentRowChanged(int row);

  void on_pb_objectdb_path_clicked();
  void on_pb_objectdb_icon_path_clicked();


private:
  static Ui::form_EditObjectDatabase ui;

  ObjectDatabase * m_object_database = nullptr;
  ObjectDatabaseEntry  m_selected_entry ;


};

#endif // PASTADEDITOROBJECTDATABASE_H

