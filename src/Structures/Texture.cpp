#include "Texture.h"
#include <SOIL/SOIL.h>
#include "RenderSubsystem.h"
#include "RenderShader.h"

#include "Log.h"

Texture::Texture(std::string path):m_path(path),m_loaded(false)
{
}
Texture::~Texture()
{
}
bool Texture::load()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* image = SOIL_load_image(m_path.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGB);
    if(image == nullptr )
    {
        Engine::getLog()->log("Texture", "loading failed");
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_loaded = true;

    return true;
}
void Texture::bind(unsigned int unit, RenderShader * render_shader)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}