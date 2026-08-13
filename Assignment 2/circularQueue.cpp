#include <iostream>
using namespace std;

class C_Queue
{
    int *arr;
    int head, tail, size, cap;

public:
    C_Queue(int c)
    {
        cap = c;
        arr = new int[cap];
        head = tail = -1;
        size = 0;
    }

    bool is_empty()
    {
        return (size == 0);
    }
    bool is_full()
    {
        return (size == cap);
    }

    void enqueue(int val)
    {
        if (is_full())
        {
            cout << "Overflow!\n";
            return;
        }

        if (head == -1)
        {
            head = tail = 0;
            arr[tail] = val;
            size++;
            return;
        }

        tail = (tail + 1) % cap;

        arr[tail] = val;
        size++;
    }

    int dequeue(){
        if(is_empty()){
            cout<<"Underflow!";
            return;
        }

        int a = arr[head];
        head = (head+1)% cap;
        size--;

        return a;

    }
};

int main()
{

    return 0;
}