#include <models.h>
#include <rotations.h>
#include <algorithm>

/**
* @brief Función que muestra la altura de un nodo
* 
* @param n Nodo al cual se muestra su campo "height"
* @return La altura del nodo
*/
int height(Node *n){return n==nullptr ? 0: n->height;}

/**
* @brief Calculador del Balance Factor de un nodo
* 
* @param n Nodo al cual se calcula su BF
* @return El Balance Factor del nodo
*/
int bf(Node* n){return height(n->left)-height(n->right);}


/**
* @brief Actualiza la altura de un nodo
* 
* @param n Nodo al cual se le actualiza su altura
* @return Actualización de su altura
*/
void updateHeight(Node *n){
    n->height = 1 + std::max(height(n->left), height(n->right));
}

void AVLTree::insert(uint32_t x){
    root = insertRec(root,x);
    root->parent = nullptr;
}

Node* AVLTree::insertRec(Node* node, uint32_t x){
    if (node ==nullptr){return new Node{x,1,0, nullptr, nullptr,nullptr};}

    if (x<node->value){
        node->left= insertRec(node->left,x);
        node->left->parent = node;
    }
    else if (x > node->value){
        node->right = insertRec(node->right,x);
        node->right->parent = node;
    } else {
        return node;
    }

    updateHeight(node);
    int b = bf(node);
    Node* nuevaRaiz = node;
    if (b == 2){
        if (bf(node->left) < 0) zag(node->left);
        nuevaRaiz = zig(node);
        updateHeight(nuevaRaiz->left);
        updateHeight(nuevaRaiz->right);
        updateHeight(nuevaRaiz);
    } else if (b == -2){
        if (bf(node->right) > 0) zig(node->right);
        nuevaRaiz = zag(node);
        updateHeight(nuevaRaiz->left);
        updateHeight(nuevaRaiz->right);
        updateHeight(nuevaRaiz);
    }
    return nuevaRaiz;
}

bool AVLTree::search(uint32_t x){
    Node* actual =root;
    while (actual!=nullptr){
        if (x==actual->value) return true;
        if (x<actual->value) actual = actual->left;
        else    actual = actual->right;
    }
    return false;
}