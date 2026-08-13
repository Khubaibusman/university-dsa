#include <iostream>
using namespace std;

class node
{
public:
    node *next;
    int data;

    node(int val)
    {
        data = val;
        next = NULL;
    }
};

class list{
    node* head;
    node* tail;
    public:

    list(){
        head = tail = NULL;
    }

    void add_head(int val){
        node* newnode = new node (val);
        if (head == NULL){
            head = tail = newnode;
            return;
        }
        newnode -> next = head;
        head = newnode;
        
    }

    void add_tail(int val){
        node * newnode = new node(val);
        if (head == NULL){
            head = tail = newnode;
            return;
        }
        tail->next = newnode;
        tail = newnode;

    }
    bool is_empty(){
        if (head == NULL){
            return true;
        }
        return false;
    }

    void display(){
        if(is_empty()){
            cout<<"Empty List!\n";
            return;
        }
        node *temp = head;
        while(temp->next != NULL){
            cout<<temp->data<<" ";
            temp = temp->next;
        }
        cout<<tail->data<<endl;
    }

    void reverse(){
        node * curr = head;
        node * next;
        node * prev = NULL;
        tail = curr;

        while (curr != NULL){
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head = prev;
        

      

    }

};

int main()
{

    list l;

    l.add_head(10);
    l.add_head(20);
    l.add_tail(30);
    l.add_tail(40);

    l.display();
    l.reverse();
    l.display();



  
    return 0;
}