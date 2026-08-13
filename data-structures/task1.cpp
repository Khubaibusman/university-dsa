#include <iostream>
using namespace std;

template <typename T>
int searchFunction(T arr[], int size, T valueToFind) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == valueToFind) {
            return i;
        }
    }
    return -1;
}

int main() {

    int numbers[] = {10, 25, 3, 47, 8, 99, 15};
    int totalNumbers = sizeof(numbers) / sizeof(numbers[0]);

    int result = searchFunction(numbers, totalNumbers, 47);
    if (result != -1)
        cout << "47 found at index: " << result << endl;
    else
        cout << "47 not found" << endl;

    result = searchFunction(numbers, totalNumbers, 100);
    if (result != -1)
        cout << "100 found at index: " << result << endl;
    else
        cout << "100 not found" << endl;

    float decimalNumbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    int totalDecimals = sizeof(decimalNumbers) / sizeof(decimalNumbers[0]);

    result = searchFunction(decimalNumbers, totalDecimals, 3.3f);
    if (result != -1)
        cout << "3.3 found at index: " << result << endl;
    else
        cout << "3.3 not found" << endl;

    char letters[] = {'a', 'b', 'c', 'd', 'e'};
    int totalLetters = sizeof(letters) / sizeof(letters[0]);

    result = searchFunction(letters, totalLetters, 'd');
    if (result != -1)
        cout << "'d' found at index: " << result << endl;
    else
        cout << "'d' not found" << endl;

    result = searchFunction(letters, totalLetters, 'z');
    if (result != -1)
        cout << "'z' found at index: " << result << endl;
    else
        cout << "'z' not found" << endl;

    return 0;
}