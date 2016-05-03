#ifndef TEXTURE_H
#define TEXTURE_H

#define TEXTURE_JITTER 19
#define TEXTURE_SHADOW_START 20
#define TEXTURE_POINT_SHADOW_START 40 


#include <string>

#include <glm/glm.hpp>
#include "Engine.h"


// class represents a texture 

class RenderShader;

class Texture
{
    public:
        Texture(std::string path);
        Texture();
        Texture(glm::vec4 color);
        ~Texture();

        Texture(const Texture& other) = delete;
        Texture& operator=(const Texture& other) = delete;

        // loads the texture
        bool load();

        // binds the texure to the texture unit
        void bind(unsigned int tex);

        // creates a texture
        void create(int unit,GLenum format, int width , int height);

        // binds texture to the framebuffer
        void bindToFramebuffer(GLenum attachement);

        // clears the texture | framebuffer must be bound
        void clear(int unit, unsigned int depth);

        // getters for width and height
        glm::vec2 getSize();

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