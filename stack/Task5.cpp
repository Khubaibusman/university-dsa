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
        topIndex = topIndex + 1;
        elements[topIndex] = item;
    }

    DataType pop() {
        if (topIndex == -1) {
            return DataType();
        }
        DataType itemToReturn = elements[topIndex];
        topIndex = topIndex - 1;
        return itemToReturn;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    void pwd() {
        if (isEmpty()) {
            cout << "/" << endl;
            return;
        }
        for (int index = 0; index <= topIndex; index++) {
            cout << "/" << elements[index];
        }
        cout << endl;
    }
};

Stack<string> directoryStack(30);

void cd(string folderName) {
    directoryStack.push(folderName);
}

void cdDotDot() {
    directoryStack.pop();
}

void pwd() {
    directoryStack.pwd();
}

int main() {
    cd("root");
    cd("projects");
    cd("assignment");

    pwd();

    cdDotDot();

    pwd();

    cd("tests");

    pwd();

    return 0;
}