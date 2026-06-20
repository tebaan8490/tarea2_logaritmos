# Tarea 2 — Árboles AVL vs Splay Trees

CC4102 - Diseño y Análisis de Algoritmos. Implementación y comparación
experimental de un **árbol AVL** y un **Splay Tree** en sus operaciones de
inserción y búsqueda, junto con la verificación empírica del *Sequential Access
Theorem* y el *Working Set Theorem*.

Integrantes:

* André Guinart C.
* Esteban Contreras S.
* Diego Beltrán M.


## Estructura del repositorio

```
include/      Headers (modelos, rotaciones, io, utils)
src/          Implementación en C++ (avl, splay, rotations, io, utils, main)
resultados/   CSV generados por el programa
graphs.py     Genera los gráficos a partir de los CSV
Makefile      Compilación
Tarea2.pdf    Enunciado
```

## Compilación

Requiere `g++` con soporte para C++17.

```bash
make          # genera el ejecutable ./program
make clean    # elimina build/ y el ejecutable
```

## Ejecución

```bash
./program              # escenarios base (N = 2^10 ... 2^14)
./program --theorems   # además corre los teoremas con N = 2^25
```

El programa imprime las tablas de tiempos en consola y escribe los resultados
en la carpeta `resultados/`:

- `base_totales.csv` — tiempos totales de inserción y búsqueda por escenario.
- `base_por_busqueda.csv` — tiempos por bloque de búsquedas (naturaleza amortizada).
- `teorema_secuencial.csv` — Sequential Access Theorem.
- `teorema_working_set.csv` — Working Set Theorem.

> El parámetro `c` (constante de la cantidad de búsquedas `M = 10·c·N`) está
> fijado en `src/main.cpp`.

## Experimentos

**Escenarios base** — para cada `N ∈ {2^10, …, 2^14}` se realizan `N`
inserciones seguidas de `M = 10·c·N` búsquedas, en cuatro configuraciones:

| | Inserción | Búsqueda |
|---|---|---|
| A | Aleatoria | Uniforme |
| B | Aleatoria | Sesgada (exponencial `P(i)`) |
| C | Ordenada  | Uniforme |
| D | Ordenada  | Sesgada |

**Teoremas** (con `N = 2^25`):

- *Sequential Access*: búsquedas de claves estrictamente crecientes. Se espera
  `O(m·log n)` en el AVL y `O(m·log(n/m))` en el Splay Tree.
- *Working Set*: búsquedas restringidas a un subconjunto `W`. Se espera
  `O(log n)` en el AVL y `O(log W)` en el Splay Tree.

## Gráficos

Una vez generados los CSV:

```bash
python3 -m venv .venv && source .venv/bin/activate
pip install pandas matplotlib
python3 graphs.py
```

Genera los `.png` (escenarios base, acceso secuencial y working set).
