#include <iostream>
#include <models.h>


void inOrden(Node* n){
    if (n==nullptr) return;
    inOrden(n->left);
    std::cout << n->value<<" ";
    inOrden(n->right);
}

int main() {
    Node n;
    n.value = 42;
    std::cout << "Funciona: " << n.value << "\n";
    
    AVLTree avl;
    for (uint32_t i = 1; i<=10; i++){
        avl.insert(i);
    }

    std::cout << "Inorden: ";
    inOrden(avl.root);
    std::cout << "\n";

    std::cout << "Altura raiz: " << avl.root->height << "\n";

    std::cout << "Busca 7: " << avl.search(7) << "\n";
    std::cout << "Busca 99: " << avl.search(99) << "\n";

    return 0;
}