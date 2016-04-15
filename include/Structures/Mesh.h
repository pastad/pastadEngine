#ifndef MESH_H
#define MESH_H

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Engine.h"
#include <glm/glm.hpp>
#include <vector>

class IndexedRepresentation;

class Mesh
{
    public:
        Mesh(const aiMesh*mesh, int mat_index);
        ~Mesh();

        Mesh(const Mesh& other) = delete;
        Mesh& operator=(const Mesh& other)= delete ;

        // renders the mesh 
        void render(unsigned int num_indices) ;

        // returns the material index
        int getMaterialIndex();

        // buffers the matrices to a gml buffer
        void bufferModelMatrices(std::vector<glm::mat4> * matrices);

    protected:

    private:

        // enum specifying the vb structures
        enum
        {
            POSITION_VB,
            TEXCOORD_VB,
            NORMAL_VB,
            INDEX_VB,
            MODEL_VB,
            NUM_BUFFERS
        };

        //  the vao and vbo
        GLuint m_vertex_array_object;
        GLuint m_vertex_array_buffers[NUM_BUFFERS];

        // holds the material index        
        int m_mat_index;

        // holds the draw count
        unsigned int m_draw_count = 0;

        // inits the mesh
        void initMesh(const  IndexedRepresentation& model);
};

#endif // MESH_H