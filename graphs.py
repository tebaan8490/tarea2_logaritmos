import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data.csv")

Ns = [1024, 16384]
scenarios = ["A", "B", "C", "D"]

for scenario in scenarios:

    for N in Ns:

        sub = df[
            (df["scenario"] == scenario)
            &
            (df["N"] == N)
        ]

        plt.figure(figsize=(10,5))

        for tree in ["AVL", "SPLAY"]:

            datos = sub[
                sub["tree"] == tree
            ]

            plt.plot(
                datos["search_id"],
                datos["time_ms"],
                label=tree
            )

        plt.title(
            f"Escenario {scenario} - N={N}"
        )

        plt.xlabel("Búsqueda")
        plt.ylabel("Tiempo (ms)")
        plt.grid(True)
        plt.legend()

        plt.tight_layout()

        plt.savefig(
            f"scenario_{scenario}_{N}.png",
            dpi=150
        )

        plt.close()