#include "terrain.hpp"

#include<algorithm>
#include<cmath>

#include "chunk.hpp"

Terrain::Terrain(){}

Terrain::~Terrain(){}

void Terrain::set_seed(unsigned int seed)
{
    perlin.set_seed(seed);
}

Chunk& Terrain::get_chunk(int x, int y)
{
    if(!is_chunk(x, y))
        generate(x, y);
    return chunks[x][y];
}

Chunk& Terrain::get_chunk_of_block(int x, int y, int z)
{
    return get_chunk(floor((double)x/16), floor((double)z/16));
}

unsigned int Terrain::get_block(int x, int y, int z)
{
    if(y < 0 || y > 255)
        return 0;
    return get_chunk_of_block(x, y, z).get_block((unsigned)x % 16, y, (unsigned)z % 16);
}

unsigned Terrain::get_block(glm::vec3 v)
{
    return get_block(v.x, v.y, v.z);
}

void Terrain::set_block(int x, int y, int z, unsigned b)
{
    Chunk& c = get_chunk_of_block(x, y, z);
    c.set_block((unsigned)x % 16, y, (unsigned)z % 16, b);

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
    chunks[x][y].generate(perlin);
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
                        faces_to_render.push_back({i, j, k, OUEST, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i-1, j, k))
                        faces_to_render.push_back({i, j, k, OUEST, current_block-1});
                }

                /* NORD */
                if(k == 15)
                {
                    if(!get_chunk(u, v+1).get_block(i, j, 0))
                        faces_to_render.push_back({i, j, k, NORD, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k+1))
                        faces_to_render.push_back({i, j, k, NORD, current_block-1});
                }

                /* EST */
                if(i == 15)
                {
                    if(!get_chunk(u+1, v).get_block(0, j, k))
                        faces_to_render.push_back({i, j, k, EST, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i+1, j, k))
                        faces_to_render.push_back({i, j, k, EST, current_block-1});
                }

                /* SUD */
                if(k == 0)
                {
                    if(!get_chunk(u, v-1).get_block(i, j, 15))
                        faces_to_render.push_back({i, j, k, SUD, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k-1))
                        faces_to_render.push_back({i, j, k, SUD, current_block-1});
                }

                /* DESSUS */
                if(j == 255)
                {
                    faces_to_render.push_back({i, j, k, DESSUS, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i, j+1, k))
                        faces_to_render.push_back({i, j, k, DESSUS, current_block-1});
                }

                /* DESSOUS */
                if(j == 0)
                {
                    faces_to_render.push_back({i, j, k, DESSOUS, current_block-1});
                }
                else
                {
                    if(!current_chunk.get_block(i, j-1, k))
                        faces_to_render.push_back({i, j, k, DESSOUS, current_block-1});
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
