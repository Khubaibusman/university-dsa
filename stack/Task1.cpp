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

    void push(DataType item) {
        if (isFull()) {
            cout << "Stack Overflow! Table is full, cannot place another book." << endl;
            return;
        }
        topIndex = topIndex + 1;
        elements[topIndex] = item;
        cout << "Added: " << item << endl;
    }

    DataType pop() {
        if (isEmpty()) {
            cout << "Stack Underflow! Table is empty, no books to remove." << endl;
            return DataType();
        }
        DataType itemToReturn = elements[topIndex];
        topIndex = topIndex - 1;
        cout << "Removed: " << itemToReturn << endl;
        return itemToReturn;
    }

    DataType peek() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return DataType();
        }
        return elements[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == maxCapacity - 1;
    }
};

int main() {
    Stack<int> bookStack(3);
    int choice = 0;
    int bookId = 0;

    cout << "--- Deliberate Underflow Demonstration ---" << endl;
    bookStack.pop();
    cout << "------------------------------------------\n" << endl;

    cout << "--- Deliberate Overflow Demonstration ---" << endl;
    bookStack.push(101);
    bookStack.push(102);
    bookStack.push(103);
    bookStack.push(104);
    cout << "-----------------------------------------\n" << endl;

    Stack<int> menuStack(5);
    while (choice != 4) {
        cout << "\n1. Push Book\n2. Pop Book\n3. Peek Top Book\n4. Exit\nChoose option: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter book ID: ";
            cin >> bookId;
            menuStack.push(bookId);
        } else if (choice == 2) {
            menuStack.pop();
        } else if (choice == 3) {
            int topItem = menuStack.peek();
            if (!menuStack.isEmpty()) {
                cout << "Top book ID is: " << topItem << endl;
            }
        }
    }

    return 0;
}