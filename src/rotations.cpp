#include <rotations.h>

Node* zig(Node* y){
    Node* x = y->left;
    Node* B = x->right;
    x->right = y;
    y->left = B;
    return x;
}

Node* zag(Node* y){
    Node* x = y->right;
    Node* B = x->left;
    x->left = y;
    y->right = B;
    return x;
}

Node* zigzag(Node* node){ node->left  = zag(node->left);  return zig(node); }
Node* zagzig(Node* node){ node->right = zig(node->right); return zag(node); }
Node* zagzag(Node* node){ return zag(zag(node)); }
Node* zigzig(Node* node){ return zig(zig(node)); }
