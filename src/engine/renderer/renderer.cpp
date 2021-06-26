#include "renderer.hpp"

#include<algorithm>
#include<vector>
#include<tuple>
#include<glm/gtc/matrix_transform.hpp>

#include<engine/loaders/objloader.hpp>
#include<terrain/block.hpp>
#include<terrain/chunk.hpp>

Renderer::Renderer(SDL_Window* win) : camera(win)
{
    render_distance = 5;
}

Renderer::~Renderer(){}

void Renderer::render_chunk(const Chunk& chunk)
{
    chunk.start();
    texture.start();
    const auto& [u, v] = chunk.get_position();

    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), {u*16, 0, v*16}), "model");
    glDrawElements(GL_TRIANGLES, chunk.get_vertex_count(), GL_UNSIGNED_INT, 0);

    chunk.stop();
    texture.stop();
}

void Renderer::render()
{
    model.start();
    texture_manager.start_texture(0);
    
    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), {0, 0, 0}),  "model");
    glDrawElements(GL_TRIANGLES, model.get_vertex_count(), GL_UNSIGNED_INT, 0);

    model.stop();
    texture_manager.stop_texture();
}

void Renderer::render_terrain(Terrain& terrain)
{
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

    bool generated = false;
    for(auto& [u, v] : chunks_to_render)
    {
        std::vector<Face> blocks_to_render = terrain.get_visible_faces(u, v);
        if(!terrain.is_chunk(u, v)) generated = true;
        Chunk& chunk = terrain.get_chunk(u, v);

        if(generated) // 1 seule génération par call
            break;
        render_chunk(chunk);
    }
}

void Renderer::update()
{
    camera.update();
    shader.set_uniform_variable(camera.get_view_matrix(), "view");
    shader.set_uniform_variable(camera.get_projection_matrix(), "projection");
}

void Renderer::load_model(const std::string& filename)
{
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<unsigned int> indices;

    OBJLoader::load_file(filename, vertices, uvs, normals, indices);
    model.load(&vertices[0], vertices.size()*sizeof(vertices[0]), &uvs[0], uvs.size()*sizeof(uvs[0]), &normals[0], normals.size()*sizeof(normals[0]), &indices[0], indices.size()*sizeof(indices[0]));
}

void Renderer::load_texture(const std::string& filename)
{
    texture.load(filename);
}

void Renderer::load_shader(const std::string& vertex_filename, const std::string& fragment_filename)
{
    shader.stop();
    shader.load(vertex_filename, fragment_filename);
    shader.start();
}

const glm::vec3& Renderer::get_position() const
{
    return camera.get_position();
}

glm::mat4 Renderer::get_view_matrix() const
{
    return camera.get_view_matrix();
}

glm::mat4 Renderer::get_projection_matrix() const
{
    return camera.get_projection_matrix();
}
