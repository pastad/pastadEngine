#ifndef CAMERA_H
#define CAMERA_H

#include <string>

#define GLM_FORCE_RADIANS
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

    // returns the directions of the camera
    glm::vec3 getDirection();
    glm::vec3 getRight();
    glm::vec3 getUp();

    // returns the angle around the up axis (Yaw)
    float getYaw();

    // returns the pitch of the camera
    float getPitch();

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
   
    // applies physics drop 
    void applyPhysicsDrop(float offset);
    void dontApplyPhysicsDrop();
    bool isPhysicsApplied();
    void applyDrop(glm::vec3 delta);

    // up down translation allowance
    void allowUpDownTranslation();
    void dontAllowUpDownTranslation();
    bool isUpDownTranslationAllowed();

    // returns the bottom offset of the camera
    float getBottomOffset();

    // getter setter surrounding offset for collision purposes | if less distance than this at bottom_offset /2 don't step forward
    float getSurrouningOffset();
    void setSurroundingOffset(float val);

    // fall vector getter setter
    glm::vec3 getFallVector();
    void setFallVector(glm::vec3 val);

    // registers the moved callback function
    void registerMovedCallback( void  (*callback)()   );

 
private:

    // orientation and position vectors
    glm::vec3 m_pos;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;

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

    // true if camera is allowed to move up/down in Y 
    bool m_vertical_movement_allowed;

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

    // the offset for falling, colliding and if falling is applied
    float m_bottom_offset;
    float m_surrounding_offset;
    bool m_apply_physics;
    glm::vec3 m_fall_vector;

    // stores the callback function for movement
    void (*external_cameraMovedCallback)();

       // recalculates the frustrum planes
    void recalculatePlanes();

    // moves in the direction and additionally checks collision
    void movementWithCollisionCheck(glm::vec3 dir, float step);

};

#endif // CAMERA_H