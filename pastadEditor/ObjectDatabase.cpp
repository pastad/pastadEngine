#include "ObjectDatabase.h"

#include "tinyxml2.h"

#include "Engine.h"
#include "Log.h"

ObjectDatabase::ObjectDatabase(std::string name): m_path(name)
{
  m_id_count = 1;
}


ObjectDatabase::~ObjectDatabase()
{
}

void ObjectDatabase::addExistingEntry(ObjectDatabaseEntry entry)
{
  m_entries.push_back(entry);
  Engine::getLog()->log(LF_Editor, "ObjectDatabase", "added entry");
  if(entry.m_id >= m_id_count)
    m_id_count = entry.m_id+1;
}
void ObjectDatabase::addNewEntry(ObjectDatabaseEntry entry)
{
  entry.m_id = m_id_count;
  m_id_count++;
  m_entries.push_back(entry);
  Engine::getLog()->log(LF_Editor, "ObjectDatabase", "added entry");

}

void ObjectDatabase::updateEntry(ObjectDatabaseEntry entry)
{
  for (std::vector<ObjectDatabaseEntry>::iterator it = m_entries.begin(); it != m_entries.end();it++ )
  {
    if ((*it).m_id == entry.m_id)
    {
      (*it).m_name = entry.m_name;
      (*it).m_description = entry.m_description;
      (*it).m_path = entry.m_path;
      (*it).m_icon_path = entry.m_icon_path;
    }
  }
}

void ObjectDatabase::removeEntry(ObjectDatabaseEntry entry)
{
  for (std::vector<ObjectDatabaseEntry>::iterator it = m_entries.begin(); it != m_entries.end(); )
  {
    if( (*it).m_id == entry.m_id )
      it  = m_entries.erase(it);
    else
      it++;
  }
}
void ObjectDatabase::removeEntry(ObjectDatabaseEntry * entry)
{
  for (std::vector<ObjectDatabaseEntry>::iterator it = m_entries.begin(); it != m_entries.end(); )
  {
    if ( &(*it)  == entry ) 
      it = m_entries.erase(it);
    else
      it++;
  }
}

bool ObjectDatabase::load()
{
  tinyxml2::XMLDocument document;
  tinyxml2::XMLError result = document.LoadFile(m_path.c_str());

  if (result != tinyxml2::XML_SUCCESS)
  {
    Engine::getLog()->log(LF_Editor, "ObjectDatabase", "couldn't load scene file");

    return false;
  }

  tinyxml2::XMLNode * root = document.FirstChild();

  if (root != nullptr)
  {
    Engine::getLog()->log(LF_Editor, "ObjectDatabase", "loading: ", m_path);
    tinyxml2::XMLElement * child = root->FirstChildElement();

    while (child != nullptr)
    {
      std::string type = std::string(child->Name());
      Engine::getLog()->log(LF_Editor, "ObjectDatabase", "element read:", type);
      if (type == "ObjectDatabaseEntry")
      {
        ObjectDatabaseEntry entry;

        entry.m_id = child->UnsignedAttribute("id");
        entry.m_name = std::string(child->Attribute("name"));
        entry.m_path = std::string(child->Attribute("path"));
        entry.m_icon_path = std::string(child->Attribute("icon_path"));
        entry.m_description = std::string(child->Attribute("description"));
        
        addExistingEntry(entry);
      }

      child = child->NextSiblingElement();
     }
  }
  return true;
}

bool ObjectDatabase::save()
{
  tinyxml2::XMLDocument document;
  tinyxml2::XMLNode * root = document.NewElement("ObjectDatabase");
  document.InsertFirstChild(root);

  for (std::vector<ObjectDatabaseEntry>::iterator it = m_entries.begin(); it != m_entries.end(); it++)
  {
    tinyxml2::XMLElement * element_object = document.NewElement("ObjectDatabaseEntry");
    root->InsertEndChild(element_object);
    element_object->SetAttribute("id", (*it).m_id);
    element_object->SetAttribute("name", (*it).m_name.c_str() );
    element_object->SetAttribute("path", (*it).m_path.c_str());
    element_object->SetAttribute("icon_path", (*it).m_icon_path.c_str());
    element_object->SetAttribute("description", (*it).m_description.c_str());
  } 

  tinyxml2::XMLError eResult = document.SaveFile(m_path.c_str());

  return true;
}

std::vector<ObjectDatabaseEntry> ObjectDatabase::getEntries()
{
  return m_entries;
}