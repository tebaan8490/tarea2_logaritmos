#include <models.h>
#include <rotations.h>
#include <algorithm>


void SplayTree::splay(uint32_t x) {
    this->root = splayRec(this->root, x);
}

Node* SplayTree::splayRec(Node* root, uint32_t x) {
    if (root == nullptr || root->value == x)
        return root;

    if (x < root->value) {
        if (root->left == nullptr)
            return root;

        if (x < root->left->value) {
            root->left->left = splayRec(root->left->left, x);
            root = zig(root);
        } else if (x > root->left->value) {
            root->left->right = splayRec(root->left->right, x);
            if (root->left->right != nullptr)
                root->left = zag(root->left);
        }

        return (root->left == nullptr) ? root : zig(root);
    }
    else {
        if (root->right == nullptr)
            return root;

        if (x > root->right->value) {
            root->right->right = splayRec(root->right->right, x);
            root = zag(root);
        } else if (x < root->right->value) {
            root->right->left = splayRec(root->right->left, x);
            if (root->right->left != nullptr)
                root->right = zig(root->right);
        }
        return (root->right == nullptr) ? root : zag(root);
    }
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
    if (this->root == nullptr) return nullptr;
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

