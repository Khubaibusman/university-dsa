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
    int count;
    int ans;

    BST() {
        root = nullptr;
        count = 0;
        ans = -1;
    }

    void inorder(Node* node, int k) {
        if (node == nullptr)
            return;

        inorder(node->left, k);

        count++;
        if (count == k)
            ans = node->data;

        inorder(node->right, k);
    }

    int kthSmallest(int k) {
        count = 0;
        ans = -1;
        inorder(root, k);
        return ans;
    }
};