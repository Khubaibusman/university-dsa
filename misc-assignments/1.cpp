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

class list
{
    node *head;
    node *tail;

public:
    list()
    {
        head = tail = NULL;
    }

    // O(1)
    void add_tail(int val)
    {
        node *newNode = new node(val);
        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        cout << "Value (" << val << ") Added to List.\n";
    }

    // O(n)
    void print_list()
    {
        if (head == NULL)
        {
            cout << "List Empty!\n";
        }
        else
        {
            node *temp = head;

            while (temp != NULL)
            {
                cout << "[" << temp->data << "] -> ";
                temp = temp->next;
            }
            cout << "NULL\n";
        }
    }

    // O(1)
    void add_head(int val)
    {
        node *newNode = new node(val);

        if (head == NULL)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
        cout << "Value " << newNode->data << " Added before head\n";
    }

    // O(n)
    void delete_tail()
    {
        if (head == NULL)
        {
            cout << "List already empty!\n";
            return;
        }
        else if (head == tail)
        {
            cout << "Value " << head->data << " deleted!\n";
            delete head;
            head = tail = NULL;
            return;
        }
        else
        {
            node *temp = head;
            while (temp->next != tail)
            {
                temp = temp->next;
            }
            cout << "Value " << tail->data << " deleted!\n";
            delete tail;
            tail = temp;
            tail->next = NULL;
        }
    }

    // O(1)
    int delete_head()
    {
        if (head == NULL)
        {
            cout << "List already empty!\n";
            return -1;
        }
        else if (head == tail)
        {
            int a = head->data;
            delete head;
            head = tail = NULL;
            return a;
        }
        else
        {
            int a = head->data;
            node *temp = head;
            head = head->next;
            delete temp;
            return a;
        }
    }

    // O(n)
    bool delete_node(int key)  
    {
        if (head == NULL)
            return false;

        if (head->data == key)
        {
            delete_head();
            return true;
        }

        node *temp = head;

        while (temp->next != NULL)
        {
            if (temp->next->data == key)
            {
                node *del = temp->next;
                temp->next = del->next;

                if (del == tail)
                    tail = temp;

                delete del;
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // O(n)
    bool search(int val)
    {
        if (head == NULL)
        {
            cout << "List Empty!\n";
            return false;
        }
        node *temp = head;
        while (temp != NULL)
        {
            if (temp->data == val)
            {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // O(n)
    void add_value_middle(int val, int key)
    {
        if (head == NULL)
        {
            cout << "List Empty!\n";
            return;
        }
        else
        {
            node *temp = head;
            while (temp != NULL)
            {
                if (key == temp->data)
                {
                    node *newN = new node(val);
                    newN->next = temp->next;
                    temp->next = newN;

                    if (temp == tail)
                        tail = newN;

                    cout << "Value added!\n";
                    return;
                }
                temp = temp->next;
            }
            cout << "Key not found!\n";
        }
    }

    // O(1)
    int See_head()
    {
        if (head == NULL)
            return -1;

        return head->data;
    }

    // O(1)
    bool is_empty()
    {
        return head == NULL;
    }
};


class Stack
{
    list stack;

public:
    // O(1)
    void push(int val)
    {
        stack.add_head(val);
    }

    // O(1)
    int pop()
    {
        if (stack.is_empty())
        {
            cout << "Stack Underflow!\n";
            return -1;
        }
        return stack.delete_head();
    }

    // O(1)
    int top() 
    {
        return stack.See_head();
    }

    // O(1)
    bool empty()
    {
        return stack.is_empty();
    }

    // O(n)
    void print()
    {
        stack.print_list();
    }
};


class queue
{
    list Queue;

public:
    // O(1)
    void enqueue(int val)
    {
        Queue.add_tail(val);
    }

    // O(1)
    int dequeue()
    {
        if (Queue.is_empty())
        {
            cout << "Queue Underflow!\n";
            return -1;
        }
        return Queue.delete_head();
    }

    // O(1)
    int Front()   
    {
        return Queue.See_head();
    }

    // O(1)
    bool IsEmpty()   
    {
        return Queue.is_empty();
    }

    // O(n)
    void print()
    {
        if (Queue.is_empty())
        {
            cout << "Queue Empty!\n";
            return;
        }
        Queue.print_list();
    }
};


int main()
{
    cout << "===== LINKED LIST TEST =====\n";

    list l1;

    l1.add_tail(4);
    l1.add_tail(1);
    l1.add_tail(5);
    l1.print_list();

    l1.add_head(8);
    l1.add_head(4);
    l1.print_list();

    l1.add_value_middle(9, 1); 
    l1.print_list();

    cout << "Search 5: " << (l1.search(5) ? "Found\n" : "Not Found\n");

    l1.delete_head();
    l1.print_list();

    l1.delete_tail();
    l1.print_list();

    l1.delete_node(1); 
    l1.print_list();

    cout << "Head Value: " << l1.See_head() << endl;


    cout << "\n===== STACK TEST =====\n";

    Stack s1;

    s1.push(5);
    s1.push(10);
    s1.push(15);

    s1.print();

    cout << "Top: " << s1.top() << endl;

    cout << "Pop: " << s1.pop() << endl;
    cout << "Pop: " << s1.pop() << endl;

    s1.print();

    cout << "Is Empty: " << (s1.empty() ? "Yes\n" : "No\n");

    s1.pop();
    s1.pop();


    cout << "\n===== QUEUE TEST =====\n";

    queue q1;

    q1.enqueue(5);
    q1.enqueue(3);
    q1.enqueue(7);

    q1.print();

    cout << "Front: " << q1.Front() << endl;

    cout << "Dequeue: " << q1.dequeue() << endl;
    cout << "Dequeue: " << q1.dequeue() << endl;

    q1.print();

    cout << "Is Empty: " << (q1.IsEmpty() ? "Yes\n" : "No\n");

    q1.dequeue();
    q1.dequeue(); 

    return 0;
}