#include "terrain.hpp"

#include<algorithm>

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

unsigned int Terrain::get_block(int x, int y, int z)
{
    if(y < 0 || y > 255)
        return 0;
    return get_chunk(x/16, z/16).get_block((unsigned)x % 16, y, (unsigned)z % 16);
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

std::vector<std::tuple<int, int, int, int>> Terrain::get_visible_faces(int u, int v)
{
    const Chunk& chunk = get_chunk(u, v);
    if(!chunk.is_visible_faces())
        compute_visible_faces(u, v);
    return chunk.get_visible_faces();
}

void Terrain::compute_visible_faces(int u, int v)
{
    Chunk& current_chunk = get_chunk(u, v);
    std::vector<std::tuple<int, int, int, int>> faces_to_render;

    for(int i = 0; i<16; ++i)
    {
        for(int j = 0; j<256; ++j)
        {
            for(int k = 0; k<16; ++k)
            {
                
                if(current_chunk.get_block(i, j, k) == 0)
                    continue;

                /* OUEST */
                if(i == 0)
                {
                    if(!get_chunk(u-1, v).get_block(15, j, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 3));
                }
                else
                {
                    if(!current_chunk.get_block(i-1, j, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 3));
                }

                /* NORD */
                if(k == 15)
                {
                    if(!get_chunk(u, v+1).get_block(i, j, 0))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 2));
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k+1))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 2));
                }

                /* EST */
                if(i == 15)
                {
                    if(!get_chunk(u+1, v).get_block(0, j, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 1));
                }
                else
                {
                    if(!current_chunk.get_block(i+1, j, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 1));
                }

                /* SUD */
                if(k == 0)
                {
                    if(!get_chunk(u, v-1).get_block(i, j, 15))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 0));
                }
                else
                {
                    if(!current_chunk.get_block(i, j, k-1))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 0));
                }

                /* DESSUS */
                if(j == 255)
                {
                    faces_to_render.push_back(std::make_tuple(i, j, k, 4));
                }
                else
                {
                    if(!current_chunk.get_block(i, j+1, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 4));
                }

                /* DESSOUS */
                if(j == 0)
                {
                    faces_to_render.push_back(std::make_tuple(i, j, k, 5));
                }
                else
                {
                    if(!current_chunk.get_block(i, j-1, k))
                        faces_to_render.push_back(std::make_tuple(i, j, k, 5));
                }
            }
        }
    }

    std::sort(faces_to_render.begin(), faces_to_render.end(),
        [&current_chunk](std::tuple<int, int, int, int> a, std::tuple<int, int, int, int> b)
        {
            const auto& [x, y, z, w] = a;
            const auto& [i, j, k, m] = b;
            if(current_chunk.get_block(x, y, z) < current_chunk.get_block(i, j, k)) return true;
            if(current_chunk.get_block(i, j, k) < current_chunk.get_block(x, y, z)) return false;

            if(w < m) return true;
            if(m < w) return false;

            return false;
        }
    );

    current_chunk.set_visible_faces(faces_to_render);
}