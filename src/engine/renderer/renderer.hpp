#pragma once

#include<string>
#include<vector>
#include<SDL2/SDL.h>

#include<engine/model.hpp>
#include<engine/texture.hpp>
#include<engine/shader.hpp>
#include<terrain/chunk.hpp>
#include<terrain/terrain.hpp>
#include<game/world.hpp>

#include "camera.hpp"
#include "texture_manager.hpp"

class Renderer
{
    public:
        Renderer(SDL_Window* win);
        ~Renderer();

        void update();
        void render();
        void render_terrain(Terrain& terrain);
        void render_chunk(const Chunk& chunk);
        void render_world(World& world);

        void load_model(const std::string& filename);
        void load_shader(const std::string& vertex_filename, const std::string& fragment_filename);
        void load_texture(const std::string& filename);

        const glm::vec3& get_position() const;
        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;
    protected:
        Camera camera;
        int render_distance;

        Model model;
        Texture texture;
        TextureManager texture_manager;
        Shader shader;
};
