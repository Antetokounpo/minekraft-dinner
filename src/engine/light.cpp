#include "light.hpp"

#include<string>

#include<GL/glew.h>
#include<glm/glm.hpp>

Light::Light(){}

Light::~Light(){}

void Light::set_position(const glm::vec3& position)
{
    light_position = position;
}

void Light::set_color(const glm::vec3& color)
{
    light_color = color;
}

void Light::set_strength(const float& strength)
{
    light_strength = strength;
}

glm::vec3& Light::get_position()
{
    return light_position;
}

glm::vec3& Light::get_color()
{
    return light_color;
}

float& Light::get_strength()
{
    return light_strength;
}
