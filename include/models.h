#include <cstdint>
#pragma once

/**
 * @brief Estructura que representa un Nodo de los árboles AVL y Splay
 * 
 * Cada nodo contiene:
 * - value: Valor almacenado en el nodo
 * - height: Altura del nodo
 * - balance_factor: Diferencia entre alturas de los subarboles del nodo
 * - left: Hijo izquierdo
 * - right: Hijo derecho
 * - parent: El nodo padre
 */
struct Node {
    uint32_t value;
    int height;
    int balance_factor;
    Node* left;
    Node* right;
    Node* parent;
};

/**
 * @brief Estructura que representa un árbol AVL
 */     
struct AVLTree {

    /** Raíz del AVL*/
    Node* root;

    AVLTree(): root(nullptr) {}

    /**
    * @brief Función que marca el comienzo de la inserción recursiva en un AVL
    *
    * Llama a otra función privada para comenzar la recursión, y poner la nueva raíz
    * 
    * @param x Valor a insertar en el árbol
    * @return Inserción de nodo en el árbol
    */
    void insert(uint32_t x);

    /**
    * @brief Verifica si el nodo con valor "x" existe en el AVL
    *
    * 
    * @param x Valor a buscar en el árbol
    * @return True si el nodo con valor "x" está en el árbol, False en caso contrario
    */
    bool search(uint32_t x);

private:

    /**
    * @brief Inserta un nodo con valor "x" en el AVL
    *
    * Se autobalancea luego de la inserción recursivamente, con rotaciones y el BF
    * 
    * @param node Nodo de la iteración actual
    * @param x Valor a insertar en el árbol
    * @return Nodo recién rotado (si no era una hoja) o insertado (si es una hoja donde poner el valor "x")
    */
    Node* insertRec(Node* node, uint32_t x);
};

/**
 * @brief Estructura que representa un SplayTree
 */    
struct SplayTree {

    /** Raíz del SplayTree*/
    Node* root;
    SplayTree() : root(nullptr){}

    /**
    * Función usada en una versión antigua del repó, pero ya no (y me da cosa sacarla a estas alturas)
    */
    Node* insertRec(Node* node, uint32_t x);

    /**
    * @brief Inserta un nodo con valor "x" en el SplayTree
    *
    * Se llama a splay() luego de la inserción para balancear el árbol
    * 
    * @param x Valor a insertar en el árbol
    * @return Inserción del nodo
    */
    void insert(uint32_t x);

    /**
    * @brief Verifica si el nodo con valor "x" existe en el SplayTree
    *
    * Llama a splay() luego de la búsqueda. Se aplica al nodo tal que value == x, o al
    * último nodo visitado en la búsqueda, en caso de que no se haya encontrado el valor
    * 
    * @param x Valor a buscar en el árbol
    * @return True si el nodo con valor "x" está en el árbol, False en caso contrario
    */
    bool search(uint32_t x);

private:

    /**
    * @brief Balancea el árbol luego de una inserción/búsqueda, de abajo hacia arriba
    *
    * Implementa un ciclo, y aprovecha el campo "parent" de los nodos para hacer el splay
    * desde abajo del árbol hacia arriba
    * 
    * @param node Nodo desde el cual parte el splay
    * @return El SplayTree balanceado
    */
    void splay(Node* node);   // bottom-up iterativo (dobles + posible simple)
};