import pandas as pd
import matplotlib.pyplot as plt

#================================
# Tiempos de búsqueda (escenarios base)
#================================

df = pd.read_csv("resultados/base_por_busqueda.csv")

Ns = [1024, 16384]

# Cada escenario base es una combinación de tipo de inserción y búsqueda.
escenarios = {
    "A": ("Aleatoria", "Uniforme"),
    "B": ("Aleatoria", "Sesgada"),
    "C": ("Ordenada", "Uniforme"),
    "D": ("Ordenada", "Sesgada"),
}

for N in Ns:

    fig, axes = plt.subplots(
        2,
        2,
        figsize=(14,10)
    )

    axes = axes.flatten()

    for idx, (escenario, (insercion, busqueda)) in enumerate(escenarios.items()):

        ax = axes[idx]

        datos = df[
            (df["N"] == N)
            &
            (df["insercion"] == insercion)
            &
            (df["busqueda"] == busqueda)
        ]

        for arbol in ["AVL", "Splay"]:

            sub = datos[
                datos["estructura"] == arbol
            ]

            ax.plot(
                sub["busquedas_acumuladas"],
                sub["ns_por_busqueda"],
                label=arbol
            )

        ax.set_title(
            f"Escenario {escenario}: {insercion} / {busqueda}"
        )

        ax.set_xlabel(
            "Búsquedas acumuladas"
        )

        ax.set_ylabel(
            "Tiempo por búsqueda (ns)"
        )

        ax.grid(True)
        ax.legend()

    fig.suptitle(
        f"Experimentos base - N={N}"
    )

    plt.tight_layout()

    plt.savefig(
        f"escenarios_N_{N}.png",
        dpi=200
    )

    plt.close()

#================================
# Squential Access Theorem
#================================

df = pd.read_csv(
    "resultados/teorema_secuencial.csv"
)

avl = df[df["estructura"] == "AVL"]
splay = df[df["estructura"] == "Splay"]

plt.figure(figsize=(8,5))

plt.plot(
    avl["m"],
    avl["tiempo_ms"],
    marker="o",
    label="AVL"
)

plt.plot(
    splay["m"],
    splay["tiempo_ms"],
    marker="o",
    label="Splay"
)

plt.title(
    "Sequential Access Theorem"
)

plt.xlabel("m")
plt.ylabel("Tiempo (ms)")

plt.grid(True)
plt.legend()

plt.tight_layout()

plt.savefig(
    "sequential_access.png",
    dpi=200
)

plt.show()

#================================
# Working set Theorem
#================================
df = pd.read_csv(
    "resultados/teorema_working_set.csv"
)

avl = df[df["estructura"] == "AVL"]
splay = df[df["estructura"] == "Splay"]

plt.figure(figsize=(8,5))

plt.plot(
    avl["W"],
    avl["tiempo_total_ms"],
    marker="o",
    label="AVL"
)

plt.plot(
    splay["W"],
    splay["tiempo_total_ms"],
    marker="o",
    label="Splay"
)

plt.xscale("log")

plt.title(
    "Working Set Theorem"
)

plt.xlabel("W")
plt.ylabel("Tiempo (ms)")

plt.grid(True)
plt.legend()

plt.tight_layout()

plt.savefig(
    "working_set.png",
    dpi=200
)

plt.show()
