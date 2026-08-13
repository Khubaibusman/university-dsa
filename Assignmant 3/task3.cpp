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

class Tree {
public:
    Node* root;

    Tree() {
        root = nullptr;
    }

    int maxPath(Node* node, int &ans) {
        if (node == nullptr)
            return 0;

        int left = max(0, maxPath(node->left, ans));
        int right = max(0, maxPath(node->right, ans));

        ans = max(ans, node->data + left + right);

        return node->data + max(left, right);
    }

    int getMaxSum() {
        int ans = -100000;
        maxPath(root, ans);
        return ans;
    }
};