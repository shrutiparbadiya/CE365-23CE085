#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

// Function to perform operation
double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        case '^': return pow(a, b);
    }
    return 0;
}

// Function to check precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Evaluate simple constant expression (no variables)
double evaluate(string expr) {
    stringstream ss(expr);
    double num, result;
    char op;

    ss >> result;
    while (ss >> op >> num) {
        result = applyOp(result, num, op);
    }
    return result;
}

// Function for constant folding
string constantFolding(string expr) {
    string result = "";

    for (int i = 0; i < expr.length(); ) {
        if (isdigit(expr[i])) {
            string temp = "";

            // Extract full constant expression
            while (i < expr.length() &&
                  (isdigit(expr[i]) || expr[i] == '.' ||
                   expr[i] == '+' || expr[i] == '-' ||
                   expr[i] == '*' || expr[i] == '/' || expr[i] == '^')) {
                temp += expr[i];
                i++;
            }

            // Evaluate and replace
            double val = evaluate(temp);
            result += to_string((int)val);
        } else {
            result += expr[i];
            i++;
        }
    }
    return result;
}

int main() {
    string expr;
    cout << "Enter expression: ";
    getline(cin, expr);

    string optimized = constantFolding(expr);

    cout << "Optimized Expression: " << optimized << endl;
    return 0;
}
