#include <cstdint>
#pragma once

struct Node {
    uint32_t value;
    int height;
    int balance_factor;
    Node* left;
    Node* right;
    Node* parent;
};

struct AVLTree {
    Node* root;
    AVLTree(): root(nullptr) {}
    void insert(uint32_t x);
    bool search(uint32_t x);

private:
    Node* insertRec(Node* node, uint32_t x);
};

struct SplayTree {
    Node* root;
    SplayTree() : root(nullptr){}
};