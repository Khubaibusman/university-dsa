#include <iostream>
using namespace std;

class Queue
{
    int cap, size;
    int *arr;
    int head, tail;

public:
    Queue(int c)
    {
        cap = c;
        head = tail = -1;
        size = 0;
        arr = new int[cap];
    }

    bool is_empty()
    {
        return size == 0;
    }

    bool is_full()
    {
        return size == cap;
    }

    void enqueue(int val)
    {
        if (is_full())
        {
            cout << "Queue Overflow!\n";
            return;
        }

        if (head == -1)
            head = 0;

        arr[++tail] = val;
        size++;

        cout << "Value " << val << " added!\n";
    }

    int dequeue()
    {
        if (is_empty())
        {
            cout << "Queue Underflow!\n";
            return -1;
        }

        int val = arr[head++];
        size--;

        if (size == 0)
            head = tail = -1;

        cout << "Value " << val << " removed!\n";
        return val;
    }

    void print()
    {
        if (is_empty())
        {
            cout << "Queue Empty!\n";
            return;
        }

        for (int i = head; i <= tail; i++)
            cout << arr[i] << " ";

        cout << endl;
    }
};

int main()
{
    Queue q(5);
    q.enqueue(5);
    q.enqueue(3);
    q.enqueue(8);
    q.enqueue(1);
    q.enqueue(8);
    q.enqueue(8);

    q.print();
}