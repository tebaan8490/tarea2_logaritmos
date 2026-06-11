struct Node {
    int value;
    int balance_factor;
    Node* left;
    Node* right;
    Node* parent;
};

struct AVLTree {
    Node* root;
};

struct SplayTree {
    Node* root;
};