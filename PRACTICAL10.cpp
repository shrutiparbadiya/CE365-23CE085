#include <bits/stdc++.h>
using namespace std;

int precedence(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw runtime_error("Invalid");
            return a / b;
        case '^': return pow(a, b);
    }
    throw runtime_error("Invalid");
}

double evaluate(string s) {
    stack<double> values;
    stack<char> ops;

    for (int i = 0; i < s.length(); i++) {

        if (s[i] == ' ') continue;

        // Number (integer or decimal)
        if (isdigit(s[i]) || s[i] == '.') {
            string val = "";
            while (i < s.length() && (isdigit(s[i]) || s[i] == '.')) {
                val += s[i++];
            }
            values.push(stod(val));
            i--;
        }

        else if (s[i] == '(') {
            ops.push(s[i]);
        }

        else if (s[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                if (values.size() < 2) throw runtime_error("Invalid");
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            if (ops.empty()) throw runtime_error("Invalid");
            ops.pop(); // remove '('
        }

        else { // operator

            // invalid case: operator at start or after another operator
            if (i == 0 || (!isdigit(s[i-1]) && s[i-1] != ')'))
                throw runtime_error("Invalid");

            while (!ops.empty() && precedence(ops.top()) >= precedence(s[i])) {
                if (values.size() < 2) throw runtime_error("Invalid");
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(s[i]);
        }
    }

    while (!ops.empty()) {
        if (values.size() < 2) throw runtime_error("Invalid");
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    if (values.size() != 1) throw runtime_error("Invalid");

    return values.top();
}

int main() {
    string expr;
    cout << "Enter expression: ";
    getline(cin, expr);

    try {
        double result = evaluate(expr);
        cout << "Result: " << result << endl;
    } catch (...) {
        cout << "Invalid expression" << endl;
    }

    return 0;
}
