#include "Inventory.h"

#include "Item.h"
#include "Engine.h"
#include "Player.h"


#include <iostream>

Inventory::Inventory():m_selected_item(-1)
{  
}

Inventory::~Inventory()
{
  for(std::vector<Item *>::iterator it = m_items.begin(); it != m_items.end(); )
  {
    delete(*it);
    it = m_items.erase(it);
  }  
}

void Inventory::addItem(Item * item)
{
  m_items.push_back(item);
}

void Inventory::selectItem(unsigned int idx)
{
  if( (m_items.size() >idx) && (idx >=0) && (m_selected_item != idx) )
  {    
    if( m_selected_item != -1)
      m_items[m_selected_item]->setInactive();  
    m_selected_item = idx;
    m_items[m_selected_item]->setActive();
    Player::refreshSparks();
  }
}

void Inventory::scroll(double x)
{
  //std::cout << x <<std::endl;
	if( ( (x < 0) && ( -x <= m_selected_item )  )  || (x >0) )
		selectItem(m_selected_item + (int) x);
}

Item * Inventory::getSelectedItem()
{
  if( m_selected_item != -1)
    return m_items[m_selected_item];
  return nullptr;
}

void Inventory::update()
{
  if( Engine::isKeyReleasedAndPressed('1'))
    selectItem(0);
  if( Engine::isKeyReleasedAndPressed('2'))
    selectItem(1);
  if( Engine::isKeyReleasedAndPressed('3'))
    selectItem(2);

}