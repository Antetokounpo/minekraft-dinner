#version 330 core

in vec2 uvs;
in vec3 normals;
in vec3 world_positions;

out vec4 color;

uniform sampler2D model_texture;
uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_strength;

vec3 get_ambient_light()
{
    return light_strength * light_color;
}

vec3 get_diffuse_light()
{
    return max(dot(normalize(normals), normalize(light_position - world_positions)), 0.0) * vec3(1.0, 1.0, 1.0);
}

void main()
{
    color = texture(model_texture, uvs);
}