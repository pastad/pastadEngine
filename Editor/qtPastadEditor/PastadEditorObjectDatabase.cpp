#include "PastadEditorObjectDatabase.h"

#include <iostream>
#include <sstream>
#include <string>
#include <QtWidgets/QLineEdit>
#include <QtWidgets\qfiledialog.h>

#include "Engine.h"
#include "IOSubsystem.h"
#include "Log.h"

#include "ObjectDatabase.h"

Ui::form_EditObjectDatabase  PastadEditorObjectDatabase::ui;

PastadEditorObjectDatabase::PastadEditorObjectDatabase(QWidget *parent)
  : QDialog(parent)
{
  ui.setupUi(this);
  m_selected_entry.m_id = 0 ;
}

PastadEditorObjectDatabase::~PastadEditorObjectDatabase()
{
}

void PastadEditorObjectDatabase::setObjectDatabase(ObjectDatabase * db)
{
  m_object_database = db ;
  refreshList();
}

void PastadEditorObjectDatabase::on_lw_objects_chooser_currentRowChanged(int row)
{
  if (m_object_database != nullptr)
  {
    std::vector<ObjectDatabaseEntry> entries = m_object_database->getEntries();
    if( ui.lw_objects_chooser->currentRow()  < entries.size() )
    {
      m_selected_entry = entries[ui.lw_objects_chooser->currentRow()];
      refreshEntry();
    }
  }
}

void PastadEditorObjectDatabase::on_pb_objectdb_new_clicked()
{
  if (m_object_database != nullptr)
  {
    ObjectDatabaseEntry entry;
    entry.m_name = "NewName";
    entry.m_description = "...";
    entry.m_icon_path = "Not set";
    entry.m_path = "Not set";
    m_object_database->addNewEntry(entry);
    
    m_selected_entry = m_object_database->getEntries()[m_object_database->getEntries().size()-1] ;
    refreshEntry();
    refreshList();
  }
}

void PastadEditorObjectDatabase::on_pb_objectdb_apply_clicked()
{
  if (m_selected_entry.m_id != 0)
  {    
    m_selected_entry.m_name = ui.le_objectdb_txt->text().toStdString();
    m_selected_entry.m_description = ui.pt_objectdb_description->toPlainText().toStdString();
    m_selected_entry.m_path = ui.lbl_objectdb_path->text().toStdString();
    m_selected_entry.m_icon_path = ui.lbl_objectdb_icon_path->text().toStdString();
    m_object_database->updateEntry(m_selected_entry);
    refreshEntry();
    refreshList();
  }
}

void PastadEditorObjectDatabase::on_pb_objectdb_remove_clicked()
{
  if ( (m_selected_entry.m_id != 0) && (m_object_database != nullptr) )
  {
    m_object_database->removeEntry(m_selected_entry);
    m_selected_entry.m_id = 0;
    refreshEntry();
    refreshList();
  }
}

void PastadEditorObjectDatabase::on_pb_objectdb_path_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this,
    tr("Model file path "), QDir::currentPath(), tr("Model Files (*.*)"));
  if( file_name.toUtf8() == "" )
    file_name =  QString("Not set");
  else
  {
    std::size_t found = file_name.toStdString().find(QDir::currentPath().toStdString());
    if (found != std::string::npos)
    {
      std::cout << QDir::currentPath().toStdString().size() <<std::endl;
      std::string str3 = file_name.toStdString().substr(QDir::currentPath().toStdString().size()+1);

      ui.lbl_objectdb_path->setText( QString::fromUtf8( str3.c_str()) );
    }
  }
}
void PastadEditorObjectDatabase::on_pb_objectdb_icon_path_clicked()
{
  QString file_name = QFileDialog::getOpenFileName(this,
    tr("Icon file path "), QDir::currentPath(), tr("PNG Files (*.png)"));
  if (file_name.toUtf8() == "")
    file_name = QString("Not set");
  else
  {
    std::size_t found = file_name.toStdString().find(QDir::currentPath().toStdString());
    if (found != std::string::npos)
    {
      std::cout << QDir::currentPath().toStdString().size() << std::endl;
      std::string str3 = file_name.toStdString().substr(QDir::currentPath().toStdString().size()+1);

      ui.lbl_objectdb_icon_path->setText(QString::fromUtf8(str3.c_str()));
    }
  }
}

void PastadEditorObjectDatabase::refreshList()
{
  if(m_object_database != nullptr)
  {
 //   ui.lw_objects_chooser->clear();
    while (ui.lw_objects_chooser->count()>0)
      ui.lw_objects_chooser->takeItem(0);
    
    std::vector<ObjectDatabaseEntry> entries = m_object_database->getEntries();
    for (std::vector<ObjectDatabaseEntry>::iterator it = entries.begin(); it != entries.end(); it++)
    {
      ui.lw_objects_chooser->insertItem(ui.lw_objects_chooser->count(), QString::fromUtf8((*it).m_name.c_str()));
    }
  }
}

void PastadEditorObjectDatabase::refreshEntry()
{
  if (m_selected_entry.m_id != 0)
  {
    ui.le_objectdb_txt->setText(QString::fromUtf8(m_selected_entry.m_name.c_str()));
    ui.pt_objectdb_description->document()->setPlainText(QString::fromUtf8(m_selected_entry.m_description.c_str()));

    ui.lbl_objectdb_path->setText(QString::fromUtf8(m_selected_entry.m_path.c_str()));
    ui.lbl_objectdb_icon_path->setText(QString::fromUtf8(m_selected_entry.m_icon_path.c_str()));
  
    QPixmap pix(m_selected_entry.m_icon_path.c_str());
    int w = ui.lbl_test->width();
    int h = ui.lbl_test->height();
    ui.lbl_test->setPixmap(pix.scaled(w,h,Qt::IgnoreAspectRatio) );  
  }
}