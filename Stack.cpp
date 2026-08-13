#include <iostream>
using namespace std;

int stack[5] = {0};
int top = -1;
int Max = 4;

void pop()
{
    if (top == -1)
    {
        cout << "Stack Underflow!" << endl;
    }
    else
    {
        stack[top] = -1;
        top--;
    }
}

void push()
{
    int val;
    if (top == Max)
    {
        cout << "Stack Overflow" << endl;
    }
    else
    {
        cout << "Enter the value you want to add in stack:";
        cin >> val;
        top++;
        stack[top] = val;
    }
}

void display()
{
    cout << "---Stack Diagram---" << endl;
    for (int i = top; i >= 0; i--)
    {
        cout << "|" << stack[i] << "|" << endl;
    }
}

int main()
{
    push();
    push();
    push();
    push();
    push();
    push();
    push();

    display();

    pop();
    pop();
    display();

    pop();
    pop();
    pop();
    pop();

    display();

    return 0;
}