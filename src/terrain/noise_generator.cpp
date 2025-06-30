#include "noise_generator.hpp"

#include<numeric>
#include<cmath>

NoiseGenerator::NoiseGenerator() {}

NoiseGenerator::~NoiseGenerator() {}

void NoiseGenerator::add_octave(double a, double f)
{
    amplitudes.push_back(a);
    frequencies.push_back(f);
}

double NoiseGenerator::noise(double x, double y, double z) const
{
    double e = 0.0f;
    for(unsigned i = 0; i<amplitudes.size(); ++i)
    {
        e += amplitudes[i] * perlin(frequencies[i]*x, frequencies[i]*y, frequencies[i]*z);
    }

    // Normalize output noise (between 0 and 1)
    e /= std::reduce(amplitudes.begin(), amplitudes.end()); // Sum of vector

    //e = round(e * 100) / 100;

    return e;
}
