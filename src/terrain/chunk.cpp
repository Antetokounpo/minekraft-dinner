#include "chunk.hpp"

#include<cstdlib>
#include<algorithm>

#include<engine/loaders/objloader.hpp>

Chunk::Chunk()
{
    visible_faces = {};

    faces = {
        { // SUD
            -0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f
        },
        { // EST
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f,  0.5f
        },
        { // NORD
             0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f
        },
        { // OUEST
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f
        },
        { // DESSUS
            -0.5f, 0.5f,  0.5f,
             0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,
             0.5f, 0.5f,  0.5f
        },
        { // DESSOUS
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f
        },
    };

}

Chunk::~Chunk(){}

void Chunk::set_position(int i, int k)
{
    x = i;
    z = k;
}

std::tuple<int, int> Chunk::get_position() const
{
    return std::make_tuple(x, z);
}

unsigned int Chunk::get_block(unsigned int x, unsigned int y, unsigned int z) const
{
    return blocks[x][y][z];
}

void Chunk::generate(Perlin& perlin)
{
    for(int i = 0; i<16; ++i)
        for(int k = 0; k<16; ++k)
        {
            double h = perlin.perlin((double)x+((double)i/16), (double)z+((double)k/16));
            for(int j = 0; j<256; ++j)
            {
                if(j < h*10)
                {
                    blocks[i][j][k] = 1;
                }
                else
                    blocks[i][j][k] = 0;
            }
        }
}

void Chunk::build_mesh()
{
    std::array<unsigned, 6> indices = {
        0, 1, 2,
        1, 0, 3
    };

    std::array<float, 8> uvs = {
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f
    };

    std::array<float, 12> normals = {
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
        auto& [i, j, k, f] = face;
        std::vector<float> temp_vertices (faces[f].begin(), faces[f].end());
        for(unsigned c = 0; c<temp_vertices.size(); c += 3)
        {
            temp_vertices[c]   += i;
            temp_vertices[c+1] += j;
            temp_vertices[c+2] += k;
        }

        mesh_vertices.insert(mesh_vertices.end(), temp_vertices.begin(), temp_vertices.end());
        mesh_uvs.insert(mesh_uvs.end(), uvs.begin(), uvs.end());
        mesh_normals.insert(mesh_normals.end(), normals.begin(), normals.end());

        std::vector<unsigned> temp_indices(indices.begin(), indices.end());
        std::transform(temp_indices.begin(), temp_indices.end(), temp_indices.begin(), [&](auto a) { return a+4*cnt; });

        mesh_indices.insert(mesh_indices.end(), temp_indices.begin(), temp_indices.end());
        cnt += 1;
    }

    chunk_mesh.load(&mesh_vertices[0], mesh_vertices.size()*sizeof(mesh_vertices[0]), 
                    &mesh_uvs[0], mesh_uvs.size()*sizeof(mesh_uvs[0]),
                    &mesh_normals[0], mesh_normals.size()*sizeof(mesh_normals[0]),
                    &mesh_indices[0], mesh_indices.size()*sizeof(mesh_indices[0]));
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

void Chunk::set_visible_faces(const std::vector<std::tuple<int, int, int, int>>& visible_f)
{
    visible_faces = visible_f;
    build_mesh();
}

std::vector<std::tuple<int, int, int, int>> Chunk::get_visible_faces() const
{
    return visible_faces;
}

bool Chunk::is_visible_faces() const
{
    return !visible_faces.empty();
}
