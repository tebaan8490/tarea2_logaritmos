#include <cstdint>
#include <random>
#include <unordered_set>
#include <vector>
#include <io.h>

std::vector<uint32_t> generarDataset(size_t N) {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<uint32_t> dist(
        0,
        UINT32_MAX
    );

    std::unordered_set<uint32_t> usados;
    std::vector<uint32_t> dataset;
    dataset.reserve(N);

    while (dataset.size() < N) {
        uint32_t x = dist(rng);

        if (usados.insert(x).second) {
            dataset.push_back(x);
        }
    }

    return dataset;
}

double funcionProbabilidad(uint32_t x, size_t N) {
    // lambda entre [0.001, 0.05], elegimos 0.025
    return std::exp(-0.025 * x) * ((1-std::exp(-0.025))/(1-std::exp(-0.025 * N)));
}