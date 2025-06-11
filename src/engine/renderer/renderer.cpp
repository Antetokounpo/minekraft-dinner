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
    render_distance = 12;
}

Renderer::~Renderer(){}

void Renderer::render_chunk(const Chunk& chunk)
{
    const auto& [u, v] = chunk.get_position();

    chunk.start();
    texture.start();

    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), {u*16, 0, v*16}), "model");
    glDrawElements(GL_TRIANGLES, chunk.get_vertex_count(), GL_UNSIGNED_INT, 0);

    chunk.stop();
    texture.stop();
}

void Renderer::render_transparent_chunk(const Chunk &chunk)
{
    const auto& [u, v] = chunk.get_position();

    chunk.start_transparent();
    texture.start();

    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), {u*16, 0, v*16}), "model");
    glDrawElements(GL_TRIANGLES, chunk.get_transparent_vertex_count(), GL_UNSIGNED_INT, 0);

    chunk.stop_transparent();
    texture.stop();
} 

void Renderer::render()
{
    model.start();
    texture.start();
    
    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), {0, 0, 0}),  "model");
    glDrawElements(GL_TRIANGLES, model.get_vertex_count(), GL_UNSIGNED_INT, 0);

    model.stop();
    texture.stop();
}

void Renderer::render_terrain(Terrain& terrain)
{
    int r = floor(camera.get_position()[0]/16);
    int s = floor(camera.get_position()[2]/16);
    std::vector<std::tuple<int, int>> chunks_to_render;

    for(int i = -render_distance; i<render_distance+1; ++i)
    {
        for(int j = -render_distance; j<render_distance+1; ++j)
        {
            chunks_to_render.push_back({r+i, s+j});
        }
    }

    // L'ordre des chunks doit être du plus loin au plus proche pour que l'effet de transparence soit réaliste
    std::sort(chunks_to_render.begin(), chunks_to_render.end(), [](const std::tuple<int, int>& a, const std::tuple<int, int>& b)
    {
        auto da = pow(std::get<0>(a), 2) + pow(std::get<1>(a), 2);
        auto db = pow(std::get<0>(b), 2) + pow(std::get<1>(b), 2);

        return da > db;
    });

    bool generated = false;
    bool visible_faces = false;
    for(auto& [u, v] : chunks_to_render)
    {
       if(!generated || terrain.is_chunk(u, v))
        {
            if(!terrain.is_chunk(u, v)) generated = true;
            Chunk& chunk = terrain.get_chunk(u, v);

            if(!visible_faces || chunk.is_visible_faces()) // 1 seule génération par call
            {
                std::vector<Face> blocks_to_render = terrain.get_visible_faces(u, v);
                if(!chunk.is_visible_faces()) visible_faces = true;
                render_chunk(chunk);
            }
        }
    }

    for(auto& [u, v] : chunks_to_render)
    {
       if(!generated || terrain.is_chunk(u, v))
        {
            if(!terrain.is_chunk(u, v)) generated = true;
            Chunk& chunk = terrain.get_chunk(u, v);

            if(!visible_faces || chunk.is_visible_faces()) // 1 seule génération par call
            {
                std::vector<Face> blocks_to_render = terrain.get_visible_faces(u, v);
                if(!chunk.is_visible_faces()) visible_faces = true;
                render_transparent_chunk(chunk);
            }
        }
    }
}

void Renderer::render_world(World& world)
{
    shader.start();
    camera = world.get_player();

    update();

    render_terrain(world.get_terrain());
    if(world.get_player().is_looking_at_face())
        render_face(world.get_player().get_looking_face(), world.get_terrain().get_chunk_of_block(world.get_player().get_looking_block()));
    
    shader.stop();

    skybox_shader.start();
    render_skybox(world.get_skybox());
    skybox_shader.stop();

    shader.start();
    render_hud();
    shader.stop();
}

void Renderer::render_face(Face f, const Chunk& chunk)
{
    const auto& [x, y, z, o, t] = f;
    const auto& [u, v] = chunk.get_position();
    auto face_vertices = faces[o];

    /* Petit offset qui va placer la face "highlit" un peu par-dessus la face qu'on vise réellement pour qu'on voit la texture du bloc
       au travers du highlight. Ce vecteur est additionner à la translation plus tard.
    */
    glm::vec3 highlight_offset = {0, 0, 0};
    switch (o)
    {
    case SUD:
        highlight_offset = {0.0, 0.0, -1.0f};
        break;
    case EST:
        highlight_offset = {1.0f, 0.0, 0.0};
        break;
    case NORD:
        highlight_offset = {0.0, 0.0, 1.0f};
        break;
    case OUEST:
        highlight_offset = {-1.0f, 0.0f, 0.0f};
        break;
    case DESSUS:
        highlight_offset = {0.0f, 1.0f, 0.0f};
        break;
    case DESSOUS:
        highlight_offset = {0.0f, -1.0f, 0.0f};
        break;
    default:
        break;
    }
    highlight_offset *= 0.01f;

    std::vector<float> uvs = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
    };

    std::vector<float> normals = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    std::array<unsigned, 6> indices = {
        0, 1, 2,
        1, 0, 3
    };

    Model face_model;
    face_model.load(&face_vertices[0], face_vertices.size()*sizeof(face_vertices[0]), 
                    &uvs[0], uvs.size()*sizeof(uvs[0]),
                    &normals[0], normals.size()*sizeof(normals[0]),
                    &indices[0], indices.size()*sizeof(indices[0]));
    Texture face_texture;
    face_texture.set_texture_color(255, 255, 255, 100);

    face_model.start();
    face_texture.start();
    
    shader.set_uniform_variable(glm::translate(glm::mat4(1.0f), glm::vec3({(int)x+u*16, y, (int)z+v*16})+highlight_offset),  "model");
    glDrawElements(GL_TRIANGLES, face_model.get_vertex_count(), GL_UNSIGNED_INT, 0);

    face_model.stop();
    face_texture.stop();
}

void Renderer::render_hud()
{
    float crosshair_size = 15.0f;
    std::array<float, 12> vertices = 
        {
             crosshair_size, 0.0f,  1.0f,
            0.0f,  crosshair_size,  1.0f,
             crosshair_size,  crosshair_size,  1.0f,
            0.0f, 0.0f,  1.0f
        };

    std::vector<float> uvs = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f
    };

    std::vector<float> normals = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    std::vector<unsigned> indices = {
        0, 1, 2,
        1, 0, 3
    };

    Model hud_model;
    hud_model.load(&vertices[0], vertices.size()*sizeof(vertices[0]), 
                    &uvs[0], uvs.size()*sizeof(uvs[0]),
                    &normals[0], normals.size()*sizeof(normals[0]),
                    &indices[0], indices.size()*sizeof(indices[0]));
    Texture hud_texture;
    hud_texture.load("res/tex/crosshair.png");

    glDisable(GL_DEPTH_TEST);

    hud_model.start();
    hud_texture.start();
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), {camera.get_window_width()/2-crosshair_size/2, camera.get_window_height()/2-crosshair_size/2, 0.0f});
    glm::mat4 ortho = glm::ortho(0.0f, (float)camera.get_window_width(), (float)camera.get_window_height(), 0.0f);
    shader.set_uniform_variable(translate,  "model");
    shader.set_uniform_variable(ortho,  "view");
    shader.set_uniform_variable(glm::mat4(1.0f),  "projection");
    glDrawElements(GL_TRIANGLES, hud_model.get_vertex_count(), GL_UNSIGNED_INT, 0);
    hud_model.start();
    hud_model.stop();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::render_skybox(Skybox& skybox)
{
    skybox.start();

    glm::mat4 view_matrix = camera.get_view_matrix();
    for(int i = 0; i<3; ++i)
        view_matrix[3][i] = 0.0f;

    skybox_shader.set_uniform_variable(view_matrix, "view");
    skybox_shader.set_uniform_variable(camera.get_projection_matrix(), "projection");

    glDrawArrays(GL_TRIANGLES, 0, skybox.get_vertex_count());

    skybox.stop();
}

void Renderer::update()
{
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
}

void Renderer::load_skybox_shader(const std::string& vertex_filename, const std::string& fragment_filename)
{
    skybox_shader.stop();
    skybox_shader.load(vertex_filename, fragment_filename);
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
