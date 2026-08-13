#include <iostream>
#include <string>

using namespace std;

template <typename DataType>
class Stack {
private:
    DataType* elements;
    int topIndex;
    int maxCapacity;

public:
    Stack(int size) {
        maxCapacity = size;
        elements = new DataType[maxCapacity];
        topIndex = -1;
    }

    ~Stack() {
        delete[] elements;
    }

    void Push(DataType item) {
        topIndex = topIndex + 1;
        elements[topIndex] = item;
    }

    DataType Pop() {
        DataType itemToReturn = elements[topIndex];
        topIndex = topIndex - 1;
        return itemToReturn;
    }

    DataType Peek() {
        return elements[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    void Display() {
        if (isEmpty()) {
            cout << "No calls in history." << endl;
            return;
        }
        for (int index = topIndex; index >= 0; index--) {
            cout << elements[index] << endl;
        }
    }
};

int main() {
    Stack<string> callStack(10);
    
    callStack.Push("Home");
    callStack.Push("Doctor");
    callStack.Push("Friend");

    cout << "Checking current call: " << callStack.Peek() << endl;

    cout << "\nShowing all calls:" << endl;
    callStack.Display();

    cout << "\nEnding call: " << callStack.Pop() << endl;

    cout << "\nNew current call: " << callStack.Peek() << endl;

    return 0;
}