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