#include <iostream>
using namespace std;

class Node {
public:
    int data;
    Node* l_Child;
    Node* r_Child;

    Node(int val) {
        data = val;
        l_Child = nullptr;
        r_Child = nullptr;
    }
};

class BST {
public:
    Node* root;

    BST() {
        root = nullptr;
    }

    Node* insertion(Node* node, int val) {
        if (node == nullptr)
            return new Node(val);

        if (val < node->data)
            node->l_Child = insertion(node->l_Child, val);
        else if (val > node->data)
            node->r_Child = insertion(node->r_Child, val);

        return node;
    }

    bool find(Node* node, int val) {
        if (node == nullptr)
            return false;

        if (node->data == val)
            return true;

        if (val < node->data)
            return find(node->l_Child, val);

        return find(node->r_Child, val);
    }

    void inOrder(Node* node) {
        if (node == nullptr)
            return;

        inOrder(node->l_Child);
        cout << node->data << " ";
        inOrder(node->r_Child);
    }

    void preOrder(Node* node) {
        if (node == nullptr)
            return;

        cout << node->data << " ";
        preOrder(node->l_Child);
        preOrder(node->r_Child);
    }

    void postOrder(Node* node) {
        if (node == nullptr)
            return;

        postOrder(node->l_Child);
        postOrder(node->r_Child);
        cout << node->data << " ";
    }

    void insert(int val) {
        root = insertion(root, val);
    }

    bool search(int val) {
        return find(root, val);
    }

    void displayInOrder() {
        inOrder(root);
        cout << endl;
    }

    void displayPreOrder() {
        preOrder(root);
        cout << endl;
    }

    void displayPostOrder() {
        postOrder(root);
        cout << endl;
    }
};

int main() {
    BST tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.displayInOrder();
    tree.displayPreOrder();
    tree.displayPostOrder();

    cout << tree.search(40) << endl;
    cout << tree.search(100) << endl;

    return 0;
}