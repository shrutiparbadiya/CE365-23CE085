#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

#define MAX 50

/* keyword list */
string keywords[10] =
{
    "int", "char", "float", "double",
    "if", "else", "return", "while", "for", "void"
};

/* symbol table */
string symtab[50];
int symcount = 0;

/* function declarations */
bool isKeyword(string word);
bool isSymbol(string word);
void addSymbol(string word);

int main()
{
    ifstream fin("input.c");
    char c;
    string buffer ="";
    int line = 1;

    if(!fin)
    {
        cout << "error file opening" << endl;
        return 1;
    }

    cout << "Tokens:\n\n";

    while(fin.get(c))
    {
        if(c == '\n')
        {
            line++;
        }

        if(c == '/')
        {
            char next;
            fin.get(next);

            if(next == '/')
            {
                while(fin.get(c) && c!='\n');
                line++;
                continue;
            }
            else if (next == '*')
            {
                while(fin.get(c))
                {
                    if(c == '\n') line++;
                    if (c == '*')
                    {
                        fin.get(next);
                        if (next == '/')
                            break;
                    }

                }
                continue;
            }
            else
            {
                fin.unget();
                cout << "Operator: /\n";
            }
        }
        if(isalpha(c) || c == '_')
        {
            buffer = c;
            while(fin.get(c) && (isalnum(c) || c == '_'))
            {
                buffer += c;
            }
            fin.unget();

            if(isKeyword(buffer))
            {
                cout << "Keyword: " << buffer << endl;
            }
            else
            {
                cout << "Identifier: " << buffer << endl;
                addSymbol(buffer);
            }
        }
        else if (c == '+' || c == '*' || c == '-' || c == '=')
        {
            cout << "Operator: " << c << endl;
        }
        else if (c == '(' || c == ')' || c == '{' || c == '}' ||
                 c == ';' || c == ',')
        {
            cout << "Punctuation: " << c << endl;
        }
        else if (isdigit(c))
        {
            buffer = c;
            while (fin.get(c) && isalnum(c))
            {
                buffer += c;
            }
            fin.unget();

            if (isalpha(buffer.back()))
            {
                cout << "\nLEXICAL ERROR\n";
                cout << "Line " << line << " : " << buffer
                     << " invalid lexeme\n\n";
            }
            else
                cout << "Constant: " << buffer << endl;
        }

    }
    cout << "\nSYMBOL TABLE ENTRIES\n";
    for (int i = 0; i < symcount; i++)
    {
        cout << i + 1 << ") " << symtab[i] << endl;
    }

    fin.close();
    return 0;
}

/* check keyword */
bool isKeyword(string word)
{
    for (int i = 0; i < 10; i++)
    {
        if (word == keywords[i])
            return true;
    }
    return false;
}

/* check symbol table */
bool isSymbol(string word)
{
    for (int i = 0; i < symcount; i++)
    {
        if (symtab[i] == word)
            return true;
    }
    return false;
}

/* add identifier */
void addSymbol(string word)
{
    if (!isSymbol(word))
    {
        symtab[symcount++] = word;
    }
}
