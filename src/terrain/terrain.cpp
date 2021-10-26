#include "terrain.hpp"

#include<algorithm>
#include<cmath>

#include "chunk.hpp"
#include "block.hpp"

Terrain::Terrain(){}

Terrain::~Terrain(){}

void Terrain::set_seed(unsigned int seed)
{
    noise_generator.set_seed(seed);
    noise_generator.add_octave(25, 1);
    noise_generator.add_octave(10, 5);
    noise_generator.add_octave(100, 0.1f);
}

Chunk& Terrain::get_chunk(int x, int y)
{
    if(!is_chunk(x, y))
        generate(x, y);
    return chunks[x][y];
}

Chunk& Terrain::get_chunk_of_block(double x, double y, double z)
{
    return get_chunk(floor(x/16), floor(z/16));
}

Chunk& Terrain::get_chunk_of_block(glm::vec3 v)
{
    return get_chunk_of_block(v.x, v.y, v.z);
}

unsigned int Terrain::get_block(double x, double y, double z)
{
    if(y < 0 || y > 255)
        return 0;
    return get_chunk_of_block(x, y, z).get_block((unsigned)floor(x) % 16, y, (unsigned)floor(z) % 16);
}

unsigned Terrain::get_block(glm::vec3 v)
{
    return get_block(v.x, v.y, v.z);
}

void Terrain::set_block(double x, double y, double z, unsigned b)
{
    Chunk& c = get_chunk_of_block(x, y, z);
    c.set_block((unsigned)floor(x) % 16, y, (unsigned)floor(z) % 16, b);

    // Update les chunks contigus
    const auto& [u, v] = c.get_position();
    for(int i = -1; i<=1; ++i)
    {
        get_chunk(u+i, v).reset_visible_faces();
        get_chunk(u, v+i).reset_visible_faces();
    }
}

void Terrain::set_block(glm::vec3 v, unsigned b)
{
    set_block(v.x, v.y, v.z, b);
}

bool Terrain::is_chunk(int x, int y)
{
    return chunks[x].count(y);
}

void Terrain::generate(int x, int y)
{
    chunks[x][y].set_position(x, y);
    chunks[x][y].generate(noise_generator);
}

std::vector<Face> Terrain::get_visible_faces(int u, int v)
{
    const Chunk& chunk = get_chunk(u, v);
    if(!chunk.is_visible_faces())
        compute_visible_faces(u, v);
    return chunk.get_visible_faces();
}

void Terrain::compute_visible_faces(int u, int v)
{
    Chunk& current_chunk = get_chunk(u, v);
    std::vector<Face> faces_to_render;

    for(unsigned i = 0; i<16; ++i)
    {
        for(unsigned j = 0; j<256; ++j)
        {
            for(unsigned k = 0; k<16; ++k)
            {
                unsigned current_block = current_chunk.get_block(i, j, k);
                
                if(current_block == 0)
                    continue;

                /* OUEST */
                if(i == 0)
                {
                    if(!get_chunk(u-1, v).get_block(15, j, k))
                        faces_to_render.push_back({i, j, k, OUEST, BLOCK_TYPES[current_block].tex_face_o});
                }
                else
                {
                    if(!current_chunk.get_block(i-1, j, k))
                        faces_to_render.push_back({i, j, k, OUEST, BLOCK_TYPES[current_block].tex_face_o});
                }

                /* NORD */
                if(k == 15)
                {
                    if(!get_chunk(u, v+1).get_block(i, j, 0))
                        faces_to_render.push_back({i, j, k, NORD, BLOCK_TYPES[current_block].tex_face_n});
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k+1))
                        faces_to_render.push_back({i, j, k, NORD, BLOCK_TYPES[current_block].tex_face_n});
                }

                /* EST */
                if(i == 15)
                {
                    if(!get_chunk(u+1, v).get_block(0, j, k))
                        faces_to_render.push_back({i, j, k, EST, BLOCK_TYPES[current_block].tex_face_e});
                }
                else
                {
                    if(!current_chunk.get_block(i+1, j, k))
                        faces_to_render.push_back({i, j, k, EST, BLOCK_TYPES[current_block].tex_face_e});
                }

                /* SUD */
                if(k == 0)
                {
                    if(!get_chunk(u, v-1).get_block(i, j, 15))
                        faces_to_render.push_back({i, j, k, SUD, BLOCK_TYPES[current_block].tex_face_s});
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k-1))
                        faces_to_render.push_back({i, j, k, SUD, BLOCK_TYPES[current_block].tex_face_s});
                }

                /* DESSUS */
                if(j == 255)
                {
                    faces_to_render.push_back({i, j, k, DESSUS, BLOCK_TYPES[current_block].tex_face_u});
                }
                else
                {
                    if(!current_chunk.get_block(i, j+1, k))
                        faces_to_render.push_back({i, j, k, DESSUS, BLOCK_TYPES[current_block].tex_face_u});
                }

                /* DESSOUS */
                if(j == 0)
                {
                    faces_to_render.push_back({i, j, k, DESSOUS, BLOCK_TYPES[current_block].tex_face_d});
                }
                else
                {
                    if(!current_chunk.get_block(i, j-1, k))
                        faces_to_render.push_back({i, j, k, DESSOUS, BLOCK_TYPES[current_block].tex_face_d});
                }
            }
        }
    }

    std::sort(faces_to_render.begin(), faces_to_render.end(),
        [&current_chunk](Face a, Face b)
        {
            const auto& [x, y, z, w, v] = a;
            const auto& [i, j, k, m, n] = b;
            if(current_chunk.get_block(x, y, z) < current_chunk.get_block(i, j, k)) return true;
            if(current_chunk.get_block(i, j, k) < current_chunk.get_block(x, y, z)) return false;

            if(w < m) return true;
            if(m < w) return false;

            return false;
        }
    );

    current_chunk.set_visible_faces(faces_to_render);
}
