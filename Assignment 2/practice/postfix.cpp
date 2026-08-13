#include <iostream>
#include <string>
using namespace std;

class stack
{
    char *arr;
    int top;
    int cap;

public:
    stack(int c = 10)
    {
        cap = c;
        arr = new char[cap];
        top = -1;
    }

    void push(int val)
    {
        if (top == cap - 1)
        {
            cout << "Overflow!\n";
            return;
        }
        arr[++top] = val;

        cout << "Value " << arr[top] << " added in stack!\n";
    }

    char pop()
    {
        if (top == -1)
        {
            cout << "Underflow!\n";
            return '\0';
        }

        cout << "Value " << arr[top] << " removed!\n";
        return arr[top--];
    }

    void print()
    {
        for (int i = 0; i <= top; i++)
        {
            cout << arr[i];
        }
        cout << endl;
    }

    bool is_empty()
    {
        return (top == -1);
    }

    bool is_full()
    {
        return (top == cap - 1);
    }

    char peek()
    {
        if (is_empty())
        {
            return '\0';
        }
        return arr[top];
    }
};
int precedence(char op)
{
    if (op == '^')
        return 3;
    if (op == '*' || op == '/')
        return 2;
    if (op == '+' || op == '-')
        return 1;
    return -1;
}

string infixtopostfix(string infix)
{

    string post = "";

    stack s;

    int size = infix.length();

    char a;

    for (int i = 0; i < size; i++)
    {
        a = infix[i];

        if (a >= 'a' && a <= 'z' || a >= 'A' && a <= 'Z' || a >= '0' && a <= '9')
        {
            post += a;
        }
        else if (a == '(')
        {
            s.push(a);
        }
        else if (a == ')')
        {
            while (!(s.is_empty()) && s.peek() != '(')
            {
                post += s.pop();
            }
            if (!(s.is_empty()))
            {
                s.pop();
            }
        }
        else
        {
            while (!(s.is_empty()) && precedence(s.peek()) >= precedence(a))
            {
                post += s.pop();
            }
            s.push(a);
        }
    }
    while (!(s.is_empty()))
    {
        post += s.pop();
    }
    return post;
}

int main()
{

    string infix("A+(B*C^5)");

    string postfix = infixtopostfix(infix);

    cout << postfix << endl;

    return 0;
}