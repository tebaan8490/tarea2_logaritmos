#include <models.h>
#include <rotations.h>
#include <algorithm>

int height(Node *n){return n==nullptr ? 0: n->height;}
int bf(Node* n){return height(n->left)-height(n->right);}

void updateHeight(Node *n){
    n->height = 1 + std::max(height(n->left), height(n->right));
}

void AVLTree::insert(uint32_t x){
    root = insertRec(root,x);
}

Node* AVLTree::insertRec(Node* node, uint32_t x){
    if (node ==nullptr){return new Node{x,1,0, nullptr, nullptr,nullptr};}

    if (x<node->value){
        node->left= insertRec(node->left,x);
    }
    else if (x > node->value){
        node->right = insertRec(node->right,x);
    } else {
        return node;
    }

    updateHeight(node);
    int b = bf(node);
    Node* nuevaRaiz = node;
    if (b == 2){
        if (bf(node->left) < 0) node->left = zag(node->left);
        nuevaRaiz = zig(node);
        updateHeight(nuevaRaiz->left);
        updateHeight(nuevaRaiz->right);
        updateHeight(nuevaRaiz);
    } else if (b == -2){
        if (bf(node->right) > 0) node->right = zig(node->right);
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