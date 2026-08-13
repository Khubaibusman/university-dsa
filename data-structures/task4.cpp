#include <iostream>
#include <string>
using namespace std;

template <typename T>
class Student {
private:
    string name;
    T scores[10];

public:
    void inputData();
    void showData();
    T getTotal();
    float getAverage();
    T getHighest();
    int searchScore(T value);
    void sortScores();
};

template <typename T>
void Student<T>::inputData() {
    cout << "Enter name: ";
    cin >> name;

    cout << "Enter 10 marks: ";
    for (int i = 0; i < 10; i++) {
        cin >> scores[i];
    }
}

template <typename T>
void Student<T>::showData() {
    cout << "Name: " << name << endl;
    cout << "Marks: ";

    for (int i = 0; i < 10; i++) {
        if (i != 9)
            cout << scores[i] << ", ";
        else
            cout << scores[i];
    }

    cout << endl;
    cout << "Total: " << getTotal()
         << ", Average: " << getAverage()
         << ", Highest: " << getHighest() << endl;
}

template <typename T>
T Student<T>::getTotal() {
    T sum = 0;
    for (int i = 0; i < 10; i++) {
        sum = sum + scores[i];
    }
    return sum;
}

template <typename T>
float Student<T>::getAverage() {
    return (float)getTotal() / 10;
}

template <typename T>
T Student<T>::getHighest() {
    T max = scores[0];
    for (int i = 1; i < 10; i++) {
        if (scores[i] > max) {
            max = scores[i];
        }
    }
    return max;
}

template <typename T>
int Student<T>::searchScore(T value) {
    for (int i = 0; i < 10; i++) {
        if (scores[i] == value) {
            return i;
        }
    }
    return -1;
}

template <typename T>
void Student<T>::sortScores() {
    for (int i = 0; i < 9; i++) {
        int minIndex = i;

        for (int j = i + 1; j < 10; j++) {
            if (scores[j] < scores[minIndex]) {
                minIndex = j;
            }
        }

        T temp = scores[i];
        scores[i] = scores[minIndex];
        scores[minIndex] = temp;
    }

    cout << "Sorted: ";
    for (int i = 0; i < 10; i++) {
        if (i != 9)
            cout << scores[i] << ", ";
        else
            cout << scores[i];
    }
    cout << endl;
}

int main() {

    Student<int> s1;
    s1.inputData();
    s1.showData();
    int r1 = s1.searchScore(3);
    if (r1 != -1)
        cout << "Find (3): " << r1 << endl;
    else
        cout << "Find (3): not found" << endl;
    s1.sortScores();

    cout << endl;

    Student<int> s2;
    s2.inputData();
    s2.showData();
    int r2 = s2.searchScore(7);
    if (r2 != -1)
        cout << "Find (7): " << r2 << endl;
    else
        cout << "Find (7): not found" << endl;
    s2.sortScores();

    cout << endl;

    Student<float> s3;
    s3.inputData();
    s3.showData();
    int r3 = s3.searchScore(4.5f);
    if (r3 != -1)
        cout << "Find (4.5): " << r3 << endl;
    else
        cout << "Find (4.5): not found" << endl;
    s3.sortScores();

    cout << endl;

    Student<float> s4;
    s4.inputData();
    s4.showData();
    int r4 = s4.searchScore(6.5f);
    if (r4 != -1)
        cout << "Find (6.5): " << r4 << endl;
    else
        cout << "Find (6.5): not found" << endl;
    s4.sortScores();

    cout << endl;

    Student<int> s5;
    s5.inputData();
    s5.showData();
    int r5 = s5.searchScore(5);
    if (r5 != -1)
        cout << "Find (5): " << r5 << endl;
    else
        cout << "Find (5): not found" << endl;
    s5.sortScores();

    return 0;
}