#include <iostream>

using namespace std;

int getLen(const char* s) {
    int c = 0;
    while(s[c] != '\0') {
        c++;
    }
    return c;
}

char* setLocker(char* p, const char* in) {
    if (p != nullptr) {
        delete[] p; 
    }
    
    int s = getLen(in);
    char* n = new char[s + 1];
    
    for(int i = 0; i <= s; i++) {
        n[i] = in[i];
    }
    
    return n;
}

char* resetLocker(char* p) {
    if (p != nullptr) {
        delete[] p;
    }
    return nullptr; 
}

int main() {
    char buf[50];
    char* p1 = nullptr;
    
    cout << "enter code: ";
    cin >> buf;
    p1 = setLocker(p1, buf);
    cout << "locker setup: " << p1 << endl;

    cout << "enter new code: ";
    cin >> buf;
    p1 = setLocker(p1, buf); 
    cout << "no memory leak! old memory deleted. new setup: " << p1 << endl;

    p1 = resetLocker(p1);
    
    if (p1 == nullptr) {
        cout << "locker reset safely. no dangling pointer." << endl;
    }

    return 0;
}