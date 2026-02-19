%{
#include <stdio.h>

int characters = 0;
int words = 0;
int lines = 1;
%}

%%

\n            { lines++; characters++; }
[ \t]+        { characters += yyleng; }
[^ \t\n]+     { words++; characters += yyleng; }
. {characters++;}
%%

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");

    if (!fp)
    {
        printf("File not found!\n");
        return 1;
    }

    yyin = fp;
    yylex();

    printf("\nCharacters : %d", characters);
    printf("\nWords      : %d", words);
    printf("\nLines      : %d\n", lines);

    fclose(fp);
    return 0;
}

int yywrap()
{
    return 1;
}