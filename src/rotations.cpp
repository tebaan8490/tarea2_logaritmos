#include <rotations.h>

Node* zig(Node* node){
    Node* y = node;
    Node* x = y->left;
    Node* B = x-> right;
    Node* abuelo = y->parent;

    y->left = B;
    if ( B!=nullptr) B->parent = y;
    x->right = y;
    y->parent = x;
    
    x->parent = abuelo;
    if (abuelo != nullptr){
        if (abuelo->left == y) abuelo->left =x;
        else    abuelo->right=x;
    }
    return x;
}

Node* zag(Node* node){
    Node* y=node;
    Node* x= y->right;
    Node* B = x->left;
    Node* abuelo = y->parent;

    y->right = B;
    if (B!=nullptr) B->parent= y;
    x->left=y;
    y->parent = x;

    x->parent = abuelo;
    if (abuelo != nullptr){
        if (abuelo->left == y) abuelo->left =x;
        else    abuelo->right=x;
    }
    return x;
}

Node* zigzag(Node* node){
    zag(node->left);
    return zig(node);
}

Node* zagzig(Node* node){
    zig(node->right);
    return zag(node);
}

Node* zagzag(Node* node){
    return zag(zag(node));
}

Node* zigzig(Node* node){
    return zig(zig(node));
}