#include <iostream>

using namespace std;

int getLen(const char* s) {
    int c = 0;
    while(s[c] != '\0') {
        c++;
    }
    return c;
}

char* setLocker(const char* in) {
    int s = getLen(in);
    char* p = new char[s + 1];
    
    for(int i = 0; i <= s; i++) {
        p[i] = in[i];
    }
    
    return p;
}

void resetLocker(char* p) {
    delete[] p;
}

int main() {
    char buf[50];
    
    cout << "enter code: ";
    cin >> buf;
    char* p1 = setLocker(buf);
    cout << "locker setup: " << p1 << endl;

    cout << "enter new code: ";
    cin >> buf;
    p1 = setLocker(buf); 
    cout << "memory leaked for first code! new setup: " << p1 << endl;

    resetLocker(p1);
    
    cout << "wrong code or garbage value showing: " << p1 << endl;

    return 0;
}