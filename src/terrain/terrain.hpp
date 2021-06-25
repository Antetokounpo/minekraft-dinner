#pragma once

#include<vector>
#include<unordered_map>
#include<memory>

#include "chunk.hpp"
#include "perlin.hpp"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        void set_seed(unsigned int seed);
        Chunk& get_chunk(int x, int y);
        unsigned int get_block(int x, int y, int z);
        bool is_chunk(int x , int y);
        void generate(int x, int y);
        std::vector<Face> get_visible_faces(int u, int v);

    private:
        void compute_visible_faces(int u, int v);
        Perlin perlin;

        std::unordered_map<int, std::unordered_map<int, Chunk>> chunks;
        std::unordered_map<int, std::unordered_map<int, std::vector<std::tuple<int, int, int, int>>>> visible_faces_per_chunk;
};
