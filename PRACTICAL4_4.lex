%{
#include <stdio.h>
#include <string.h>

int hasLower = 0, hasUpper = 0, hasDigit = 0, hasSymbol = 0;
int length = 0;
%}

LOWER   [a-z]
UPPER   [A-Z]
DIGIT   [0-9]
SYMBOL  [*#$@,]

%%

{LOWER}    { hasLower = 1; length++; }
{UPPER}    { hasUpper = 1; length++; }
{DIGIT}    { hasDigit = 1; length++; }
{SYMBOL}   { hasSymbol = 1; length++; }

\n {
    if (length >= 9 && length <= 15 &&
        hasLower && hasUpper && hasDigit && hasSymbol)
        printf("Valid password\n");
    else
        printf("Invalid password\n");

    /* reset for next input */
    hasLower = hasUpper = hasDigit = hasSymbol = 0;
    length = 0;
}

.   { length++; }   /* any other character */

%%

int main()
{
    printf("Enter password: ");
    yylex();
    return 0;
}

int yywrap()
{
    return 1;
}
