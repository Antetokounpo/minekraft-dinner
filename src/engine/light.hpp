#pragma once

#include<string>

#include<GL/glew.h>
#include<glm/glm.hpp>

class Light
{
    public:
        Light();
        ~Light();

        void set_position(const glm::vec3& position);
        void set_color(const glm::vec3& color);
        void set_strength(const float& strength);

        glm::vec3& get_position();
        glm::vec3& get_color();
        float& get_strength();
    private:
        glm::vec3 light_position;
        glm::vec3 light_color;
        float light_strength;
};
