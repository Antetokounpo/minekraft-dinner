#pragma once

#include<vector>
#include<unordered_map>
#include<memory>

#include "chunk.hpp"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        Chunk& get_chunk(int x, int y);
        unsigned int get_block(int x, int y, int z);
        bool is_chunk(int x , int y);
        void generate(int x, int y);
        std::vector<std::tuple<int, int, int>> get_visible_blocks(int u, int v);

    private:
        void compute_visible_blocks(int u, int v);

        std::unordered_map<int, std::unordered_map<int, Chunk>> chunks;
        std::unordered_map<int, std::unordered_map<int, std::vector<std::tuple<int, int, int>>>> visible_blocks_per_chunk;
};
