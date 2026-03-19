#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

char stack[MAX];
int top = -1;

void push(char x) {
    stack[++top] = x;
}

char pop() {
    return stack[top--];
}

int priority(char x) {
    if (x == '(') return 0;
    if (x == '+' || x == '-') return 1;
    if (x == '*' || x == '/') return 2;
    return 0;
}

// Convert Infix to Postfix
void infixToPostfix(char infix[], char postfix[]) {
    int i = 0, k = 0;
    char ch;

    while ((ch = infix[i++]) != '\0') {
        if (isalnum(ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            push(ch);
        } else if (ch == ')') {
            while ((ch = pop()) != '(')
                postfix[k++] = ch;
        } else {
            while (top != -1 && priority(stack[top]) >= priority(ch))
                postfix[k++] = pop();
            push(ch);
        }
    }

    while (top != -1)
        postfix[k++] = pop();

    postfix[k] = '\0';
}

// Generate Quadruples
void generateQuadruple(char postfix[]) {
    char tempStack[MAX][10];
    int top2 = -1;
    int tempCount = 1;

    printf("\nOperator\tOperand1\tOperand2\tResult\n");

    for (int i = 0; postfix[i] != '\0'; i++) {
        char ch = postfix[i];

        if (isalnum(ch)) {
            char operand[10];
            operand[0] = ch;
            operand[1] = '\0';
            strcpy(tempStack[++top2], operand);
        } else {
            char op2[10], op1[10], result[10];

            strcpy(op2, tempStack[top2--]);
            strcpy(op1, tempStack[top2--]);

            sprintf(result, "t%d", tempCount++);

            printf("%c\t\t%s\t\t%s\t\t%s\n", ch, op1, op2, result);

            strcpy(tempStack[++top2], result);
        }
    }
}

int main() {
    char infix[MAX], postfix[MAX];

    printf("Enter Expression: ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("Postfix: %s\n", postfix);

    generateQuadruple(postfix);

    return 0;
}
