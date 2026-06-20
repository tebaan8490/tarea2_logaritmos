#include <cstdint>
#include <random>
#include <unordered_set>
#include <vector>

/**
 * @brief Genera un dataset de N números distintos con distribución uniforme
 *
 * @param N Tamaño del dataset
 * @return Vector con los elementos distintos
 */
std::vector<uint32_t> generarDataset(size_t N);

/**
 * @brief Función que representa una función de probabilidad sesgada basada en la distribución exponencial
 * 
 * Se usa lamda = 0.025
 *
 * @param x Indice
 * @param N Tamaño del dataset
 * @return Probabilidad del elemento con índice x
 */
double funcionProbabilidad(uint32_t x, size_t N);