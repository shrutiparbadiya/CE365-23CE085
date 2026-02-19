%{
#include <stdio.h>
int line = 1;
%}

KEYWORD     int|char|float|double|return|if|else|while|for|void|main
ID          [a-zA-Z_][a-zA-Z0-9_]*
NUM         [0-9]+
INVALIDNUM  [0-9]+[a-zA-Z_]+[a-zA-Z0-9_]*
CHARCONST   \'([^\\\']|\\.)\'
STR         \"([^\\\"]|\\.)*\"
OP          [+\-*/=%]
PUNC        [(){};,]

%%

{KEYWORD}   { printf("Keyword: %s\n", yytext); }

{INVALIDNUM} {
    printf("LEXICAL ERROR (Line %d): Invalid lexeme %s\n", line, yytext);
}

{ID}        { printf("Identifier: %s\n", yytext); }

{NUM}       { printf("Constant: %s\n", yytext); }

{CHARCONST} { printf("Constant: %s\n", yytext); }

{STR}       { printf("String: %s\n", yytext); }

{OP}        { printf("Operator: %s\n", yytext); }

{PUNC}      { printf("Punctuation: %s\n", yytext); }

"//".*      { }

"/*"([^*]|\*+[^*/])*\*+"/"   { }

[ \t]+      { }

\n          { line++; }

.           {
    printf("LEXICAL ERROR (Line %d): Invalid lexeme %s\n", line, yytext);
}

%%

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        FILE *fp = fopen(argv[1], "r");
        if (!fp)
        {
            printf("Cannot open file\n");
            return 1;
        }
        yyin = fp;
    }

    printf("TOKENS\n");
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
