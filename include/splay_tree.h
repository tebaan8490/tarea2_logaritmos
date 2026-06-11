#pragma once
#include <bits/stdc++.h>
#include "models.h"
#include "utils.h"

class SplayTree {
    private:

        Node* root;

    public:

        explicit SplayTree();
        Node* getRoot() const {return root;}
        Node* rotateLeft(Node* node);
        Node* rotateRight(Node* node);

        Node* zig(Node* node);
        Node* zag(Node* node);
        Node* zigZig(Node* node);
        Node* zigZag(Node* node);
        Node* zagZig(Node* node);
        Node* zagZag(Node* node);

        Node* splay(Node* node);
        Node* insert(Node* node);
        Node* search(Node* node);
};
