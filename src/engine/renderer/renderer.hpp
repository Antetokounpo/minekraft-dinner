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
#include<engine/skybox.hpp>

#include "camera.hpp"
#include "texture_manager.hpp"

class Renderer
{
    public:
        Renderer(SDL_Window* win);
        ~Renderer();

        void update();

        void render_terrain(Terrain& terrain);
        void render_chunk(const Chunk& chunk);
        void render_transparent_chunk(const Chunk& chunk);
        void render_world(World& world);
        void render_face(Face f, const Chunk& chunk);
        void render_hud();
        void render_skybox(Skybox& skybox);

        void load_shader(const std::string& vertex_filename, const std::string& fragment_filename);
        void load_skybox_shader(const std::string& vertex_filename, const std::string& fragment_filename);
        void load_texture(const std::string& filename);

        const glm::vec3& get_position() const;
        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;
    protected:
        Camera camera;
        int render_distance;

        Texture texture;
        Shader shader;
        Shader skybox_shader;
};
