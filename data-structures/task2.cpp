#include <iostream>
using namespace std;

template <typename T>
int searchFunction(T arr[], int size, T value) {
    int start = 0;
    int end = size - 1;

    while (start <= end) {
        int middle = (start + end) / 2;

        if (arr[middle] == value) {
            return middle;
        }
        else if (arr[middle] < value) {
            start = middle + 1;
        }
        else {
            end = middle - 1;
        }
    }

    return -1;
}

int main() {

    int numbers[] = {3, 8, 10, 25, 47, 99, 100};
    int totalNumbers = sizeof(numbers) / sizeof(numbers[0]);

    int result = searchFunction(numbers, totalNumbers, 47);
    if (result != -1)
        cout << "47 found at index: " << result << endl;
    else
        cout << "47 not found" << endl;

    result = searchFunction(numbers, totalNumbers, 3);
    if (result != -1)
        cout << "3 found at index: " << result << endl;
    else
        cout << "3 not found" << endl;

    result = searchFunction(numbers, totalNumbers, 100);
    if (result != -1)
        cout << "100 found at index: " << result << endl;
    else
        cout << "100 not found" << endl;

    result = searchFunction(numbers, totalNumbers, 55);
    if (result != -1)
        cout << "55 found at index: " << result << endl;
    else
        cout << "55 not found" << endl;

    float decimals[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    int totalDecimals = sizeof(decimals) / sizeof(decimals[0]);

    result = searchFunction(decimals, totalDecimals, 2.2f);
    if (result != -1)
        cout << "2.2 found at index: " << result << endl;
    else
        cout << "2.2 not found" << endl;

    result = searchFunction(decimals, totalDecimals, 9.9f);
    if (result != -1)
        cout << "9.9 found at index: " << result << endl;
    else
        cout << "9.9 not found" << endl;

    char letters[] = {'a', 'b', 'c', 'd', 'e'};
    int totalLetters = sizeof(letters) / sizeof(letters[0]);

    result = searchFunction(letters, totalLetters, 'c');
    if (result != -1)
        cout << "'c' found at index: " << result << endl;
    else
        cout << "'c' not found" << endl;

    result = searchFunction(letters, totalLetters, 'z');
    if (result != -1)
        cout << "'z' found at index: " << result << endl;
    else
        cout << "'z' not found" << endl;

    return 0;
}