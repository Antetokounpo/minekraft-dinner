#pragma once

#include<string>
#include<vector>
#include<SDL2/SDL.h>

#include<engine/model.hpp>
#include<engine/texture.hpp>
#include<engine/shader.hpp>
#include<terrain/chunk.hpp>

#include "camera.hpp"
#include "texture_manager.hpp"

class Renderer
{
    public:
        Renderer(SDL_Window* win);
        ~Renderer();

        void update();
        void render_chunk(Chunk chunk);

        void load_model(const std::string& filename);
        void load_shader(const std::string& vertex_filename, const std::string& fragment_filename);
        void init_textures();

        glm::vec3& get_position();
        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix();
    protected:
        Camera camera;
        glm::vec3 object_position;

        Model model;
        TextureManager texture_manager;
        Shader shader;
};
