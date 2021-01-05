#version 330 core

layout(location=0) in vec3 positions;
layout(location=1) in vec2 in_uvs;
layout(location=2) in vec3 in_normals;

out vec2 uvs;
out vec3 normals;
out vec3 world_positions;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    world_positions = (model * vec4(positions, 1.0)).xyz;
    gl_Position = projection * view * vec4(world_positions, 1.0);
    uvs = in_uvs;
    normals = in_normals;
}