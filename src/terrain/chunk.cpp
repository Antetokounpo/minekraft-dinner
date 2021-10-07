#include "chunk.hpp"

#include<cstdlib>
#include<algorithm>

#include<engine/loaders/objloader.hpp>

#include "faces.hpp"

Chunk::Chunk()
{
    visible_faces = {};
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

void Chunk::set_block(unsigned x, unsigned y, unsigned z, unsigned b)
{
    blocks[x][y][z] = b;
    reset_visible_faces();
}

void Chunk::generate(NoiseGenerator& generator)
{
    for(int i = 0; i<16; ++i)
        for(int k = 0; k<16; ++k)
        {
            double h = generator.noise((double)x+((double)i/16), (double)z+((double)k/16));
            for(int j = 0; j<256; ++j)
            {
                if(j < h)
                {
                    if(h < 10.0f)
                        blocks[i][j][k] = 2;
                    else if(j == (int)h && h > 12.0f)
                        blocks[i][j][k] = 3;
                    else
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
        1.0f/(atlas_w/tex_w), 1.0f/(atlas_h/tex_h),
        0.0f, 1.0f/(atlas_h/tex_h),
        1.0f/(atlas_w/tex_w), 0.0f
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
        auto& [i, j, k, f, t] = face;

        std::vector<float> temp_vertices (faces[f].begin(), faces[f].end());
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

void Chunk::set_visible_faces(const std::vector<Face>& visible_f)
{
    visible_faces = visible_f;
    build_mesh();
}

std::vector<Face> Chunk::get_visible_faces() const
{
    return visible_faces;
}

bool Chunk::is_visible_faces() const
{
    return !visible_faces.empty();
}

void Chunk::reset_visible_faces()
{
    visible_faces = {};
}
