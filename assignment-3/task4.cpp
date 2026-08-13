#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = right = nullptr;
    }
};

class BST {
public:
    Node* root;
    Node* head;
    Node* prev;

    BST() {
        root = nullptr;
        head = nullptr;
        prev = nullptr;
    }

    void convert(Node* node) {
        if (node == nullptr)
            return;

        convert(node->left);

        if (prev == nullptr) {
            head = node;
        } else {
            prev->right = node;
            node->left = prev;
        }

        prev = node;

        convert(node->right);
    }

    Node* makeCircular() {
        convert(root);

        if (head == nullptr)
            return nullptr;

        Node* tail = prev;

        head->left = tail;
        tail->right = head;

        return head;
    }
};