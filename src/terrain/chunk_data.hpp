#pragma once

#include<array>

#include "noise_generator.hpp"

template<typename T>
using ChunkSizedArray = std::array<std::array<std::array<T, 16>, 256>, 16>;

class ChunkData
{
    public:
        ChunkData();

        std::tuple<int, int> get_position() const;
        bool is_generated() const;
        void set_position(int i, int k);
        unsigned get_block(unsigned x, unsigned y, unsigned z) const;
        void set_block(unsigned x, unsigned y, unsigned z, unsigned b);
        void generate(const NoiseGenerator& terrain_noise_generator, const NoiseGenerator& trees_noise_generator);

        const ChunkSizedArray<unsigned>& get_blocks() const;
        void set_blocks(ChunkSizedArray<unsigned> new_blocks);
    private:
        int x;
        int z;
        ChunkSizedArray<unsigned> blocks;

        bool generated;
};
