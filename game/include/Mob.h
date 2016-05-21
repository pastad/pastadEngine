#ifndef MOB_H
#define MOB_H

class Object;


// represents a mobile in the game

class Mob
{
public:
  Mob();
  ~Mob();

  Mob(const Mob& other) = delete;
  Mob& operator=(const Mob& other) = delete;


  // initializes the mobile
  bool initialize();

  // updates the mobile
  void update();


private:

  Object  * m_object;

};

#endif