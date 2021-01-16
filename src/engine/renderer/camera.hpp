#pragma once

#include<string>

#include<GL/glew.h>
#include<glm/glm.hpp>
#include<SDL2/SDL.h>

class Camera
{
    public:
        Camera(SDL_Window* win);
        ~Camera();

        void update();
        void rotate();

        glm::vec3& get_position();

        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix();
    private:
        void reset_mouse();

        Uint32 t;

        glm::vec3 position;
        double vertical_angle;
        double horizontal_angle;

        glm::vec3 direction;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 walking_direction;

        float speed;
        float fov;
        float sensitivity;

        SDL_Window* window;
        int width;
        int height;
};
