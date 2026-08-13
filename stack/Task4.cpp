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
        DataType itemToReturn = elements[topIndex];
        topIndex = topIndex - 1;
        return itemToReturn;
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    void emptyStack() {
        topIndex = -1;
    }

    void printStackContents() {
        if (isEmpty()) {
            cout << "Empty";
            return;
        }
        for (int index = 0; index <= topIndex; index++) {
            cout << elements[index] << " ";
        }
    }
};

Stack<char> stackUndo(50);
Stack<char> stackRedo(50);
string documentText = "";

void printStates() {
    cout << "Text: " << documentText << endl;
    cout << "Undo Stack: ";
    stackUndo.printStackContents();
    cout << "\nRedo Stack: ";
    stackRedo.printStackContents();
    cout << "\n-------------------" << endl;
}

void type(char letter) {
    documentText += letter;
    stackUndo.push(letter);
    stackRedo.emptyStack();
    cout << "Action: type(" << letter << ")" << endl;
    printStates();
}

void undo() {
    if (!stackUndo.isEmpty()) {
        char lastLetter = stackUndo.pop();
        stackRedo.push(lastLetter);
        
        string updatedText = "";
        for (int index = 0; index < documentText.length() - 1; index++) {
            updatedText += documentText[index];
        }
        documentText = updatedText;
        cout << "Action: undo()" << endl;
        printStates();
    }
}

void redo() {
    if (!stackRedo.isEmpty()) {
        char restoreLetter = stackRedo.pop();
        stackUndo.push(restoreLetter);
        documentText += restoreLetter;
        cout << "Action: redo()" << endl;
        printStates();
    }
}

int main() {
    type('H');
    type('i');
    undo();
    redo();

    cout << "\n--- Final Answers ---" << endl;
    cout << "1. Final text is: " << documentText << endl;
    cout << "2. Two stacks are efficient because push and pop happen instantly at the top. We do not need to search or move other data around when we go back or forward in history." << endl;

    return 0;
}