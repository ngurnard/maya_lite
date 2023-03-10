#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}


void Camera::RecomputeAttributes()
{
    // Transoformation relative to original cartesian directions!
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(theta), glm::vec3(0, 1, 0))
                  * glm::rotate(glm::mat4(1.0f), glm::radians(phi), glm::vec3(1, 0, 0));
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom));

    eye = glm::vec3(rot * trans * glm::vec4(0, 0, 0, 1));
    right = glm::vec3(rot * glm::vec4(1, 0, 0, 0));
    up = glm::vec3(rot * glm::vec4(0, 1, 0, 0));
    look = glm::vec3(rot * glm::vec4(0, 0, 1, 0));

    float tan_fovy = tan(glm::radians(fovy/2));
    float len = glm::length(ref - eye);
    aspect = width / static_cast<float>(height);
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(glm::radians(fovy), width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

void Camera::RotateAboutUp(float deg)
{
    this->theta += deg;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    this->phi += deg;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    this->zoom -= amt; // minus because look is along -z
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}
