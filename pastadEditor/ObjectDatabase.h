#ifndef OBJECTDATABASE_H
#define OBJECTDATABASE_H

#include <string>
#include <vector>

struct ObjectDatabaseEntry
{
  std::string m_name;
  std::string m_path;
  std::string m_icon_path;
  std::string m_description;
};

// class that stores the objects for easier scene adding

class ObjectDatabase
{
public:
  ObjectDatabase(std::string name);
  ~ObjectDatabase();

  ObjectDatabase(const ObjectDatabase& other) = delete;
  ObjectDatabase& operator=(const ObjectDatabase& other) = delete;

  // load and save method
  bool load();
  bool save();

  // adds an entry to the db
  void addEntry();

private:
  
  // the path to the database file
  std::string m_path;
    
  // the entries in the database
  std::vector<ObjectDatabaseEntry> m_entries;

};

#endif
