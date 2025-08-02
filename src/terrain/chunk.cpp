#include "chunk.hpp"

#include<cstdlib>
#include<cmath>
#include<algorithm>

#include<engine/loaders/objloader.hpp>

#include "faces.hpp"


Chunk::Chunk()
{
    visible_faces = {};
    transparent_faces = {};
}

void Chunk::set_position(int i, int k)
{
    chunk_data.set_position(i, k);
}

std::tuple<int, int> Chunk::get_position() const
{
    return chunk_data.get_position();
}

void Chunk::set_chunk_data(ChunkData new_chunk_data)
{
    chunk_data = new_chunk_data;
}

unsigned Chunk::get_block(unsigned int x, unsigned int y, unsigned int z) const
{
    return chunk_data.get_block(x, y, z);
}

bool Chunk::is_block_transparent(unsigned x, unsigned y, unsigned z) const
{
    return BLOCK_TYPES[get_block(x, y, z)].transparent;
}

bool Chunk::is_block_air(unsigned x, unsigned y, unsigned z) const
{
    return get_block(x, y, z) == 0;
}

void Chunk::set_block(unsigned x, unsigned y, unsigned z, unsigned b)
{
    chunk_data.set_block(x, y, z, b);
    reset_visible_faces();
}

bool Chunk::is_generated() const
{
    return chunk_data.is_generated();
}

void Chunk::build_mesh()
{
    if(visible_faces.empty())
        return;

    const std::array<unsigned, 6> indices = {
        0, 1, 2,
        1, 0, 3
    };

    const std::array<float, 8> uvs = {
        0.0f, 0.0f,
        1.0f/(atlas_w/tex_w), 1.0f/(atlas_h/tex_h),
        0.0f, 1.0f/(atlas_h/tex_h),
        1.0f/(atlas_w/tex_w), 0.0f
    };

    const std::array<float, 12> normals = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    std::vector<float> mesh_vertices = {};
    std::vector<float> mesh_uvs = {};
    std::vector<float> mesh_normals = {};
    std::vector<unsigned> mesh_indices = {};

    unsigned cnt = 0;
    for(const auto& face : visible_faces)
    {
        const auto& [i, j, k, f, t] = face;

        std::vector<float> temp_vertices (faces[f].begin(), faces[f].end()); // On peut pas changer les valeurs de l'enum à cause de ça ici.
        for(unsigned c = 0; c<temp_vertices.size(); c += 3)
        {
            temp_vertices[c]   += i;
            temp_vertices[c+1] += j;
            temp_vertices[c+2] += k;
        }

        std::vector<float> temp_uvs (uvs.begin(), uvs.end());
        for(unsigned c = 0; c<temp_uvs.size(); c += 2)
        {
            unsigned u = t % (atlas_w/tex_w);
            temp_uvs[c] += u * 1.0f/(atlas_w/tex_w);
            u = t / (atlas_w/tex_w);
            temp_uvs[c+1] += u * 1.0f/(atlas_h/tex_h);
        }

        mesh_vertices.insert(mesh_vertices.end(), temp_vertices.begin(), temp_vertices.end());
        mesh_uvs.insert(mesh_uvs.end(), temp_uvs.begin(), temp_uvs.end());
        mesh_normals.insert(mesh_normals.end(), normals.begin(), normals.end());

        std::vector<unsigned> temp_indices(indices.begin(), indices.end());
        std::transform(temp_indices.begin(), temp_indices.end(), temp_indices.begin(), [&](auto a) { return a+4*cnt; });

        mesh_indices.insert(mesh_indices.end(), temp_indices.begin(), temp_indices.end());
        cnt += 1;
    }

    chunk_mesh.load(mesh_vertices, mesh_uvs, mesh_normals, mesh_indices);
}

void Chunk::build_transparent_mesh()
{
    if(transparent_faces.empty())
        return;

    const std::array<unsigned, 6> indices = {
        0, 1, 2,
        1, 0, 3
    };

    const std::array<float, 8> uvs = {
        0.0f, 0.0f,
        1.0f/(atlas_w/tex_w), 1.0f/(atlas_h/tex_h),
        0.0f, 1.0f/(atlas_h/tex_h),
        1.0f/(atlas_w/tex_w), 0.0f
    };

    const std::array<float, 12> normals = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    std::vector<float> mesh_vertices = {};
    std::vector<float> mesh_uvs = {};
    std::vector<float> mesh_normals = {};
    std::vector<unsigned> mesh_indices = {};

    unsigned cnt = 0;
    for(const auto& face : transparent_faces)
    {
        const auto& [i, j, k, f, t] = face;

        std::vector<float> temp_vertices (faces[f].begin(), faces[f].end()); // On peut pas changer les valeurs de l'enum à cause de ça ici.
        for(unsigned c = 0; c<temp_vertices.size(); c += 3)
        {
            temp_vertices[c]   += i;
            temp_vertices[c+1] += j;
            temp_vertices[c+2] += k;
        }

        std::vector<float> temp_uvs (uvs.begin(), uvs.end());
        for(unsigned c = 0; c<temp_uvs.size(); c += 2)
        {
            unsigned u = t % (atlas_w/tex_w);
            temp_uvs[c] += u * 1.0f/(atlas_w/tex_w);
            u = t / (atlas_w/tex_w);
            temp_uvs[c+1] += u * 1.0f/(atlas_h/tex_h);
        }

        mesh_vertices.insert(mesh_vertices.end(), temp_vertices.begin(), temp_vertices.end());
        mesh_uvs.insert(mesh_uvs.end(), temp_uvs.begin(), temp_uvs.end());
        mesh_normals.insert(mesh_normals.end(), normals.begin(), normals.end());

        std::vector<unsigned> temp_indices(indices.begin(), indices.end());
        std::transform(temp_indices.begin(), temp_indices.end(), temp_indices.begin(), [&](auto a) { return a+4*cnt; });

        mesh_indices.insert(mesh_indices.end(), temp_indices.begin(), temp_indices.end());
        cnt += 1;
    }

    transparent_mesh.load(mesh_vertices, mesh_uvs, mesh_normals, mesh_indices);
}

void Chunk::start() const
{
    chunk_mesh.start();
}

void Chunk::stop() const
{
    chunk_mesh.stop();
}

int Chunk::get_vertex_count() const
{
    return chunk_mesh.get_vertex_count();
}

void Chunk::start_transparent() const
{
    transparent_mesh.start();
}

void Chunk::stop_transparent() const
{
    transparent_mesh.stop();
}

int Chunk::get_transparent_vertex_count() const
{
    return transparent_mesh.get_vertex_count();
}

void Chunk::set_visible_faces(const std::vector<Face>& visible_f)
{
    visible_faces = visible_f;
    build_mesh();
}

void Chunk::set_transparent_faces(const std::vector<Face>& transparent_f)
{
    transparent_faces = transparent_f;
    build_transparent_mesh();
}

std::vector<Face> Chunk::get_visible_faces() const
{
    return visible_faces;
}

bool Chunk::is_visible_solid_faces() const
{
    return !visible_faces.empty();
}

bool Chunk::is_visible_transparent_faces() const
{
    return !transparent_faces.empty();
}

bool Chunk::is_visible_faces() const
{
    return is_visible_solid_faces() || is_visible_transparent_faces();
}

void Chunk::reset_visible_faces()
{
    visible_faces = {};
    transparent_faces = {};
}
