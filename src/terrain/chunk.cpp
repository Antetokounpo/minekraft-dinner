#include "chunk.hpp"

#include<cstdlib>

Chunk::Chunk(){}

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

void Chunk::generate()
{
    for(int i = 0; i<16; ++i)
        for(int j = 0; j<256; ++j)
            for(int k = 0; k<16; ++k)
            {
                if(j < 50)
                {
                    blocks[i][j][k] = rand() % 2 + 1;
                }
                else
                    blocks[i][j][k] = 0;
            }
}
