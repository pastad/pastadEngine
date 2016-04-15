#include "IndexedRepresentation.h"

// gets the number of elements of the array
#define ELEMENTS_IN_ARRAY(a) (sizeof(a)/sizeof(a[0]))

IndexedRepresentation::IndexedRepresentation()
{
}

IndexedRepresentation::~IndexedRepresentation()
{
}

void IndexedRepresentation::calcNormals()
{
    for(unsigned int i = 0; i < m_indices.size(); i += 3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        glm::vec3 v1 = m_positions[i1] - m_positions[i0];
        glm::vec3 v2 = m_positions[i2] - m_positions[i0];

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        m_normals[i0] += normal;
        m_normals[i1] += normal;
        m_normals[i2] += normal;
    }

    for(unsigned int i = 0; i < m_positions.size(); i++)
        m_normals[i] = glm::normalize(m_normals[i]);
}

void VertexBoneData::addBoneData(int id, float weight)
{
    for (int i = 0 ; i < ELEMENTS_IN_ARRAY(m_ids) ; i++)
    {
        if (m_weights[i] == 0.0f)
        {
            m_ids[i]     = id;
            m_weights[i] = weight;
            return;
        }
    }
}

int IndexedRepresentation::exists(glm::vec3 pos,glm::vec3 norm, glm::vec2 tex, int input_count )
{
    for(unsigned int i = 0; i < m_positions.size(); i ++)
    {
        if(equals(pos,m_positions[i]) && equals(norm,m_normals[i]) )
        {
            if(input_count == 3)
            {
                if(equals(m_texCoords[i],tex))
                {
                    return i;
                }
            }
            else
            {
                return i;
            }
        }
    }
    return -1;
}

bool IndexedRepresentation::equals(glm::vec3 v1, glm::vec3 v2)
{
    return (equals(v1.x,v2.x) && equals(v1.y,v2.y) &&equals(v1.z,v2.z));
}
bool IndexedRepresentation::equals(glm::vec2 v1, glm::vec2 v2)
{
    return (equals(v1.x,v2.x) && equals(v1.y,v2.y)) ;
}
bool IndexedRepresentation::equals(float f1, float f2)
{
    return ( abs(f1 - f2) <0.01f);
}