# include <models.h>
# pragma once

/**
* @brief Implementación de rotación base "zig"
* 
* Maneja la lógica para poner correctamente los padres
* de los nodos rotados luego del zig
*
* @param node Nodo en la raíz, el cual se quiere rotar con "zig"
* @return El otro nodo que ahora está en la raíz, producto del "zig"
*/
Node* zig(Node* node);

/**
* @brief Implementación de rotación base "zag"
* 
* Maneja la lógica para poner correctamente los padres
* de los nodos rotados luego del zag
*
* @param node Nodo en la raíz, el cual se quiere rotar con "zag"
* @return El otro nodo que ahora está en la raíz, producto del "zag"
*/
Node* zag(Node* node);

/**
* @brief Implementación de rotación doble "zigZag"
*
* @param node Nodo en la raíz, al cual se le aplica la rotación
* @return El nodo de más abajo que ahora está en la raíz, producto del "zigZag"
*/
Node* zigzag(Node* node);

/**
* @brief Implementación de rotación doble "zagZig"
*
* @param node Nodo en la raíz, al cual se le aplica la rotación
* @return El nodo de más abajo que ahora está en la raíz, producto del "zagZig"
*/
Node* zagzig(Node* node);

/**
* @brief Implementación de rotación doble "zagZag"
*
* @param node Nodo en la raíz, al cual se le aplica la rotación
* @return El nodo de más abajo que ahora está en la raíz, producto del "zagZag"
*/
Node* zagzag(Node* node);

/**
* @brief Implementación de rotación doble "zigZig"
*
* @param node Nodo en la raíz, al cual se le aplica la rotación
* @return El nodo de más abajo que ahora está en la raíz, producto del "zigZig"
*/
Node* zigzig(Node* node);
