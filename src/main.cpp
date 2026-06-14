#include <io.h>
#include <models.h>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <random>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <string>

using Clock = std::chrono::high_resolution_clock;
using Duration = std::chrono::duration<double, std::milli>;

static const size_t c = 3;

template <typename F>
double medirTiempo(F&& accion) {
    auto inicio = Clock::now();
    accion();
    return Duration(Clock::now() - inicio).count();
}

std::vector<double> construirPesosSesgados(size_t N) {
    std::vector<double> pesos;
    pesos.reserve(N);
    for (size_t i = 0; i < N; ++i) {
        pesos.push_back(funcionProbabilidad(i, N));
    }
    return pesos;
}

struct ResultadoExperimento {
    size_t N;
    size_t M;
    std::string insercion;
    std::string busqueda;
    double avlInsercionMs;
    double avlBusquedaMs;
    double splayInsercionMs;
    double splayBusquedaMs;
};

void imprimirEncabezado() {
    std::cout << std::left << std::setw(8) << "N"
              << std::setw(12) << "Insercion"
              << std::setw(12) << "Busqueda"
              << std::setw(14) << "AVL ins(ms)"
              << std::setw(14) << "AVL busq(ms)"
              << std::setw(14) << "Splay ins(ms)"
              << std::setw(14) << "Splay busq(ms)"
              << "\n";
    std::cout << std::string(80, '-') << "\n";
}

void imprimirResultado(const ResultadoExperimento& resultado) {
    std::cout << std::left << std::setw(8) << resultado.N
              << std::setw(12) << resultado.insercion
              << std::setw(12) << resultado.busqueda
              << std::setw(14) << std::fixed << std::setprecision(2) << resultado.avlInsercionMs
              << std::setw(14) << resultado.avlBusquedaMs
              << std::setw(14) << resultado.splayInsercionMs
              << std::setw(14) << resultado.splayBusquedaMs
              << "\n";
}

template <typename Tree>
double medirInsercion(Tree& arbol, const std::vector<uint32_t>& valores) {
    return medirTiempo([&] {
        for (uint32_t x : valores) {
            arbol.insert(x);
        }
    });
}

template <typename Tree>
double medirBusqueda(Tree& arbol,
                     size_t M,
                     const std::vector<uint32_t>& conjuntoDatos,
                     std::mt19937& rng,
                     bool sesgada,
                     std::uniform_int_distribution<size_t>& distribucionUniforme,
                     std::discrete_distribution<size_t>& distribucionSesgada) {
    return medirTiempo([&] {
        if (sesgada) {
            for (size_t j = 0; j < M; ++j) {
                size_t idx = distribucionSesgada(rng);
                arbol.search(conjuntoDatos[idx]);
            }
        } else {
            for (size_t j = 0; j < M; ++j) {
                size_t idx = distribucionUniforme(rng);
                arbol.search(conjuntoDatos[idx]);
            }
        }
    });
}

ResultadoExperimento ejecutarEscenario(size_t N,
                             bool insercionOrdenada,
                             bool busquedaSesgada,
                             std::mt19937& rng) {
    size_t multiplier = 1;
    for (size_t i = 0; i < c; ++i) {
        multiplier *= 10;
    }
    size_t M = multiplier * N;

    std::vector<uint32_t> dataset = generarDataset(N);
    std::vector<uint32_t> insertionValues = dataset;

    if (insercionOrdenada) {
        std::sort(insertionValues.begin(), insertionValues.end());
    } else {
        std::shuffle(insertionValues.begin(), insertionValues.end(), rng);
    }

    std::vector<double> pesosSesgados = construirPesosSesgados(N);
    std::uniform_int_distribution<size_t> distribucionUniforme(0, N - 1);
    std::discrete_distribution<size_t> distribucionSesgada(pesosSesgados.begin(), pesosSesgados.end());

    AVLTree avl;
    SplayTree splay;

    double avlInsertMs = medirInsercion(avl, insertionValues);
    double splayInsertMs = medirInsercion(splay, insertionValues);

    double avlSearchMs = medirBusqueda(avl, M, dataset, rng, busquedaSesgada, distribucionUniforme, distribucionSesgada);
    double splaySearchMs = medirBusqueda(splay, M, dataset, rng, busquedaSesgada, distribucionUniforme, distribucionSesgada);

    ResultadoExperimento resultado;
    resultado.N = N;
    resultado.M = M;
    resultado.insercion = insercionOrdenada ? "Ordenada" : "Aleatoria";
    resultado.busqueda = busquedaSesgada ? "Sesgada" : "Uniforme";
    resultado.avlInsercionMs = avlInsertMs;
    resultado.avlBusquedaMs = avlSearchMs;
    resultado.splayInsercionMs = splayInsertMs;
    resultado.splayBusquedaMs = splaySearchMs;
    return resultado;
}

void ejecutarExperimentosBase(std::mt19937& rng) {
    std::cout << "Conjunto de experimentos: escenarios base (c=" << c << ")" << "\n";
    imprimirEncabezado();
    size_t valoresN[] = {1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14};
    for (size_t N : valoresN) {
        imprimirResultado(ejecutarEscenario(N, false, false, rng));
        imprimirResultado(ejecutarEscenario(N, false, true, rng));
        imprimirResultado(ejecutarEscenario(N, true, false, rng));
        imprimirResultado(ejecutarEscenario(N, true, true, rng));
    }
}

void ejecutarTeoremaAccesoSecuencial(size_t N, std::mt19937& rng) {
    std::cout << "\n=== Teorema de acceso secuencial (N=" << N << ") ===\n";
    std::vector<uint32_t> conjuntoDatos = generarDataset(N);
    std::vector<uint32_t> ordenInsercion = conjuntoDatos;
    std::shuffle(ordenInsercion.begin(), ordenInsercion.end(), rng);

    AVLTree avl;
    SplayTree splay;
    for (uint32_t x : ordenInsercion) {
        avl.insert(x);
        splay.insert(x);
    }

    std::vector<uint32_t> clavesOrdenadas = conjuntoDatos;
    std::sort(clavesOrdenadas.begin(), clavesOrdenadas.end());

    std::cout << std::left << std::setw(12) << "m"
              << std::setw(16) << "AVL(ms)"
              << std::setw(16) << "Splay(ms)"
              << "\n";
    std::cout << std::string(44, '-') << "\n";

    for (size_t k = 1; k <= 10; ++k) {
        size_t m = (k < 10) ? (N * k / 100) : (N / 10);
        if (m == 0) {
            continue;
        }
        std::vector<uint32_t> consultas;
        consultas.reserve(m);
        size_t paso = std::max<size_t>(1, N / m);
        for (size_t i = 0; i < m; ++i) {
            size_t idx = std::min(N - 1, i * paso);
            consultas.push_back(clavesOrdenadas[idx]);
        }

        double avlTiempo = medirTiempo([&] {
            for (uint32_t clave : consultas) {
                avl.search(clave);
            }
        });
        double splayTiempo = medirTiempo([&] {
            for (uint32_t clave : consultas) {
                splay.search(clave);
            }
        });

        std::cout << std::left << std::setw(12) << m
                  << std::setw(16) << std::fixed << std::setprecision(2) << avlTiempo
                  << std::setw(16) << splayTiempo
                  << "\n";
    }
}

void ejecutarTeoremaWorkingSet(size_t N, std::mt19937& rng) {
    std::cout << "\n=== Teorema del working set (N=" << N << ") ===\n";
    std::vector<uint32_t> conjuntoDatos = generarDataset(N);
    std::vector<uint32_t> ordenInsercion = conjuntoDatos;
    std::shuffle(ordenInsercion.begin(), ordenInsercion.end(), rng);

    AVLTree avl;
    SplayTree splay;
    for (uint32_t x : ordenInsercion) {
        avl.insert(x);
        splay.insert(x);
    }

    std::vector<size_t> tamWorkingSet = {10, 100, 1000, 10000, 100000, 1000000};
    size_t multiplicador = 1;
    for (size_t i = 0; i < c; ++i) {
        multiplicador *= 10;
    }
    size_t M = multiplicador * N;

    std::cout << std::left << std::setw(12) << "W"
              << std::setw(16) << "AVL(ms)"
              << std::setw(16) << "Splay(ms)"
              << "\n";
    std::cout << std::string(44, '-') << "\n";

    for (size_t W : tamWorkingSet) {
        if (W > N) {
            break;
        }
        std::vector<uint32_t> conjuntoTrabajo;
        conjuntoTrabajo.reserve(W);
        std::sample(conjuntoDatos.begin(), conjuntoDatos.end(), std::back_inserter(conjuntoTrabajo), W, rng);

        std::uniform_int_distribution<size_t> distribucionUniforme(0, conjuntoTrabajo.size() - 1);

        double avlTiempo = medirTiempo([&] {
            for (size_t i = 0; i < M; ++i) {
                avl.search(conjuntoTrabajo[distribucionUniforme(rng)]);
            }
        });
        double splayTiempo = medirTiempo([&] {
            for (size_t i = 0; i < M; ++i) {
                splay.search(conjuntoTrabajo[distribucionUniforme(rng)]);
            }
        });

        std::cout << std::left << std::setw(12) << W
                  << std::setw(16) << std::fixed << std::setprecision(2) << avlTiempo
                  << std::setw(16) << splayTiempo
                  << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::random_device rd;
    std::mt19937 rng(rd());

    ejecutarExperimentosBase(rng);

    if (argc > 1 && std::string(argv[1]) == "--theorems") {
        size_t N = 1u << 25;
        ejecutarTeoremaAccesoSecuencial(N, rng);
        ejecutarTeoremaWorkingSet(N, rng);
    }

    SplayTree sp;
    for (uint32_t i = 1; i <= 10; i++) sp.insert(i);

    std::cout << "\n[Splay] Inorden: ";
    inOrden(sp.root);
    std::cout << "\n";

    sp.search(3);
    std::cout << "[Splay] raiz tras buscar 3: " << sp.root->value << "\n";
    sp.search(8);
    std::cout << "[Splay] raiz tras buscar 8: " << sp.root->value << "\n";
    std::cout << "[Splay] busca inexistente 99 (no debe crashear)\n";
    sp.search(99);
    std::cout << "[Splay] raiz tras buscar 99: " << sp.root->value << "\n";

    return 0;
}
