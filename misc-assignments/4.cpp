#include <iostream>
using namespace std;

class queue {
    int q[10];
    int front, rear;

public:
    queue() {
        front = 0;
        rear = -1;
    }

    void add(int val) {
        rear++;
        q[rear] = val;
    }

    void remove() {
        front++;
    }

    int get_front() {
        return q[front];
    }

    bool is_empty() {
        if (front > rear) {
            return true;
        }
        return false;
    }

    void display(int size) {
        for (int i = front; i <= rear; i++) {
            if (i >= 0 && i < size) {
                cout << q[i] << " ";
            }
        }
    }
};

class queue_game {
    queue q[3];
    int size;

public:
    queue_game(int n) {
        size = n;
        int val = 1;
        
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < n; j++) {
                q[i].add(val);
                val++;
            }
        }
    }

    void display() {
        for (int i = 0; i < 3; i++) {
            cout << "Queue " << i << ": ";
            q[i].display(size);
            cout << endl;
        }
        cout << "----------------------\n";
    }

    void process() {
        int time = 0;

        while (true) {
            bool empty = true;

            for (int i = 0; i < 3; i++) {
                if (!q[i].is_empty()) {
                    empty = false;

                    cout << "Processing person " << q[i].get_front()
                         << " from Queue " << i << endl;

                    q[i].remove();
                    time += 2;

                    cout << "Time: " << time << " sec\n";
                    display();
                }
            }

            if (empty == true) {
                break;
            }
        }

        cout << "All people processed in " << time << " seconds.\n";
    }
};

int main() {
    int n = 5;

    queue_game q(n);

    cout << "Initial State:\n";
    q.display();

    q.process();

    return 0;
}