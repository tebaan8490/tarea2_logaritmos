#include <io.h>
#include <models.h>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <filesystem>
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

static const size_t c = 2;

static const size_t NUM_BLOQUES = 200;

static const size_t WS_LIMITE_BUSQUEDAS = 0;

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

struct ResultadoBusqueda {
    double totalMs = 0.0;
    std::vector<double> tiempoBloqueMs;
    std::vector<size_t> consultasBloque;
};

template <typename Tree, typename Dist>
ResultadoBusqueda medirBusquedaPorBloques(Tree& arbol,
                                          size_t M,
                                          const std::vector<uint32_t>& dataset,
                                          std::mt19937& gen,
                                          Dist& dist,
                                          size_t numBloques) {
    ResultadoBusqueda r;
    numBloques = std::min(numBloques, M);
    if (numBloques == 0) return r;

    size_t base = M / numBloques;
    size_t resto = M % numBloques;

    for (size_t b = 0; b < numBloques; ++b) {
        size_t cnt = base + (b < resto ? 1 : 0);
        double t = medirTiempo([&] {
            for (size_t j = 0; j < cnt; ++j) {
                arbol.search(dataset[dist(gen)]);
            }
        });
        r.tiempoBloqueMs.push_back(t);
        r.consultasBloque.push_back(cnt);
        r.totalMs += t;
    }
    return r;
}

struct ResultadoEscenario {
    size_t N, M;
    std::string insercion, busqueda;
    double avlInsMs, splayInsMs;
    double avlBusqMs, splayBusqMs;
};

void imprimirEncabezado() {
    std::cout << std::left << std::setw(8) << "N"
              << std::setw(12) << "Insercion"
              << std::setw(12) << "Busqueda"
              << std::setw(14) << "AVL ins(ms)"
              << std::setw(14) << "AVL busq(ms)"
              << std::setw(14) << "Splay ins(ms)"
              << std::setw(14) << "Splay busq(ms)"
              << "\n" << std::string(88, '-') << "\n";
}

void imprimirResultado(const ResultadoEscenario& r) {
    std::cout << std::left << std::setw(8) << r.N
              << std::setw(12) << r.insercion
              << std::setw(12) << r.busqueda
              << std::setw(14) << std::fixed << std::setprecision(2) << r.avlInsMs
              << std::setw(14) << r.avlBusqMs
              << std::setw(14) << r.splayInsMs
              << std::setw(14) << r.splayBusqMs
              << "\n";
}

template <typename Tree>
double medirInsercion(Tree& arbol, const std::vector<uint32_t>& valores) {
    return medirTiempo([&] {
        for (uint32_t x : valores) arbol.insert(x);
    });
}

void volcarBloques(std::ofstream& csv, size_t N, const std::string& insercion,
                   const std::string& busqueda, const std::string& estructura,
                   const ResultadoBusqueda& r) {
    size_t acumulado = 0;
    for (size_t b = 0; b < r.tiempoBloqueMs.size(); ++b) {
        acumulado += r.consultasBloque[b];
        double nsPorBusqueda = r.consultasBloque[b] > 0
            ? r.tiempoBloqueMs[b] * 1e6 / r.consultasBloque[b] : 0.0;
        csv << N << "," << insercion << "," << busqueda << "," << estructura << ","
            << b << "," << acumulado << ","
            << r.tiempoBloqueMs[b] << "," << nsPorBusqueda << "\n";
    }
}

ResultadoEscenario ejecutarEscenario(size_t N,
                                     bool insercionOrdenada,
                                     bool busquedaSesgada,
                                     const std::vector<uint32_t>& dataset,
                                     std::mt19937& rng,
                                     std::ofstream& csvBloques) {
    size_t M = 10 * c * N;

    std::vector<uint32_t> valoresInsercion = dataset;
    if (insercionOrdenada)
        std::sort(valoresInsercion.begin(), valoresInsercion.end());
    else
        std::shuffle(valoresInsercion.begin(), valoresInsercion.end(), rng);

    std::vector<double> pesos = construirPesosSesgados(N);
    std::uniform_int_distribution<size_t> distUniforme(0, N - 1);
    std::discrete_distribution<size_t> distSesgada(pesos.begin(), pesos.end());

    AVLTree avl;
    SplayTree splay;
    double avlInsMs = medirInsercion(avl, valoresInsercion);
    double splayInsMs = medirInsercion(splay, valoresInsercion);

    std::string insercion = insercionOrdenada ? "Ordenada" : "Aleatoria";
    std::string busqueda = busquedaSesgada ? "Sesgada" : "Uniforme";

    uint32_t semilla = rng();
    ResultadoBusqueda rAvl, rSplay;
    if (busquedaSesgada) {
        std::mt19937 gA(semilla); distSesgada.reset();
        rAvl = medirBusquedaPorBloques(avl, M, dataset, gA, distSesgada, NUM_BLOQUES);
        std::mt19937 gS(semilla); distSesgada.reset();
        rSplay = medirBusquedaPorBloques(splay, M, dataset, gS, distSesgada, NUM_BLOQUES);
    } else {
        std::mt19937 gA(semilla);
        rAvl = medirBusquedaPorBloques(avl, M, dataset, gA, distUniforme, NUM_BLOQUES);
        std::mt19937 gS(semilla);
        rSplay = medirBusquedaPorBloques(splay, M, dataset, gS, distUniforme, NUM_BLOQUES);
    }

    volcarBloques(csvBloques, N, insercion, busqueda, "AVL", rAvl);
    volcarBloques(csvBloques, N, insercion, busqueda, "Splay", rSplay);

    return {N, M, insercion, busqueda, avlInsMs, splayInsMs, rAvl.totalMs, rSplay.totalMs};
}

void ejecutarExperimentosBase(std::mt19937& rng) {
    std::cout << "Conjunto de experimentos: escenarios base (c=" << c << ")\n";
    imprimirEncabezado();

    std::ofstream csvTotales("resultados/base_totales.csv");
    csvTotales << "N,insercion,busqueda,avl_ins_ms,splay_ins_ms,avl_busq_ms,splay_busq_ms\n";
    std::ofstream csvBloques("resultados/base_por_busqueda.csv");
    csvBloques << "N,insercion,busqueda,estructura,bloque,busquedas_acumuladas,tiempo_bloque_ms,ns_por_busqueda\n";

    size_t valoresN[] = {1u << 10, 1u << 11, 1u << 12, 1u << 13, 1u << 14};
    for (size_t N : valoresN) {
        std::vector<uint32_t> dataset = generarDataset(N);
        bool configs[4][2] = {{false, false}, {false, true}, {true, false}, {true, true}};
        for (auto& cfg : configs) {
            ResultadoEscenario r = ejecutarEscenario(N, cfg[0], cfg[1], dataset, rng, csvBloques);
            imprimirResultado(r);
            csvTotales << r.N << "," << r.insercion << "," << r.busqueda << ","
                       << r.avlInsMs << "," << r.splayInsMs << ","
                       << r.avlBusqMs << "," << r.splayBusqMs << "\n";
        }
    }
}

void ejecutarTeoremaAccesoSecuencial(const std::vector<uint32_t>& conjuntoDatos,
                                     const std::vector<uint32_t>& ordenInsercion) {
    size_t N = conjuntoDatos.size();
    std::cout << "\n=== Teorema de acceso secuencial (N=" << N << ") ===\n";

    AVLTree avl;
    SplayTree splay;
    for (uint32_t x : ordenInsercion) { avl.insert(x); splay.insert(x); }

    std::vector<uint32_t> clavesOrdenadas = conjuntoDatos;
    std::sort(clavesOrdenadas.begin(), clavesOrdenadas.end());

    std::ofstream csv("resultados/teorema_secuencial.csv");
    csv << "m,estructura,tiempo_ms,ns_por_busqueda\n";
    std::cout << std::left << std::setw(12) << "m"
              << std::setw(16) << "AVL(ms)" << std::setw(16) << "Splay(ms)" << "\n"
              << std::string(44, '-') << "\n";

    for (size_t k = 1; k <= 10; ++k) {
        size_t m = (k < 10) ? (N * k / 100) : (N / 10);
        if (m == 0) continue;

        std::vector<uint32_t> consultas;
        consultas.reserve(m);
        size_t paso = std::max<size_t>(1, N / m);
        for (size_t i = 0; i < m; ++i)
            consultas.push_back(clavesOrdenadas[std::min(N - 1, i * paso)]);

        double avlMs = medirTiempo([&] {
            for (uint32_t clave : consultas) avl.search(clave);
        });
        double splayMs = medirTiempo([&] {
            for (uint32_t clave : consultas) splay.search(clave);
        });

        csv << m << ",AVL,"   << avlMs   << "," << avlMs * 1e6 / m   << "\n";
        csv << m << ",Splay," << splayMs << "," << splayMs * 1e6 / m << "\n";
        std::cout << std::left << std::setw(12) << m
                  << std::setw(16) << std::fixed << std::setprecision(2) << avlMs
                  << std::setw(16) << splayMs << "\n";
    }
}

void ejecutarTeoremaWorkingSet(const std::vector<uint32_t>& conjuntoDatos,
                               const std::vector<uint32_t>& ordenInsercion,
                               std::mt19937& rng) {
    size_t N = conjuntoDatos.size();
    std::cout << "\n=== Teorema del working set (N=" << N << ") ===\n";

    AVLTree avl;
    SplayTree splay;
    for (uint32_t x : ordenInsercion) { avl.insert(x); splay.insert(x); }

    size_t M = 10 * c * N;
    if (WS_LIMITE_BUSQUEDAS > 0) M = std::min(M, WS_LIMITE_BUSQUEDAS);
    std::cout << "(M = " << M << " busquedas por W)\n";

    std::ofstream csv("resultados/teorema_working_set.csv");
    csv << "W,estructura,busquedas,tiempo_total_ms,ns_por_busqueda\n";
    std::cout << std::left << std::setw(12) << "W"
              << std::setw(16) << "AVL(ms)" << std::setw(16) << "Splay(ms)" << "\n"
              << std::string(44, '-') << "\n";

    std::vector<size_t> tamWorkingSet = {10, 100, 1000, 10000, 100000, 1000000};
    for (size_t W : tamWorkingSet) {
        if (W > N) break;
        std::vector<uint32_t> conjuntoTrabajo;
        conjuntoTrabajo.reserve(W);
        std::sample(conjuntoDatos.begin(), conjuntoDatos.end(),
                    std::back_inserter(conjuntoTrabajo), W, rng);

        std::uniform_int_distribution<size_t> dist(0, conjuntoTrabajo.size() - 1);
        uint32_t semilla = rng();

        std::mt19937 gA(semilla);
        double avlMs = medirTiempo([&] {
            for (size_t i = 0; i < M; ++i) avl.search(conjuntoTrabajo[dist(gA)]);
        });
        std::mt19937 gS(semilla);
        double splayMs = medirTiempo([&] {
            for (size_t i = 0; i < M; ++i) splay.search(conjuntoTrabajo[dist(gS)]);
        });

        csv << W << ",AVL,"   << M << "," << avlMs   << "," << avlMs * 1e6 / M   << "\n";
        csv << W << ",Splay," << M << "," << splayMs << "," << splayMs * 1e6 / M << "\n";
        std::cout << std::left << std::setw(12) << W
                  << std::setw(16) << std::fixed << std::setprecision(2) << avlMs
                  << std::setw(16) << splayMs << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::create_directory("resultados");
    std::random_device rd;
    std::mt19937 rng(rd());

    ejecutarExperimentosBase(rng);

    if (argc > 1 && std::string(argv[1]) == "--theorems") {
        size_t N = 1u << 25;
        std::vector<uint32_t> conjuntoDatos = generarDataset(N);
        std::vector<uint32_t> ordenInsercion = conjuntoDatos;
        std::shuffle(ordenInsercion.begin(), ordenInsercion.end(), rng);
        ejecutarTeoremaAccesoSecuencial(conjuntoDatos, ordenInsercion);
        ejecutarTeoremaWorkingSet(conjuntoDatos, ordenInsercion, rng);
    }
    return 0;
}
