#pragma once

#include<array>

class Chunk
{
    public:
        Chunk();
        ~Chunk();

        unsigned int get_block(unsigned int x, unsigned int y, unsigned int z);
        void generate();
    private:
        int x;
        int y;
        unsigned int blocks[16][256][16];
};
