#include "terrain.hpp"

#include<algorithm>
#include<cmath>

#include "chunk.hpp"
#include "block.hpp"

#include<chrono> // profile
#include<iostream>

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

uint8_t Terrain::visible_faces_mask(Chunk& current_chunk, unsigned i, unsigned j, unsigned k)
{
    uint8_t block_mask = 0x0;
    auto [u, v] = current_chunk.get_position();

    /* OUEST */
    if (i == 0)
    {
        if (get_chunk(u - 1, v).is_block_transparent(15, j, k))
            block_mask |= (0x1 << OUEST);
    }
    else
    {
        if (current_chunk.is_block_transparent(i - 1, j, k))
            block_mask |= (0x1 << OUEST);
    }

    /* NORD */
    if (k == 15)
    {
        if (get_chunk(u, v + 1).is_block_transparent(i, j, 0))
            block_mask |= (0x1 << NORD);
    }
    else
    {
        if (current_chunk.is_block_transparent(i, j, k + 1))
            block_mask |= (0x1 << NORD);
    }

    /* EST */
    if (i == 15)
    {
        if (get_chunk(u + 1, v).is_block_transparent(0, j, k))
            block_mask |= (0x1 << EST);
    }
    else
    {
        if (current_chunk.is_block_transparent(i + 1, j, k))
            block_mask |= (0x1 << EST);
    }

    /* SUD */
    if (k == 0)
    {
        if (get_chunk(u, v - 1).is_block_transparent(i, j, 15))
            block_mask |= (0x1 << SUD);
    }
    else
    {
        if (current_chunk.is_block_transparent(i, j, k - 1))
            block_mask |= (0x1 << SUD);
    }

    /* DESSUS */
    if (j == 255)
    {
        block_mask |= (0x1 << DESSUS);
    }
    else
    {
        if (current_chunk.is_block_transparent(i, j + 1, k))
            block_mask |= (0x1 << DESSUS);
    }

    /* DESSOUS */
    if (j == 0)
    {
        block_mask |= (0x1 << DESSOUS);
    }
    else
    {
        if (current_chunk.is_block_transparent(i, j - 1, k))
            block_mask |= (0x1 << DESSOUS);
    }

    return block_mask;
}

uint8_t Terrain::visible_transparent_faces_mask(Chunk& current_chunk, unsigned i, unsigned j, unsigned k)
{
    uint8_t block_mask = 0x0;
    auto [u, v] = current_chunk.get_position();

    /* OUEST */
    if (i == 0)
    {
        if (get_chunk(u - 1, v).is_block_air(15, j, k))
            block_mask |= (0x1 << OUEST);
    }
    else
    {
        if (current_chunk.is_block_air(i - 1, j, k))
            block_mask |= (0x1 << OUEST);
    }

    /* NORD */
    if (k == 15)
    {
        if (get_chunk(u, v + 1).is_block_air(i, j, 0))
            block_mask |= (0x1 << NORD);
    }
    else
    {
        if (current_chunk.is_block_air(i, j, k + 1))
            block_mask |= (0x1 << NORD);
    }

    /* EST */
    if (i == 15)
    {
        if (get_chunk(u + 1, v).is_block_air(0, j, k))
            block_mask |= (0x1 << EST);
    }
    else
    {
        if (current_chunk.is_block_air(i + 1, j, k))
            block_mask |= (0x1 << EST);
    }

    /* SUD */
    if (k == 0)
    {
        if (get_chunk(u, v - 1).is_block_air(i, j, 15))
            block_mask |= (0x1 << SUD);
    }
    else
    {
        if (current_chunk.is_block_air(i, j, k - 1))
            block_mask |= (0x1 << SUD);
    }

    /* DESSUS */
    if (j == 255)
    {
        block_mask |= (0x1 << DESSUS);
    }
    else
    {
        if (current_chunk.is_block_air(i, j + 1, k))
            block_mask |= (0x1 << DESSUS);
    }

    /* DESSOUS */
    if (j == 0)
    {
        block_mask |= (0x1 << DESSOUS);
    }
    else
    {
        if (current_chunk.is_block_air(i, j - 1, k))
            block_mask |= (0x1 << DESSOUS);
    }

    return block_mask;
}

void Terrain::compute_visible_faces(int u, int v)
{
    // temps moyen : ~ 8500 microsecondes
    //auto start = std::chrono::high_resolution_clock::now();
    Chunk& current_chunk = get_chunk(u, v);
    std::vector<Face> faces_to_render;
    std::vector<Face> transparent_faces_to_render;

    for(unsigned i = 0; i<16; ++i)
    {
        for(unsigned j = 0; j<256; ++j)
        {
            for(unsigned k = 0; k<16; ++k)
            {
                auto current_block = current_chunk.get_block(i, j, k);
                auto current_block_data = BLOCK_TYPES[current_block];

                if(current_block == 0) // Si c'est de l'air, on skip
                    continue;

                uint8_t block_mask = !current_block_data.transparent ? visible_faces_mask(current_chunk, i, j, k) : visible_transparent_faces_mask(current_chunk, i, j, k);

                FaceOrientation face_o;
                unsigned tex_face_id;
                for(int u = 0; u<6; ++u)
                {
                    face_o = (FaceOrientation)u;
                    if(block_mask & (0x1 << face_o))
                    {
                        switch (face_o)
                        {
                        case SUD:
                            tex_face_id = current_block_data.tex_face_s; 
                            break;
                        case EST:
                            tex_face_id = current_block_data.tex_face_e;
                            break;
                        case NORD:
                            tex_face_id = current_block_data.tex_face_n;
                            break;
                        case OUEST:
                            tex_face_id = current_block_data.tex_face_o;
                            break;
                        case DESSUS:
                            tex_face_id = current_block_data.tex_face_u;
                            break;
                        case DESSOUS:
                            tex_face_id = current_block_data.tex_face_d;
                            break;
                        default:
                            break;
                        }
                        
                        if(current_block_data.transparent)
                            transparent_faces_to_render.push_back({i, j, k, face_o, tex_face_id});
                        else
                            faces_to_render.push_back({i, j, k, face_o, tex_face_id});
                    }
                }
            }
        }
    }

    current_chunk.set_visible_faces(faces_to_render);
    current_chunk.set_transparent_faces(transparent_faces_to_render);

    /*
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

    std::cout << duration.count() << " microseconds" << std::endl;
    */
}
