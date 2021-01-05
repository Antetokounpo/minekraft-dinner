#pragma once

#include<vector>

#include "chunk.hpp"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        Chunk get_chunk(int x, int y);
        void generate();

    private:
        std::vector<std::vector<Chunk>> chunks;
};