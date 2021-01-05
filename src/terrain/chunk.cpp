#include "chunk.hpp"

#include<cstdlib>

Chunk::Chunk()
{
    for(unsigned int i = 0; i<16; ++i)
        for(unsigned int j = 0; j<16; ++j)
            for(unsigned int k = 0; k<256; ++k)
                blocks[i][j][k] = 0;
}

Chunk::~Chunk(){}

unsigned int Chunk::get_block(unsigned int x, unsigned int y, unsigned int z)
{
    return blocks[x][y][z];
}

void Chunk::generate()
{
    for(int i = 0; i<16; ++i)
        for(int j = 0; j<256; ++j)
            for(int k = 0; k<16; ++k)
            {
                if(rand() % 2)
                    blocks[i][j][k] = 1;
                else
                    blocks[i][j][k] = 0;
            }
}

