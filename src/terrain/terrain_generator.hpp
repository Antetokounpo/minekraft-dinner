#pragma once

#include<thread>
#include<queue>
#include<set>
#include<condition_variable>
#include<optional>

#include<terrain/chunk.hpp>
#include<terrain/chunk_data.hpp>

class TerrainGenerator
{
    public:
        TerrainGenerator();
        ~TerrainGenerator();

        void set_seed(unsigned seed);

        void push_chunk_to_generate(int u, int v);
        std::optional<ChunkData> pop_generated_chunk();

        void start_thread();
        void stop_thread();
    private:
        std::tuple<int, int> pop_chunk_to_generate();
        void generation_thread_loop(); // This is where the magic happens
        std::mutex mut;
        std::condition_variable cond;
        std::thread worker_thread;

        std::queue<std::tuple<int, int>> chunks_to_generate;
        std::set<std::tuple<int, int>> chunks_to_generate_set;
        std::queue<ChunkData> generated_chunks;

        NoiseGenerator terrain_noise_generator;
        NoiseGenerator trees_noise_generator;

        bool running;
};
