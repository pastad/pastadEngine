#ifndef CAMERA_H
#define CAMERA_H

#include<string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera(float x,float y, float z);
    ~Camera();

    Camera(const Camera& other) = delete;
    Camera& operator=(const Camera& other) = delete;

    // returns the view matrix
    glm::mat4 getView();

    // returns the vie without translation (e.g. for skybox)
    glm::mat4 getViewWithoutTranslation();

    // returns the projetcion matrix
    glm::mat4 getProjection();

    //returns the current camera position
    glm::vec3 getPosition();

    // rotates the camera
    void rotate(float deltax, float deltay);

    // moves the camera by step
    void move(glm::vec3 step);

    // recalculates the view, view_wt and projection matrix
    void recalculateMatrices();

    // updates the camera / key movement
    void update();
   

private:

    // orientation and position vectors
    glm::vec3 m_pos;
    glm::vec3 m_forward;
    glm::vec3 m_up;

    // rotation of the camera  
    float m_rot_1;
    float m_rot_2;

    // view and projecion matrix
    glm::mat4 m_view;
    glm::mat4 m_view_wt;
    glm::mat4 m_projection;

    // field ov fiew
    float m_fov;    

    // movement speed 
    float m_speed;
};

#endif // CAMERA_H