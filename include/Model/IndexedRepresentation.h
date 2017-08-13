#ifndef INDEXEDREPRESENTATION_H
#define INDEXEDREPRESENTATION_H

#include <vector>
#include <glm/glm.hpp>

// class for loading purposes


// the vertex bone data structure
struct VertexBoneData
{
  int m_ids[4];
  float m_weights[4];

  VertexBoneData()
  {
    m_weights[0]=m_weights[1]=m_weights[2]=m_weights[3]=0;
    m_ids[0]=m_ids[1]=m_ids[2]=m_ids[3]=0;
  };

  void addBoneData(int id, float weight);
};

// the bone info structure
struct BoneInfo
{
  glm::mat4 m_offset =  glm::mat4();
  glm::mat4 m_transformation= glm::mat4();

  BoneInfo()
  {
    for(int x=0; x<4; x++)
    {
      for(int y=0; y<4; y++)
      {
       //m_offset[x][y] = 0;
      // m_transformation[x][y] = 0;
      }
    }

  }
};

class IndexedRepresentation
{
  public:
    IndexedRepresentation();
    ~IndexedRepresentation();

    IndexedRepresentation(const IndexedRepresentation& other);
    IndexedRepresentation& operator=(const IndexedRepresentation& other);
    

    // the positions of the representation
    std::vector<glm::vec3> m_positions;

    // the tex coords of the representation
    std::vector<glm::vec2> m_texCoords;

    // the nomrals of the representation
    std::vector<glm::vec3> m_normals;

    // the tangents of the representation
    std::vector<glm::vec3> m_tangents;

    // the bitangets of the representation
    std::vector<glm::vec3> m_bitangents;

    // the indices of the representation
    std::vector<unsigned int> m_indices;

    // the bones for the anim of the representation
    std::vector<VertexBoneData> m_bones;

    // calculates the normals
    void calcNormals();

    // checks if the vertex exists
    int exists(glm::vec3 pos,glm::vec3 norm, glm::vec2 tex, int input_count );

protected:
private:
    
  // comparison functions that accept a small error
  bool equals(glm::vec3 v1, glm::vec3 v2);
  bool equals(glm::vec2 v1, glm::vec2 v2);
  bool equals(float f1, float f2);
};

#endif // INDEXEDREPRESENTATION_H
