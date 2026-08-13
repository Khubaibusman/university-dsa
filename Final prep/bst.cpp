#include <iostream>
using namespace std;

class node
{

public:
    int data;
    node *left;
    node *right;

    node()
    {
        left = right = nullptr;
        data = 0;
    }

    node(int data)
    {
        this->data = data;
        left = right = nullptr;
    }
};

class BST
{
    node *root;

    node *insert(node *Node, int d)
    {
        if (Node == nullptr)
        {
            return new node(d);
        }
        if (Node->data > d)
        {
            Node->left = insert(Node->left, d);
        }
        else
        {
            Node->right = insert(Node->right, d);
        }

        return Node;
    }

    void inorder(node *Node)
    {
        if (Node == nullptr)
        {
            return;
        }

        inorder(Node->left);
        cout << Node->data << " ";
        inorder(Node->right);
    }

    void preorder(node *Node)
    {
        if (Node == nullptr)
        {
            return;
        }
        cout << " C ";
        cout << Node->data << " ";
        cout << " L ";
        preorder(Node->left);
        cout << " R ";
        preorder(Node->right);
    }
    bool Search(int key, node *Node)
    {
        if (Node == nullptr)
        {
            return false;
        }

        if (Node->data == key)
        {
            return true;
        }
        if (Node->data < key){
            return Search(key,Node->right);
        }

        else if (Node->data > key){
            return Search(key,Node->left);
        }
        
    }

    node* Delete(int key, node *Node)
    {
        if (Node == nullptr)
        {
            return nullptr;
        }

        if (Node->data < key){
            return Delete(key,Node->right);
        }

        else if (Node->data > key){
            return Delete(key,Node->left);
        }


        else{
            if(Node->left == nullptr && Node->right == nullptr){
                delete Node;
                return nullptr;


            }

            else if(Node->left == nullptr){
                node* temp = Node->right;
                delete Node;

                return temp;
                

            }

            else if (Node->right == nullptr){
                node*temp = Node->left;
                delete Node;
                return temp;
            }

            else {
                
            }
        }
        
    }




public:
    BST()
    {
        root = nullptr;
    }

    void insert(int d)
    {
        root = insert(root, d);
    }
    void display()
    {
        inorder(root);
    }

    void search(int key)
    {
        if (Search(key, root)){
            cout<<"Value Found!\n";
        }

        else{
            cout<<"Value Not Found!\n";
        }
    }
};

int main()
{

    BST T1;
    T1.insert(50);
    T1.insert(40);
    T1.insert(60);
    T1.insert(80);
    T1.insert(90);
    T1.insert(20);

    T1.display();

    T1.search(20);
    return 0;
}