#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glm/glm.hpp>
#include "Engine.h"

class Texture
{
    public:
        Texture(std::string path);
        Texture(glm::vec4 color);
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        // loads the texture
        bool load();

        // binds the texure to the texture unit
        void bind(unsigned int tex);
    protected:

    private:
        
        // the width and height of the texture
        int m_width;
        int m_height;

        // the texture path
        std::string m_path;

        // gl handle
        GLuint m_texture;

        // indicates loading state
        bool m_loaded;
};

#endif // TEXTURE_H