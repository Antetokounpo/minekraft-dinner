#include "renderer.hpp"

#include<vector>
#include<glm/gtc/matrix_transform.hpp>

#include<engine/loaders/objloader.hpp>
#include<terrain/block.hpp>

#include<iostream> // debug

Renderer::Renderer(SDL_Window* win) : camera(win)
{}

Renderer::~Renderer(){}

void Renderer::render_chunk(Chunk chunk)
{
    model.start();
    for(int i = 0; i<16; ++i)
        for(int j = 0; j<64; ++j) // temp pour le lag
            for(int k = 0; k<16; ++k)
            {
                int block_id = chunk.get_block(i, j, k);
                if(block_id == 0)
                    continue;
                texture_manager.start_texture(block_id-1);
                object_position = {i, j, k};
                glDrawElements(GL_TRIANGLES, model.get_vertex_count(), GL_UNSIGNED_INT, 0);
                shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), object_position),  "model");
                texture_manager.stop_texture();
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
