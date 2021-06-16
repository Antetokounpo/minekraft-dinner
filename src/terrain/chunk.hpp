#pragma once

#include<tuple>
#include<vector>

#include<engine/model.hpp>
#include "perlin.hpp"

class Chunk
{
    public:
        Chunk();
        ~Chunk();

        void set_position(int i, int k);
        std::tuple<int, int> get_position() const;

        unsigned int get_block(unsigned int x, unsigned int y, unsigned int z) const;
        void generate(Perlin& perlin);
        
        void build_mesh();
        void start() const;
        void stop() const;
        int get_vertex_count() const;

        std::vector<std::tuple<int, int, int, int>> get_visible_faces() const;
        void set_visible_faces(const std::vector<std::tuple<int, int, int, int>>& visible_f);
        bool is_visible_faces() const;
    private:
        int x;
        int z;
        unsigned int blocks[16][256][16];
        std::vector<std::tuple<int, int, int, int>> visible_faces;
        std::vector<std::array<float, 12>> faces;
        Model chunk_mesh;
};
