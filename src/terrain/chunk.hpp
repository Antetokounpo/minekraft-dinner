#pragma once

#include<tuple>

class Chunk
{
    public:
        Chunk();
        ~Chunk();

        void set_position(int i, int k);
        std::tuple<int, int> get_position() const;

        unsigned int get_block(unsigned int x, unsigned int y, unsigned int z) const;
        void generate();
    private:
        int x;
        int z;
        unsigned int blocks[16][256][16];
};
