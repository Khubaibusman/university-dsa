#include <iostream>
using namespace std;

class node
{
public:
    int data;
    node *left;
    node *right;

    node(int d)
    {
        data = d;
        left = right = NULL;
    }
};


class BST{
node *root = NULL;


node* insert(node* r,int data){
    if (r == NULL){
        return (new node(data));
    }

    if (data< r->data){
        insert(r->left,data);

    }
    else{
        insert(r->right,data);
    }

    return root;
}

}

int main()
{

    return 0;
}