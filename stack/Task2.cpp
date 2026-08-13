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

    DataType peek() {
        return elements[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }
};

string reverseText(string text) {
    Stack<char> characters(text.length());
    for (int index = 0; index < text.length(); index++) {
        characters.push(text[index]);
    }

    string reversedText = "";
    while (!characters.isEmpty()) {
        reversedText += characters.pop();
    }
    return reversedText;
}

bool areBracketsBalanced(string mathText) {
    Stack<char> brackets(mathText.length());
    for (int index = 0; index < mathText.length(); index++) {
        char symbol = mathText[index];
        
        if (symbol == '(' || symbol == '{' || symbol == '[') {
            brackets.push(symbol);
        } 
        else if (symbol == ')' || symbol == '}' || symbol == ']') {
            if (brackets.isEmpty()) {
                return false;
            }
            char lastOpen = brackets.pop();
            if ((symbol == ')' && lastOpen != '(') ||
                (symbol == '}' && lastOpen != '{') ||
                (symbol == ']' && lastOpen != '[')) {
                return false;
            }
        }
    }
    return brackets.isEmpty();
}

int getPriority(char mathOperator) {
    if (mathOperator == '+' || mathOperator == '-') {
        return 1;
    }
    if (mathOperator == '*' || mathOperator == '/') {
        return 2;
    }
    return 0;
}

string infixToPostfix(string normalText) {
    Stack<char> symbols(normalText.length());
    string machineText = "";

    for (int index = 0; index < normalText.length(); index++) {
        char symbol = normalText[index];

        if ((symbol >= 'a' && symbol <= 'z') || 
            (symbol >= 'A' && symbol <= 'Z') || 
            (symbol >= '0' && symbol <= '9')) {
            machineText += symbol;
        } 
        else if (symbol == '(') {
            symbols.push(symbol);
        } 
        else if (symbol == ')') {
            while (!symbols.isEmpty() && symbols.peek() != '(') {
                machineText += symbols.pop();
            }
            symbols.pop();
        } 
        else {
            while (!symbols.isEmpty() && getPriority(symbols.peek()) >= getPriority(symbol)) {
                machineText += symbols.pop();
            }
            symbols.push(symbol);
        }
    }

    while (!symbols.isEmpty()) {
        machineText += symbols.pop();
    }

    return machineText;
}

int main() {
    cout << "Reverse 'Hello': " << reverseText("Hello") << endl;
    
    cout << "Balanced '((a+b)*c)': " << areBracketsBalanced("((a+b)*c)") << endl;
    cout << "Balanced '((a+b)': " << areBracketsBalanced("((a+b)") << endl;
    
    cout << "Postfix '(a+b)*c': " << infixToPostfix("(a+b)*c") << endl;

    return 0;
}