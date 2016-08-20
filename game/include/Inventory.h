#ifndef INVENTORY_H
#define INVENTORY_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>

class Item;

// the players inevntory

class Inventory
{
public:
  Inventory();
  ~Inventory();

  Inventory(const Inventory& other) = delete;
  Inventory& operator=(const Inventory& other) = delete;

  // adds an item to the inventory
  void addItem(Item * item);

  // selects the item with the index 
  void selectItem(unsigned int idx);

  // returns the selected item
  Item * getSelectedItem();

  // scrolls up/down the inventory
  void scroll(double x);

  // updates the inventory
  void update();


private:

  // the items of the inventory
  std::vector<Item *> m_items;

  // the index of the selected item
  int m_selected_item;
};

#endif // INVENTORY_H