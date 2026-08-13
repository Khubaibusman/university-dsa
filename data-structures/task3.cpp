#include <iostream>
using namespace std;

template <typename T>
void sortArray(T arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int minIndex = i;

        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        T temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

template <typename T>
void showArray(T arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {

    int numbers[] = {64, 25, 12, 22, 11};
    int totalNumbers = sizeof(numbers) / sizeof(numbers[0]);

    cout << "Integer Array Before Sorting: ";
    showArray(numbers, totalNumbers);

    sortArray(numbers, totalNumbers);

    cout << "Integer Array After Sorting:  ";
    showArray(numbers, totalNumbers);

    cout << endl;

    float decimals[] = {5.5, 1.1, 4.4, 2.2, 3.3};
    int totalDecimals = sizeof(decimals) / sizeof(decimals[0]);

    cout << "Float Array Before Sorting: ";
    showArray(decimals, totalDecimals);

    sortArray(decimals, totalDecimals);

    cout << "Float Array After Sorting:  ";
    showArray(decimals, totalDecimals);

    cout << endl;

    char letters[] = {'e', 'a', 'd', 'b', 'c'};
    int totalLetters = sizeof(letters) / sizeof(letters[0]);

    cout << "Char Array Before Sorting: ";
    showArray(letters, totalLetters);

    sortArray(letters, totalLetters);

    cout << "Char Array After Sorting:  ";
    showArray(letters, totalLetters);

    cout << endl;

    int sortedNumbers[] = {1, 2, 3, 4, 5};
    int sortedSize = sizeof(sortedNumbers) / sizeof(sortedNumbers[0]);

    cout << "Already Sorted Array Before: ";
    showArray(sortedNumbers, sortedSize);

    sortArray(sortedNumbers, sortedSize);

    cout << "Already Sorted Array After:  ";
    showArray(sortedNumbers, sortedSize);

    cout << endl;

    int reverseNumbers[] = {5, 4, 3, 2, 1};
    int reverseSize = sizeof(reverseNumbers) / sizeof(reverseNumbers[0]);

    cout << "Reverse Array Before Sorting: ";
    showArray(reverseNumbers, reverseSize);

    sortArray(reverseNumbers, reverseSize);

    cout << "Reverse Array After Sorting:  ";
    showArray(reverseNumbers, reverseSize);

    return 0;
}