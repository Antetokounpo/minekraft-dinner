#pragma once

#include<tuple>
#include<vector>

#include<engine/model.hpp>
#include "noise_generator.hpp"
#include "faces.hpp"
#include "block.hpp"
#include "chunk_data.hpp"

// TODO: hériter de ChunkData
class Chunk
{
    public:
        Chunk(); // Constructor

        void set_position(int i, int k);
        std::tuple<int, int> get_position() const;

        void set_chunk_data(ChunkData new_chunk_data);
        bool is_generated() const;

        unsigned get_block(unsigned int x, unsigned int y, unsigned int z) const;
        bool is_block_transparent(unsigned x, unsigned y, unsigned z) const;
        bool is_block_air(unsigned x, unsigned y, unsigned z) const;
        void set_block(unsigned x, unsigned y, unsigned z, unsigned b);
        
        void build_mesh();
        void build_transparent_mesh();

        void start() const;
        void stop() const;
        int get_vertex_count() const;

        void start_transparent() const;
        void stop_transparent() const;
        int get_transparent_vertex_count() const;

        std::vector<Face> get_visible_faces() const;
        void set_visible_faces(const std::vector<Face>& visible_f);
        void set_transparent_faces(const std::vector<Face>& transparent_f);
        bool is_visible_faces() const;
        bool is_visible_solid_faces() const;
        bool is_visible_transparent_faces() const;
        void reset_visible_faces();

    private:
        ChunkData chunk_data;
        std::vector<Face> visible_faces;
        std::vector<Face> transparent_faces;
        Model chunk_mesh;
        Model transparent_mesh;
};
