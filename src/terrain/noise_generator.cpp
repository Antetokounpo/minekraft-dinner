#include "noise_generator.hpp"

#include<numeric>

NoiseGenerator::NoiseGenerator()
{
    frequency = 1.0f;
    amplitude = 1.0f;
}

NoiseGenerator::~NoiseGenerator() {}

void NoiseGenerator::add_octave(double a, double f)
{
    amplitudes.push_back(a);
    frequencies.push_back(f);
}

double NoiseGenerator::noise(double x, double y, double z)
{
    double e = 0.0f;
    for(unsigned i = 0; i<amplitudes.size(); ++i)
    {
        e += amplitudes[i] * perlin(frequencies[i]*x, frequencies[i]*y, frequencies[i]*z);
    }

    return e;
}
