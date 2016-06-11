#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum 
{ 
  FRUSTRUM_NO_INTERSECTION,
  FRUSTRUM_INTERSECTION,
  FRUSTRUM_INCLUTION 
};

class Plane;
class Object;
class BoundingBox;

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

    // returns the direction of the camera
    glm::vec3 getDirection();

    // returns the angle around the up axis
    float getAngleAroundY();

    // returns the fov
    float getFOV();

    // rotates the camera
    void rotate(float deltax, float deltay);

    // moves the camera by step
    void move(glm::vec3 step);

    // recalculates the view, view_wt and projection matrix
    void recalculateMatrices();

    // updates the camera / key movement
    void update(float delta_time);

    // rotation setters
    void setRotationAllowed();
    void setRotationNotAllowed();

    // getter setter for exposure
    float getExposure();
    void setExposure(float value);

    // returns specifier according to  obj|bb - frustrum intersection
    unsigned int insideFrustrum(Object * obj);
    unsigned int insideFrustrum(BoundingBox * bb);
   

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

    // movement and rotation speed 
    float m_speed;
    float m_rotation_speed;

    // true if camera is allowed to rotate with mouse
    bool m_rotation_allowed;

    // holds the exposure for the camera
    float m_exposure;

    // the heights and widths of the near and far plane
    float m_near_plane_height;
    float m_near_plane_width;
    float m_far_plane_height;
    float m_far_plane_width;

    // the planes of the frustrum
    Plane * m_plane_top;
    Plane * m_plane_bottom;
    Plane * m_plane_left;
    Plane * m_plane_right;
    Plane * m_plane_front;
    Plane * m_plane_back;

    // recalculates the frustrum planes
    void recalculatePlanes();

};

#endif // CAMERA_H