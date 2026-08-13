#include <iostream>
using namespace std;

class nodes
{
public:
    int data;
    nodes *next;

    nodes(int val)
    {
        data = val;
        next = NULL;
    }
};

class LL
{
    nodes *head;
    nodes *tail;

public:
    LL()
    {
        head = tail = NULL;
    }

    void push_f(int val)
    {
        nodes * newNode = new nodes(val);

        if (head == NULL){
            head = tail = newNode;
            return;
        }
        else{
            newNode->next = head;
            head = newNode;

        }
    }

    void push_b(int val){
        nodes * newNode = new nodes(val);

        if (tail == NULL){
            tail = head = newNode;
            return;
        }

        else{
            tail->next = newNode;
            tail = newNode;

        }

    }

    void print(){
        nodes * temp = head;

        while (temp != NULL){
            cout<<temp->data<<"-> ";
            temp = temp->next;
        }

        cout<<"NULL"<<endl;
       
        

    }

    void del_f(){
        nodes* temp = head;
        if (head == NULL){
            cout<<"Underflow!"<<endl;
            return;
        }

        else if(head == tail){
            head = tail = NULL;
        }
        else{
            head = temp->next;
        }

        delete temp;

    }
    void del_b(){
        if (tail == NULL){
            cout<<"Underflow!"<<endl;
            return;
        }

        else if(head == tail){
            head = tail = NULL;
        }
        else{
            nodes* temp = head;

            while (temp->next != tail){

                temp = temp->next;

            }

            delete tail;
            tail = temp;
            tail->next = NULL;
        }

    }


};

int main()
{

    LL l1;

    l1.push_b(4);
    l1.push_f(1);
    l1.push_f(3);
    l1.push_f(5);
    l1.push_b(8);
    
    l1.print();
    l1.del_f();
    l1.print();
    l1.del_b();
    l1.print();

    return 0;
}