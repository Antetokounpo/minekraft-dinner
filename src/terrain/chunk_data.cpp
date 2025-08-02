#include "chunk_data.hpp"

ChunkData::ChunkData()
{
    generated = false;
    for(unsigned i = 0; i<16; ++i) for(unsigned j = 0; j<256; ++j) for(unsigned k = 0; k<16; ++k) blocks[i][j][k] = 0;
}

std::tuple<int, int> ChunkData::get_position() const
{
    return std::make_tuple(x, z);
}

void ChunkData::set_position(int i, int k)
{
    x = i;
    z = k;
}

bool ChunkData::is_generated() const
{
    return generated;
}

unsigned ChunkData::get_block(unsigned x, unsigned y, unsigned z) const
{
    return blocks[x][y][z];
}


void ChunkData::set_block(unsigned x, unsigned y, unsigned z, unsigned b)
{
    blocks[x][y][z] = b;
} 

void ChunkData::generate(const NoiseGenerator& terrain_noise_generator, const NoiseGenerator& trees_noise_generator)
{
    for(int i = 0; i<16; ++i)
    {
        for(int k = 0; k<16; ++k)
        {
            double h = terrain_noise_generator.noise((double)x+((double)i/16), (double)z+((double)k/16));
            h *= 120.0; // Scale

            for(int j = 0; j<256; ++j)
            {
                if(j <= h)
                {
                    if(j <= 50 && j >= 45)
                        blocks[i][j][k] = 4; // Sand
                    else if(j < 45)
                        blocks[i][j][k] = 2; // Stone
                    else if(j == (int)h && h > 12.0)
                        blocks[i][j][k] = 3; // Grass
                    else
                        blocks[i][j][k] = 1; // Dirt
                }

                // Remplir d'eau
                if(j <= 50 && blocks[i][j][k] == 0)
                    blocks[i][j][k] = 5; // Water

                /* On garde dans une array séparée, la transparence pour chaque block du chunk */
                //if(!BLOCK_TYPES[blocks[i][j][k]].transparent)
                //    transparent_blocks[i][j][k] = false;
            }

            // Don't place trees in water or close to the edges of the chunk
            if(h <= 50.0 || i < 2 || i > 13 || k < 2 || k > 13)
                continue;
            
            double current_max = 0.0;
            int R = 9; // The bigger the more spaced are the trees
            for(int di = -R; di <= R; ++di)
            {
                for(int dk = -R; dk <= R; ++dk)
                {
                    double e = trees_noise_generator.noise((double)x+(double)(i+di)/16, (double)z+(double)(k+dk)/16);
                    if(e > current_max) current_max = e; 
                }
            }

            if(trees_noise_generator.noise((double)x+(double)(i)/16, (double)z+(double)(k)/16) == current_max)
            {
                // trunk
                for(int dh = 1; dh < 7; ++dh)
                    blocks[i][(int)h+dh][k] = 8;

                // top leaf block
                blocks[i][(int)h+7][k] = 9;

                // First layer of leaf pyramid
                for(int di = -2; di <= 2; ++di)
                {
                    for(int dk = -2; dk <= 2; ++dk)
                    {
                        if(di == 0 && dk == 0) continue;

                        blocks[i+di][(int)h+5][k+dk] = 9;
                    }
                }

                // Second layer
                for(int di = -1; di <= 1; ++di)
                {
                    for(int dk = -1; dk <= 1; ++dk)
                    {
                        if(di == 0 && dk == 0) continue;

                        blocks[i+di][(int)h+6][k+dk] = 9;
                    }
                }
            }
        }
    }

    generated = true;
}