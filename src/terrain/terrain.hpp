#pragma once

#include<vector>
#include<unordered_map>
#include<memory>
#include<queue>
#include<set>

#include<glm/vec3.hpp>

#include "chunk.hpp"
#include "noise_generator.hpp"
#include "terrain_generator.hpp"

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
        void push_chunk_to_generate(int u, int v);
        void pop_and_generate_chunk();
        void copy_generated_chunks(unsigned n);

        std::vector<Face> get_visible_faces(int u, int v);

    private:
        void compute_visible_faces(int u, int v);
        uint8_t visible_faces_mask(Chunk& current_chunk, unsigned i, unsigned j, unsigned k);
        uint8_t visible_transparent_faces_mask(Chunk& current_chunk, unsigned i, unsigned j, unsigned k);
        NoiseGenerator terrain_noise_generator;
        NoiseGenerator trees_noise_generator;
        TerrainGenerator terrain_generator;

        std::unordered_map<int, std::unordered_map<int, Chunk>> chunks;
        std::queue<std::tuple<int, int>> chunks_to_generate;
        std::set<std::tuple<int, int>> chunks_to_generate_set;
};
