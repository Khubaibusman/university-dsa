#include <iostream>
using namespace std;

class node{
public:
    node *next;
    int data;

    node(int v){
        next = NULL;
        data = v;
    }
};

class list{
    node *front;
    node *back;

public:
    list(){
        front = back = NULL;
    }

    void push(int val){
        node *newnode = new node(val);

        if(front == NULL){
            front = back = newnode;
            return;
        }

        back->next = newnode;
        back = newnode;
    }

    void pop(){
        if(front == NULL){
            cout << "Underflow!";
            return;
        }

        if(front == back){
            delete front;
            front = back = NULL;
            return;
        }

        node *temp = front;

        while(temp->next != back){
            temp = temp->next;
        }

        delete back;
        back = temp;
        back->next = NULL;
    }
    void sort_asc(){
        if(front == NULL){
            cout<<"List empty, no sorting!"<<endl;
        }
        if(front == back){
            cout<<"Already one Entity!";
            return;
        }

        node *curr = front;
        node* nxt;

        int temp3;


        while(curr!=NULL){
            nxt = curr->next;
            while(nxt!=NULL){
                if(curr->data<nxt->data){
                    temp3 = curr->data;
                    curr->data = nxt->data;
                    nxt->data = temp3;


                }
                nxt = nxt->next;

               
            }
            curr=curr->next;
        }



    }

    void display(){
        if(front == NULL){
            cout << "Underflow!";
            return;
        }

        node *temp = front;

        while(temp != NULL){
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout<<endl;
    }

    void push_f(int val){
        node* newnode = new node(val);
        if (front == NULL){
            front = back = newnode;
            return;
        }

        newnode->next = front;
        front = newnode;

        
    }

};

int main(){
    list l;

    l.push(5);
    l.push(2);
    l.push(7);
    
    l.display();
    l.pop();
    l.display();
    l.push(3);
    l.push(4);
    l.push(1);
    l.display();

    l.sort_asc();
    l.display();

    l.push_f(3);
    l.push_f(1);

    l.display();

    l.sort_asc();
    l.display();
    return 0;
}