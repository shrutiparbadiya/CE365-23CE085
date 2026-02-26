#include <iostream>
#include <map>
#include <set>
#include <vector>


using namespace std;

map<char, vector<string>> grammar;
map<char, set<string>> FIRST, FOLLOW;
map<char, map<char, string>> parsingTable;
set<char> terminals;
bool isLL1 = true;

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

void constructParsingTable() {
    // Collect terminals
    for (auto &g : grammar) {
        for (string prod : g.second) {
            for (char ch : prod) {
                if (!isNonTerminal(ch) && ch != 'e')
                    terminals.insert(ch);
            }
        }
    }
    terminals.insert('$');

    // Build table
    for (auto &g : grammar) {
        char lhs = g.first;

        for (string prod : g.second) {
            set<string> firstSet;

            if (prod == "e") {
                firstSet.insert("e");
            } else {
                bool nullable = true;
                for (char ch : prod) {
                    if (!isNonTerminal(ch)) {
                        firstSet.insert(string(1, ch));
                        nullable = false;
                        break;
                    } else {
                        for (auto x : FIRST[ch]) {
                            if (x != "e")
                                firstSet.insert(x);
                        }
                        if (FIRST[ch].find("e") == FIRST[ch].end()) {
                            nullable = false;
                            break;
                        }
                    }
                }
                if (nullable)
                    firstSet.insert("e");
            }

            // Fill table using FIRST
            for (auto t : firstSet) {
                if (t != "e") {
                    if (!parsingTable[lhs][t[0]].empty())
                        isLL1 = false;
                    parsingTable[lhs][t[0]] = prod;
                }
            }

            // If epsilon, use FOLLOW
            if (firstSet.find("e") != firstSet.end()) {
                for (auto f : FOLLOW[lhs]) {
                    if (!parsingTable[lhs][f[0]].empty())
                        isLL1 = false;
                    parsingTable[lhs][f[0]] = prod;
                }
            }
        }
    }
}

void printParsingTable() {
    cout << "\nPredictive Parsing Table:\n\n";

    cout << "\t";
    for (char t : terminals)
        cout << t << "\t";
    cout << endl;

    for (auto &g : grammar) {
        char nt = g.first;
        cout << nt << "\t";

        for (char t : terminals) {
            if (!parsingTable[nt][t].empty())
                cout << nt << "->" << parsingTable[nt][t] << "\t";
            else
                cout << " \t";
        }
        cout << endl;
    }
}

bool validateString(string input, char startSymbol) {
    input += "$";

    vector<char> stack;
    stack.push_back('$');
    stack.push_back(startSymbol);

    int pointer = 0;

    while (!stack.empty()) {
        char top = stack.back();
        char current = input[pointer];

        if (top == current) {
            stack.pop_back();
            pointer++;
        }
        else if (!isNonTerminal(top)) {
            return false;
        }
        else {
            if (parsingTable[top][current].empty())
                return false;

            string production = parsingTable[top][current];
            stack.pop_back();

            if (production != "e") {
                for (int i = production.size() - 1; i >= 0; i--)
                    stack.push_back(production[i]);
            }
        }
    }

    return pointer == input.size();
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
    /*printSet("First", 'S', FIRST['S']);
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
    printSet("Follow", 'D', FOLLOW['D']);*/
    constructParsingTable();
    printParsingTable();

    if (isLL1)
        cout << "\nGrammar is LL(1)\n";
    else
        cout << "\nGrammar is NOT LL(1)\n";

    if (isLL1) {
        string input;
        cout << "\nEnter input string: ";
        cin >> input;

        if (validateString(input, 'S'))
            cout << "Valid string\n";
        else
            cout << "Invalid string\n";
    }

    return 0;
}
