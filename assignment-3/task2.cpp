#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

class BST {
public:
    Node* root;

    BST() {
        root = nullptr;
    }

    Node* makeBST(int arr[], int start, int end) {
        if (start > end)
            return nullptr;

        int mid = (start + end) / 2;

        Node* temp = new Node(arr[mid]);

        temp->left = makeBST(arr, start, mid - 1);
        temp->right = makeBST(arr, mid + 1, end);

        return temp;
    }

    void build(int arr[], int size) {
        root = makeBST(arr, 0, size - 1);
    }

    void print(Node* node) {
        if (node == nullptr)
            return;

        print(node->left);
        cout << node->data << " ";
        print(node->right);
    }

    void show() {
        print(root);
        cout << endl;
    }
};

int main() {
    BST tree;

    int arr[] = {1, 2, 3, 4, 5, 6, 7};
    int size = sizeof(arr) / sizeof(arr[0]);

    tree.build(arr, size);

    tree.show();

    return 0;
}