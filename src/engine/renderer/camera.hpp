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

        virtual void update();
        virtual void update_position(float delta);
        void update_angles(float delta);
        void rotate();

        const glm::vec3& get_position() const;
        void set_position(const glm::vec3& p);

        const glm::vec3& get_direction() const;
        void set_direction(const glm::vec3& d);

        const glm::vec3& get_walking_direction() const;
        void set_walking_direction(const glm::vec3& wd);

        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;
        int get_window_height() const;
        int get_window_width() const;
    protected:
        float get_time_delta();
        void reset_mouse();
        void check_inputs();

        Uint32 t;

        glm::vec3 position;
        double vertical_angle;
        double horizontal_angle;
        int mouse_pos_x, mouse_pos_y;
        Uint32 mouse_state;

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
