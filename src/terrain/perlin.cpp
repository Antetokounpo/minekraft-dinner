#include "perlin.hpp"

#include<vector>
#include<algorithm>
#include<cstdlib>
#include<cmath>

#include<iostream>

Perlin::Perlin()
{
    repeat = 0;
}

Perlin::~Perlin(){}

void Perlin::set_seed(unsigned int seed)
{
    std::vector<unsigned int> permutation;
    for(int i = 0; i<256; ++i)
        permutation.push_back(i);

    std::srand(seed);
    std::random_shuffle(permutation.begin(), permutation.end());

    p.clear();
    for(int i = 0; i<512; ++i)
        p.emplace_back(permutation[i % 256]);
}

double Perlin::fade(double t)
{
    return 6*std::pow(t, 5) - 15*std::pow(t, 4) + 10*std::pow(t, 3);
}

double Perlin::grad(int hash, double x, double y)
{
    return ((hash & 1) ? x : -x) + ((hash & 2) ? y : -y);
}

double Perlin::lerp(double a, double b, double x)
{
    return (b - a)*x + a;
}

double Perlin::perlin(double x, double y)
{
    int xi = (unsigned)x & 255;
    int yi = (unsigned)y & 255;

    double xf = x - (int)x;
    double yf = y - (int)y;

    double u = fade(xf);
    double v = fade(yf);

    int aa, ab, ba, bb;

    aa = p[p[xi]+yi];
    ab = p[p[xi]+yi+1];
    ba = p[p[xi+1]+yi];
    bb = p[p[xi+1]+yi+1];

    double x1, x2;

    x1 = lerp(grad(aa, xf, yf), grad(ba, xf-1, yf), u);
    x2 = lerp(grad(ab, xf, yf-1), grad(bb, xf-1, yf-1), u);

    return (lerp(x1, x2, v)+1)/2;
}
