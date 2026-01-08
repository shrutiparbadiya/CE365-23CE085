#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

int main() {
    int numSymbols;
    cout << "Number of input symbols : ";
    cin >> numSymbols;

    vector<char> symbols(numSymbols);
    cout << "Input symbols : ";
    for (int i = 0; i < numSymbols; i++) {
        cin >> symbols[i];
    }

    int numStates;
    cout << "Enter number of states : ";
    cin >> numStates;

    int startState;
    cout << "Initial state : ";
    cin >> startState;

    int numAcceptStates;
    cout << "Number of accepting states : ";
    cin >> numAcceptStates;

    set<int> acceptStates;
    cout << "Accepting states : ";
    for (int i = 0; i < numAcceptStates; i++) {
        int s;
        cin >> s;
        acceptStates.insert(s);
    }

    // Transition table: (state, symbol) -> next_state
    map<pair<int, char>, int> transition;

    cout << "Transition table :" << endl;
    for (int i = 0; i < numStates; i++) {
        int fromState;
        for (char sym : symbols) {
            cout << "Enter transition (state symbol next_state): ";
            cin >> fromState >> sym;
            int toState;
            cin >> toState;
            transition[{fromState, sym}] = toState;
        }
    }

    string input;
    cout << "Input string : ";
    cin >> input;
    do
    {
        int currentState = startState;
        bool valid = true;

        for (char ch : input) {
        if (transition.find({currentState, ch}) == transition.end()) {
            valid = false;
            break;
        }
        currentState = transition[{currentState, ch}];
        }

        if (valid && acceptStates.count(currentState)) {
            cout << "\nValid String" << endl;
        } else {
            cout << "\nInvalid String" << endl;
        }
    }
    while(input=="0");

    return 0;
}
