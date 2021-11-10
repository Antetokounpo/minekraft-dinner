#pragma once

#include<tuple>
#include<vector>

#include<engine/model.hpp>
#include "noise_generator.hpp"
#include "faces.hpp"
#include "block.hpp"

class Chunk
{
    public:
        Chunk();
        ~Chunk();

        void set_position(int i, int k);
        std::tuple<int, int> get_position() const;

        unsigned get_block(unsigned int x, unsigned int y, unsigned int z) const;
        void set_block(unsigned x, unsigned y, unsigned z, unsigned b);
        void generate(NoiseGenerator& generator);
        
        void build_mesh();
        void start() const;
        void stop() const;
        int get_vertex_count() const;

        std::vector<Face> get_visible_faces() const;
        void set_visible_faces(const std::vector<Face>& visible_f);
        bool is_visible_faces() const;
        void reset_visible_faces();

    private:
        int x;
        int z;
        unsigned blocks[16][256][16];
        std::vector<Face> visible_faces;
        Model chunk_mesh;
};
