import pandas as pd
import matplotlib.pyplot as plt

#================================
# Tiempos de búsqueda
#================================

df = pd.read_csv("search_times.csv")

Ns = [1024, 16384]
escenarios = ["A", "B", "C", "D"]

for N in Ns:

    fig, axes = plt.subplots(
        2,
        2,
        figsize=(14,10)
    )

    axes = axes.flatten()

    for idx, escenario in enumerate(escenarios):

        ax = axes[idx]

        datos = df[
            (df["N"] == N)
            &
            (df["scenario"] == escenario)
        ]

        for arbol in ["AVL", "SPLAY"]:

            sub = datos[
                datos["tree"] == arbol
            ]

            ax.plot(
                sub["search_id"],
                sub["time_ms"],
                label=arbol
            )

        ax.set_title(
            f"Escenario {escenario}"
        )

        ax.set_xlabel(
            "Número de búsqueda"
        )

        ax.set_ylabel(
            "Tiempo (ms)"
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
    "sequential_access.csv"
)

plt.figure(figsize=(8,5))

plt.plot(
    df["m"],
    df["avl_ms"],
    marker="o",
    label="AVL"
)

plt.plot(
    df["m"],
    df["splay_ms"],
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
    "working_set.csv"
)

plt.figure(figsize=(8,5))

plt.plot(
    df["W"],
    df["avl_ms"],
    marker="o",
    label="AVL"
)

plt.plot(
    df["W"],
    df["splay_ms"],
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