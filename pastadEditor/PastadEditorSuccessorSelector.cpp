#include "PastadEditorSuccessorSelector.h"

#include <iostream>
#include <sstream>
#include <string>
#include <QtWidgets/QLineEdit>
#include <QtWidgets\qfiledialog.h>

#include "Engine.h"
#include "IOSubsystem.h"
#include "Log.h"

#include "ScriptElement.h"


Ui::form_EditSuccessors  PastadEditorSuccessorSelector::ui;

PastadEditorSuccessorSelector::PastadEditorSuccessorSelector(QWidget *parent)
  : QDialog(parent)
{
  ui.setupUi(this);
}

PastadEditorSuccessorSelector::~PastadEditorSuccessorSelector()
{
}

void PastadEditorSuccessorSelector::setup(std::vector<ScriptElement *> selected, std::vector<ScriptElement *> all, ScriptElement * current)
{
  m_selected = selected;

  m_others = all;
  
  for (std::vector<ScriptElement *>::iterator it = m_others.begin(); it != m_others.end(); )
  {
    bool stay =  true;
    for (std::vector<ScriptElement *>::iterator it2 = m_selected.begin(); it2 != m_selected.end(); it2++ )
    {
      if(  (*it) == (*it2)   )
        stay = false;
    }
    if ( (*it) == (current) )
      stay = false;
    if (!stay)
    {
     it = m_others.erase(it);
    }
    else
      it++;

  }
  refresh();
}
void PastadEditorSuccessorSelector::refresh()
{
  ui.lw_ss_script_element_chooser->reset();
  while (ui.lw_ss_script_element_chooser->count()>0)
    ui.lw_ss_script_element_chooser->takeItem(0);

  ui.lw_ss_successors->reset();
  while (ui.lw_ss_successors->count()>0)
    ui.lw_ss_successors->takeItem(0);

  for (std::vector<ScriptElement *>::iterator it = m_others.begin(); it != m_others.end(); it++)
  {
    ui.lw_ss_script_element_chooser->insertItem(ui.lw_ss_script_element_chooser->count(), QString::fromUtf8((*it)->getDescription().c_str()));
  }
  for (std::vector<ScriptElement *>::iterator it = m_selected.begin(); it != m_selected.end(); it++)
  {
    ui.lw_ss_successors->insertItem(ui.lw_ss_successors->count(), QString::fromUtf8( (*it)->getDescription().c_str()  ));
  }
}

void PastadEditorSuccessorSelector::on_pb_ss_add_clicked()
{

  std::cout << "add" << std::endl;
  int cr = ui.lw_ss_script_element_chooser->currentRow();
  if ((cr<m_others.size()) && (cr >= 0))
  {
    ScriptElement * se = m_others[cr];

    std::vector<ScriptElement *>::iterator it = m_others.begin();
    while(cr >0)
    { 
      cr--;
      it++;
    }
    m_others.erase(it);

    std::cout <<m_others.size() <<std::endl;

    m_selected.push_back(se);
  }
  refresh();

}

void PastadEditorSuccessorSelector::on_pb_ss_remove_clicked()
{
  int cr = ui.lw_ss_successors->currentRow();
  if ((cr<m_selected.size()) && (cr >= 0))
  {
    ScriptElement * se = m_selected[cr];

    std::vector<ScriptElement *>::iterator it = m_selected.begin();
    while (cr >0)
    {
      cr--;
      it++;
    }
    m_selected.erase(it);

    m_others.push_back(se);
  }
  refresh();
}

std::vector<ScriptElement *> PastadEditorSuccessorSelector::getSelected()
{
  return m_selected;
}
