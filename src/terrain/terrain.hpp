#pragma once

#include<vector>
#include<unordered_map>
#include<memory>

#include<glm/vec3.hpp>

#include "chunk.hpp"
#include "noise_generator.hpp"

class Terrain
{
    public:
        Terrain();
        ~Terrain();

        void set_seed(unsigned int seed);
        Chunk& get_chunk(int x, int y);
        Chunk& get_chunk_of_block(double x, double y, double z);
        Chunk& get_chunk_of_block(glm::vec3 v);

        unsigned get_block(double x, double y, double z);
        unsigned get_block(glm::vec3 v);
        void set_block(double x, double y, double z, unsigned b);
        void set_block(glm::vec3 v, unsigned b);

        bool is_chunk(int x , int y);
        void generate(int x, int y);
        std::vector<Face> get_visible_faces(int u, int v);

    private:
        void compute_visible_faces(int u, int v);
        NoiseGenerator noise_generator;

        std::unordered_map<int, std::unordered_map<int, Chunk>> chunks;
};
