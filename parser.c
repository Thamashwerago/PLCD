#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NUMBER, ID, PLUS, TIMES, LPAREN, RPAREN, END } TokenType;
char input[100];
int pos = 0;
TokenType lookahead;

TokenType nextToken() {
    while (input[pos] == ' ' || input[pos] == '\t') pos++;
    if (input[pos] == '\0') return END;
    if (input[pos] == '+') { pos++; return PLUS; }
    if (input[pos] == '*') { pos++; return TIMES; }
    if (input[pos] == '(') { pos++; return LPAREN; }
    if (input[pos] == ')') { pos++; return RPAREN; }
    if (input[pos] >= '0' && input[pos] <= '9') { while (input[pos] >= '0' && input[pos] <= '9') pos++; return NUMBER; }
    if ((input[pos] >= 'a' && input[pos] <= 'z') || (input[pos] >= 'A' && input[pos] <= 'Z')) {
        while ((input[pos] >= 'a' && input[pos] <= 'z') || (input[pos] >= 'A' && input[pos] <= 'Z') || (input[pos] >= '0' && input[pos] <= '9')) pos++;
        return ID;
    }
    printf("Invalid character\n");
    exit(EXIT_FAILURE);
}

void match(TokenType expected) {
    if (lookahead == expected) {
        lookahead = nextToken();
    } else {
        printf("Syntax Error\n");
        exit(EXIT_FAILURE);
    }
}

void E();
void EPrime();
void T();
void TPrime();
void F();

void E() {
    printf("E -> T E'\n");
    T();
    EPrime();
}

void EPrime() {
    if (lookahead == PLUS) {
        printf("E' -> + T E'\n");
        match(PLUS);
        T();
        EPrime();
    } else {
        printf("E' -> ε\n");
    }
}

void T() {
    printf("T -> F T'\n");
    F();
    TPrime();
}

void TPrime() {
    if (lookahead == TIMES) {
        printf("T' -> * F T'\n");
        match(TIMES);
        F();
        TPrime();
    } else {
        printf("T' -> ε\n");
    }
}

void F() {
    if (lookahead == LPAREN) {
        printf("F -> ( E )\n");
        match(LPAREN);
        E();
        match(RPAREN);
    } else if (lookahead == ID) {
        printf("F -> ID\n");
        match(ID);
    } else if (lookahead == NUMBER) {
        printf("F -> NUMBER\n");
        match(NUMBER);
    } else {
        printf("Syntax Error\n");
        exit(EXIT_FAILURE);
    }
}

int main() {
    printf("Enter the input string:\n");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline
    lookahead = nextToken();
    E();
    if (lookahead == END) {
        printf("Input is valid\n");
    } else {
        printf("Syntax Error at the end\n");
    }
    return 0;
}
