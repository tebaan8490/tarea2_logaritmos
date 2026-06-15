#include <models.h>
#include <rotations.h>

void SplayTree::insert(uint32_t x){
    if (root == nullptr){
        root = new Node{x, 1, 0, nullptr, nullptr, nullptr};
        return;
    }
    Node* actual = root;
    Node* padre = nullptr;
    while (actual != nullptr){
        padre = actual;
        if (x < actual->value)
            actual = actual->left;
        else if (x > actual->value)
            actual = actual->right;
        else {
            splay(actual);
            return;
        }
    }

    Node* nuevo = new Node{x, 1, 0, nullptr, nullptr, nullptr};
    nuevo->parent = padre;
    if (x < padre->value)
        padre->left = nuevo;
    else
        padre->right = nuevo;
    splay(nuevo);
}

bool SplayTree::search(uint32_t x){
    Node* actual = root;
    Node* ultimo = nullptr;
    while (actual != nullptr){
        ultimo = actual;
        if (x == actual->value){ splay(actual); return true; }
        if (x < actual->value) actual = actual->left;
        else                   actual = actual->right;
    }
    if (ultimo != nullptr) splay(ultimo);
    return false;
}

void SplayTree::splay(Node* node){
    while (node->parent != nullptr){
        Node* p = node->parent;
        Node* g = p->parent;

        if (g == nullptr){
            if (p->left == node) zig(p);
            else                 zag(p);
        }
        else if (g->left == p && p->left == node)   zigzig(g);
        else if (g->right == p && p->right == node) zagzag(g);
        else if (g->left == p && p->right == node)  zigzag(g);
        else                                        zagzig(g);
    }
    root = node;
}
