#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<string>> FIRST, FOLLOW;

bool isNonTerminal(char c) {
    return c >= 'A' && c <= 'Z';
}

// Compute FIRST set
void computeFirst(char symbol) {
    for (string production : grammar[symbol]) {
        // epsilon production
        if (production == "e") {
            FIRST[symbol].insert("e");
            continue;
        }

        bool allNullable = true;
        for (char ch : production) {
            if (!isNonTerminal(ch)) {
                FIRST[symbol].insert(string(1, ch));
                allNullable = false;
                break;
            } else {
                computeFirst(ch);
                for (auto x : FIRST[ch]) {
                    if (x != "e")
                        FIRST[symbol].insert(x);
                }
                if (FIRST[ch].find("e") == FIRST[ch].end()) {
                    allNullable = false;
                    break;
                }
            }
        }
        if (allNullable)
            FIRST[symbol].insert("e");
    }
}

// Compute FOLLOW set
void computeFollow(char startSymbol) {
    FOLLOW[startSymbol].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &rule : grammar) {
            char lhs = rule.first;
            for (string prod : rule.second) {
                for (int i = 0; i < prod.size(); i++) {
                    char B = prod[i];
                    if (!isNonTerminal(B)) continue;

                    bool nullable = true;
                    for (int j = i + 1; j < prod.size(); j++) {
                        char next = prod[j];
                        nullable = false;

                        if (!isNonTerminal(next)) {
                            if (FOLLOW[B].insert(string(1, next)).second)
                                changed = true;
                            break;
                        } else {
                            for (auto x : FIRST[next]) {
                                if (x != "e") {
                                    if (FOLLOW[B].insert(x).second)
                                        changed = true;
                                }
                            }
                            if (FIRST[next].find("e") != FIRST[next].end()) {
                                nullable = true;
                                continue;
                            }
                            break;
                        }
                    }
                    if (nullable) {
                        for (auto x : FOLLOW[lhs]) {
                            if (FOLLOW[B].insert(x).second)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

void printSet(string name, char symbol, set<string> s) {
    cout << name << "(" << symbol << ") = {";
    bool first = true;
    for (auto x : s) {
        if (!first) cout << ", ";
        cout << x;
        first = false;
    }
    cout << "}" << endl;
}

int main() {
    // Grammar definition
    grammar['S'] = {"ABC", "D"};
    grammar['A'] = {"a", "e"};
    grammar['B'] = {"b", "e"};
    grammar['C'] = {"(S)", "c"};
    grammar['D'] = {"AC"};

    // Compute FIRST sets
    for (auto &g : grammar)
        computeFirst(g.first);

    // Compute FOLLOW sets
    computeFollow('S');

    // Print FIRST sets
    printSet("First", 'S', FIRST['S']);
    printSet("First", 'A', FIRST['A']);
    printSet("First", 'B', FIRST['B']);
    printSet("First", 'C', FIRST['C']);
    printSet("First", 'D', FIRST['D']);

    cout << endl;

    // Print FOLLOW sets
    printSet("Follow", 'S', FOLLOW['S']);
    printSet("Follow", 'A', FOLLOW['A']);
    printSet("Follow", 'B', FOLLOW['B']);
    printSet("Follow", 'C', FOLLOW['C']);
    printSet("Follow", 'D', FOLLOW['D']);

    return 0;
}
