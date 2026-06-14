#include <io.h>
#include <models.h>
#include <iostream>     
#include <vector>       
#include <random>
#include <cstdint>      
#include <algorithm>    
#include <cmath>        
#include <chrono>       
#include <unordered_set>

void init_experiment(size_t n) {
    std::vector<uint32_t> dataset = generarDataset(n);
    
    std::vector<double> pesos;
    
    for (size_t i = 0; i < n; i++) {
        pesos.push_back(funcionProbabilidad(i, n));
    }

    std::discrete_distribution<size_t> dist(pesos.begin(),pesos.end());
}
