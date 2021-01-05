#include "camera.hpp"

#include<string>
#include<cmath>

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<SDL2/SDL.h>

#include<iostream> // debug

Camera::Camera(SDL_Window* win)
{
    window = win;

    fov = 90.0f; // Field of view

    position = glm::vec3(0.0f); // set initial position
    // Si on set pas ces vecteurs, ça peut donner des nan quand on calcule les angles
    // et donc rien ne render
    direction = glm::vec3(1.0f, 0.0f, 0.0f);
    right = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    speed = 0.015f; // speed at which the camera moves

    vertical_angle = 0.0f;
    horizontal_angle = 0.0f;
    sensitivity = 0.0005f; // speed at which the camera rotates

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

void Camera::update()
{
    if(!(SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS))
        return;

    float delta_time = (SDL_GetTicks() - t);
    float movement = delta_time * speed;

    SDL_PumpEvents();

    int xpos, ypos;
    SDL_GetMouseState(&xpos, &ypos);

    horizontal_angle += sensitivity * delta_time * -float(xpos - width/2);
    vertical_angle += sensitivity * delta_time * -float(ypos - height/2);
    if(abs(vertical_angle) > M_PI/2) // On ne peut pas se casser le cou
        vertical_angle = (vertical_angle/abs(vertical_angle)) * (M_PI_2 - 0.001f);
    rotate();

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_W])
        position += direction * movement;
    if(state[SDL_SCANCODE_S])
        position -= direction * movement;
    if(state[SDL_SCANCODE_D])
        position += right * movement;
    if(state[SDL_SCANCODE_A])
        position -= right * movement;
    if(state[SDL_SCANCODE_SPACE])
        position += up * movement;
    if(state[SDL_SCANCODE_LCTRL])
        position -= up * movement;

    reset_mouse();
    t = SDL_GetTicks();
}

void Camera::rotate()
{
    direction = glm::vec3(cos(vertical_angle) * sin(horizontal_angle),
                          sin(vertical_angle),
                          cos(vertical_angle) * cos(horizontal_angle));

    right = glm::vec3(sin(horizontal_angle - M_PI/2.0f),
                      0.0f,
                      cos(horizontal_angle - M_PI/2.0f));
}

glm::vec3& Camera::get_position()
{
    return position;
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(position, position+direction, up);
}

glm::mat4 Camera::get_projection_matrix()
{
    return glm::perspective(glm::radians(fov), (float)width/(float)height, 0.1f, 100.0f); // FOV matrix
}
