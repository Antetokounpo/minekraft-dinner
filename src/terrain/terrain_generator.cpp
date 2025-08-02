#include "terrain_generator.hpp"

#include<mutex>

TerrainGenerator::TerrainGenerator()
{
    // Setup noise generator configuration
    terrain_noise_generator.add_octave(25, 1);
    terrain_noise_generator.add_octave(10, 5);
    terrain_noise_generator.add_octave(100, 0.1);

    trees_noise_generator.add_octave(1, 50);

    running = false;
}

TerrainGenerator::~TerrainGenerator()
{
}

void TerrainGenerator::set_seed(unsigned seed)
{
    terrain_noise_generator.set_seed(seed);
    trees_noise_generator.set_seed(seed);
}

void TerrainGenerator::push_chunk_to_generate(int u, int v)
{
    std::lock_guard<std::mutex> lk(mut);

    // Chunk already exists
    if(chunks_to_generate_set.contains({u, v}))
        return;
    
    // Je pense que ce truc n'est pas optimal, TODO: trouver une meilleure structure de données
    chunks_to_generate.push({u, v});
    chunks_to_generate_set.insert({u, v});

    cond.notify_all();
}

std::optional<ChunkData> TerrainGenerator::pop_generated_chunk()
{
    std::lock_guard<std::mutex> lk(mut);

    if(generated_chunks.empty())
        return {};

    auto new_chunk = generated_chunks.front();
    chunks_to_generate_set.erase(new_chunk.get_position());
    generated_chunks.pop();

    return new_chunk;
}

void TerrainGenerator::start_thread()
{
    std::lock_guard<std::mutex> lk(mut);

    if(!running)
    {
        worker_thread = std::thread(&TerrainGenerator::generation_thread_loop, this);
        running = true;
    }

}

void TerrainGenerator::stop_thread()
{
    if(running)
    {
        std::lock_guard<std::mutex> lk(mut);
        running = false;
    } // Fin du lock ici
    else
    {
        return;
    }

    cond.notify_all();
    if(worker_thread.joinable())
    {
        worker_thread.join();
    }
}

std::tuple<int, int> TerrainGenerator::pop_chunk_to_generate()
{
    //if(chunks_to_generate.empty())
    //    return;

    auto c = chunks_to_generate.front();

    // Chunk is erased in the set in pop_generated_chunk
    chunks_to_generate.pop();

    return c;
}

void TerrainGenerator::generation_thread_loop()
{
    std::unique_lock<std::mutex> lk(mut);

    while(running)
    {
        cond.wait(lk, [this]
        {
            return !chunks_to_generate.empty() || !running;
        });

        if(!running)
            break;
        
        auto [u, v] = pop_chunk_to_generate();

        lk.unlock(); // Free up lock when generating

        ChunkData chunk;

        chunk.set_position(u, v);
        chunk.generate(terrain_noise_generator, trees_noise_generator);

        lk.lock();
        generated_chunks.push(chunk);
    }
}
