#include <models.h>
#include <rotations.h>
#include <algorithm>


void SplayTree::splay(uint32_t x) {
    this->root = splayRec(this->root, x);
}

Node* SplayTree::splayRec(Node* root, uint32_t x) {
    if (root == nullptr || root->value == x)
        return root;
    
    // x en el lado izquierdo
    if (x <= root->value) {
        if (root->left == nullptr)
            return root;
        // zigZig
        if (x <= root->left->value) {
            // recursion a la rama izquierda de root->left
            // splay a este subarbol para continuar recursion
            root->left->left = splayRec(root->left->left, x);
            // una vez termina la recursion, zigZig para trasladar el valor a la raiz
            root = zig(root);  

            return (root->left == nullptr)
            ? root
            : zig(root);  
        }
        // zigZag 
        else {
            // recursion a la rama derecha de root->left
            // splay a este subarbol para continuar recursion
            root->left->right = splayRec(root->left->right, x);
            // una vez termina la recursion, zigZag para trasladar el valor a la raiz
            if (root->left->right != nullptr)
                root->left = zag(root->left);

            return (root->left == nullptr)
            ? root
            : zig(root);
        }
    }
    // x en el lado derecho
    else {
        if (root->right == nullptr)
            return root;
        // zagZig
        if (x <= root->right->value) {
            // recursion a la rama izquierda de root->right
            // splay a este subarbol para continuar recursion
            root->right->left = splayRec(root->right->left, x);
            // una vez termina la recursion, zagZig para trasladar el valor a la raiz
            if (root->right->left != nullptr)
                root->right = zig(root->right);

            return (root->right == nullptr)
            ? root
            : zag(root);
        }
        // zagZag 
        else {
        // recursion a la rama derecha de root->right
            // splay a este subarbol para continuar recursion
            root->right->right = splayRec(root->right->right, x);
            // una vez termina la recursion, zagZag para trasladar el valor a la raiz
            root = zag(root);
            return (root->right == nullptr)
            ? root
            : zag(root);  
        }
    }
            
    return root;
}

void SplayTree::insert(uint32_t x) {
            insertRec(this->root, x);
}

Node* SplayTree::insertRec(Node* root, uint32_t x) {
    if (root == nullptr) {
        this->root = new Node{x, 0, 0, nullptr, nullptr, nullptr};
        return this->root;
    }

    if (root->value >= x) {
        if (root->left == nullptr) {
            root->left = new Node{x, 0, 0, nullptr, nullptr, nullptr};
            splay(x);
            return root->left;
        }
           return insertRec(root->left, x);
    }
    else {
        if (root->right == nullptr) {
            root->right = new Node{x, 0, 0, nullptr, nullptr, nullptr};
            splay(x);
            return root->right;
        }
        return insertRec(root->right, x);
    }
}

Node* SplayTree::search(uint32_t x) {
    Node* searched = searchRec(this->root, x);
    splay(searched->value);
    return this->root;
}

Node* SplayTree::searchRec(Node* root, uint32_t x) {

    if (root->value == x) {
        return root;
    }
    else if (root->value > x) {
        if (root->left == nullptr)
            return root;
        else 
            return searchRec(root->left, x);
    }
    else {
        if (root->right == nullptr)
            return root;
        else 
            return searchRec(root->right, x);
    }
}

