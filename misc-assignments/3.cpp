#include <iostream>
using namespace std;

class game {
    int n;

public:
    game(int val) {
        n = val;
    }

    void display(int A[], int B[], int C[]) {
        cout << "Shelf A: ";
        for (int i = 0; i < n; i++) {
            if (A[i] != 0) cout << A[i] << " ";
        }
        cout << endl;

        cout << "Shelf B: ";
        for (int i = 0; i < n; i++) {
            if (B[i] != 0) cout << B[i] << " ";
        }
        cout << endl;

        cout << "Shelf C: ";
        for (int i = 0; i < n; i++) {
            if (C[i] != 0) cout << C[i] << " ";
        }
        cout << endl;

        cout << "----------------------\n";
    }

    void move(int n, int A[], int B[], int C[], char from, char to, char aux) {
        if (n == 1) {
            int i;
            for (i = 0; i < this->n; i++) {
                if (A[i] != 0) break;
            }

            int val = A[i];
            A[i] = 0;

            int j;
            for (j = 0; j < this->n; j++) {
                if (C[j] != 0) break;
            }
            C[j - 1] = val;

            cout << "Move record " << val << " from " << from << " to " << to << endl;
            display(A, B, C);
            return;
        }

        move(n - 1, A, C, B, from, aux, to);
        move(1, A, B, C, from, to, aux);
        move(n - 1, B, A, C, aux, to, from);
    }
};

int main() {
    int n = 5;

    int A[10] = {0}, B[10] = {0}, C[10] = {0};

    for (int i = 0; i < n; i++) {
        A[i] = n - i; 
    }

    game g(n);

    cout << "Initial State:\n";
    g.display(A, B, C);

    g.move(n, A, B, C, 'A', 'C', 'B');

    return 0;
}