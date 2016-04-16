#ifndef LIGHT_H
#define LIGHT_H

#include "gl_core_4_.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>



// class to represent a light (direcitonal, point or spot)

enum
{
    LIGHT_DIRECTIONAL,
    LIGHT_POINT,
    LIGHT_SPOT
};


class Light
{
    public:
        Light();
        ~Light();
        Light(const Light& other) = delete;
        Light& operator=(const Light& other) = delete;

        // light setters        
        void setDirectional(glm::vec3 direction, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity);
        void setPoint(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                      float con ,float lin, float qua );
        void setSpot(glm::vec3 position, glm::vec3 col_am ,glm::vec3 col_dif, glm::vec3 spec,float intensity,
                      float con ,float lin, float qua, float cutoffAngle, glm::vec3 direction );


        //returns the type of the light
        unsigned int getType();
        
        // returns the position or direction of the light
        glm::vec3 getDirection();
        glm::vec3 getPosition();

        // returns the individual color
        glm::vec3 getAmbientColor();
        glm::vec3 getDiffuseColor();
        glm::vec3 getSpecularColor();

        // returns attenuation specs
        float getIntensity();
        float getAttenuationConstant();
        float getAttenuationLinear();
        float getAttenuationQuadratic();

        // returns the cutoff angle in case of spot light 
        float getCutoffAngle();


    protected:

    private:

        // the ligth type
        unsigned int m_type;

        // position and direction
        glm::vec3 m_position;
        glm::vec3 m_direction;

        // light colors
        glm::vec3 m_color_ambient;
        glm::vec3 m_color_diffuse;
        glm::vec3 m_color_specular;

        // attenuation specs
        float m_att_const;
        float m_att_linear;
        float m_att_quadratic;
        float m_intensity;

        // cuttoff angle
        float m_cutoff_angle;
};



#endif // LIGHT_H
