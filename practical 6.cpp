#include <iostream>
#include <string>
using namespace std;

string input;
int pos = 0;
bool S();
bool L();
bool Ldash();

bool match(char expected) {
    if (pos < input.length() && input[pos] == expected) {
        pos++;
        return true;
    }
    return false;
}

bool S() {
    if (match('a')) {
        return true;
    }
    else if (match('(')) {
        if (L()) {
            if (match(')')) {
                return true;
            }
        }
        return false;
    }
    return false;
}

bool L() {
    if (S()) {
        return Ldash();
    }
    return false;
}

bool Ldash() {
    if (match(',')) {
        if (S()) {
            return Ldash();
        }
        return false;
    }
    return true;
}

int main() {
    cout << "Enter string: ";
    cin >> input;

    pos = 0;

    if (S() && pos == input.length()) {
        cout << "Valid string" << endl;
    } else {
        cout << "Invalid string" << endl;
    }

    return 0;
}
