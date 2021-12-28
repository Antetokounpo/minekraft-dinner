#include "camera.hpp"

#include<string>
#include<cmath>
#ifndef M_PI
    #define M_PI acos(-1)
    #define M_PI_2 M_PI/2
#endif

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<SDL2/SDL.h>

Camera::Camera(SDL_Window* win)
{
    window = win;

    fov = 90.0f; // Field of view

    position = glm::vec3(0.0f, 0.0f, 0.0f); // set initial position
    // Si on set pas ces vecteurs, ça peut donner des nan quand on calcule les angles
    // et donc rien ne render
    direction = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    walking_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    speed = 0.015f; // speed at which the camera moves

    vertical_angle = 0.0f;
    horizontal_angle = 0.0f;
    sensitivity = 0.0003f; // speed at which the camera rotates

    // Get window size
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    width = w;
    height = h;

    // Set time point
    t = SDL_GetTicks();
}

Camera::~Camera(){}

void Camera::reset_mouse()
{
    SDL_WarpMouseInWindow(window, width/2, height/2);
}

float Camera::get_time_delta()
{
    float delta_time = SDL_GetTicks() - t;
    t = SDL_GetTicks();

    return delta_time;
}

void Camera::check_inputs()
{
    SDL_PumpEvents();
    mouse_state = SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);
}

void Camera::update_angles(float delta)
{
    horizontal_angle += sensitivity * delta * -float(mouse_pos_x - width/2);
    vertical_angle += sensitivity * delta * -float(mouse_pos_y - height/2);
    if(abs(vertical_angle) > M_PI/2) // On ne peut pas se casser le cou
        vertical_angle = (vertical_angle/abs(vertical_angle)) * (M_PI_2 - 0.001f);
}

void Camera::update_position(float delta)
{
    float movement = delta * speed;

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W])
        position += walking_direction * movement;
    if(state[SDL_SCANCODE_S])
        position -= walking_direction * movement;
    if(state[SDL_SCANCODE_D])
        position += right * movement;
    if(state[SDL_SCANCODE_A])
        position -= right * movement;
    if(state[SDL_SCANCODE_SPACE])
        position += up * movement;
    if(state[SDL_SCANCODE_LCTRL])
        position -= up * movement;
}

void Camera::update()
{
    if(!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS))
        return;

    float delta_time = get_time_delta();

    check_inputs();

    update_angles(delta_time);
    rotate();
    update_position(delta_time);

    reset_mouse();
}

void Camera::rotate()
{
    direction = glm::vec3(cos(vertical_angle) * sin(horizontal_angle),
                          sin(vertical_angle),
                          cos(vertical_angle) * cos(horizontal_angle));

    right = glm::vec3(sin(horizontal_angle - M_PI/2.0f),
                      0.0f,
                      cos(horizontal_angle - M_PI/2.0f));

    walking_direction = glm::vec3(sin(horizontal_angle),
                                  0.0f,
                                  cos(horizontal_angle));
}

const glm::vec3& Camera::get_position() const
{
    return position;
}

void Camera::set_position(const glm::vec3& p)
{
    position = p;
}

const glm::vec3& Camera::get_direction() const
{
    return direction;
}

void Camera::set_direction(const glm::vec3& d)
{
    direction = d;
}

const glm::vec3& Camera::get_walking_direction() const
{
    return  walking_direction;
}

void Camera::set_walking_direction(const glm::vec3& wd)
{
    walking_direction = wd;
}

glm::mat4 Camera::get_view_matrix() const
{
    return glm::lookAt(position, position+direction, up);
}

glm::mat4 Camera::get_projection_matrix() const
{
    return glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 1000.0f); // FOV matrix
}

int Camera::get_window_height() const
{
    return height;
}

int Camera::get_window_width() const
{
    return width;
}
