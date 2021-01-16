#include "renderer.hpp"

#include<algorithm>
#include<vector>
#include<tuple>
#include<glm/gtc/matrix_transform.hpp>

#include<engine/loaders/objloader.hpp>
#include<terrain/block.hpp>

Renderer::Renderer(SDL_Window* win) : camera(win)
{
    render_distance = 3;
}

Renderer::~Renderer(){}

void Renderer::render_chunk(const std::vector<std::tuple<int, int, int>>& blocks, const Chunk& chunk)
{
    for(auto& block : blocks)
    {
        auto&[i, j, k] = block;
        int block_id = chunk.get_block(i, j, k);

        const auto& [u, v] = chunk.get_position();

        glm::vec3 object_position = {u*16+i, j, v*16+k};

        if(block_id - 1 != texture_manager.get_active_texture())
        {
            texture_manager.stop_texture();
            texture_manager.start_texture(block_id-1);
        }

        shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), object_position), "model");
        glDrawElements(GL_TRIANGLES, model.get_vertex_count(), GL_UNSIGNED_INT, 0);
    }
}

void Renderer::render_terrain(Terrain& terrain)
{
    model.start();
    int r = camera.get_position()[0]/16;
    int s = camera.get_position()[2]/16;
    std::vector<std::tuple<int, int>> chunks_to_render;

    for(int i = -render_distance; i<render_distance+1; ++i)
    {
        for(int j = -render_distance; j<render_distance+1; ++j)
        {
            chunks_to_render.push_back({r+i, s+j});
        }
    }

    for(auto& [u, v] : chunks_to_render)
    {
        std::vector<std::tuple<int, int, int>> blocks_to_render = terrain.get_visible_blocks(u, v);
        const Chunk& chunk = terrain.get_chunk(u, v);
        render_chunk(blocks_to_render, chunk);
    }

    model.stop();
}

void Renderer::update()
{
    camera.update();
    shader.set_uniform_variable(camera.get_view_matrix(), "view");
    shader.set_uniform_variable(camera.get_projection_matrix(), "projection");
    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), object_position),  "model");
}

void Renderer::load_model(const std::string& filename)
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<int> indices;


    OBJLoader::load_file(filename, vertices, uvs, normals, indices);
    model.load(&vertices[0], vertices.size()*sizeof(vertices[0]), &uvs[0], uvs.size()*sizeof(uvs[0]), &normals[0], normals.size()*sizeof(normals[0]), &indices[0], indices.size()*sizeof(indices[0]));
}

void Renderer::load_shader(const std::string& vertex_filename, const std::string& fragment_filename)
{
    shader.stop();
    shader.load(vertex_filename, fragment_filename);
    shader.start();
}

void Renderer::init_textures()
{
    for(const auto& b : BLOCK_TYPES)
    {
        texture_manager.load_texture(b.texture_path);
    }
}

glm::vec3& Renderer::get_position()
{
    return camera.get_position();
}

glm::mat4 Renderer::get_view_matrix()
{
    return camera.get_view_matrix();
}

glm::mat4 Renderer::get_projection_matrix()
{
    return camera.get_projection_matrix();
}
