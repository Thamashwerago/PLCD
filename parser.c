#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define token type
//char input[100];
int pos = 0;
TokenType lookahead;

// Forward declarations of parsing functions
void E();
void EPrime();
void T();
void TPrime();
void F();
void error(const char *message);

// Tokenizer to fetch the next token
TokenType nextToken() {
    /*while (input[pos] == ' ' || input[pos] == '\t') pos++;
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
    error("Invalid character");
    return END;*/

    if (pos < symbolCount)
    {
        pos++;
        return symbolTable[pos-1].type;
    } else {
        return END;
    }
    
}

// Match the expected token and advance
void match(TokenType expected) {
    if (lookahead == expected) {
        lookahead = nextToken();
    } else {
        error("Syntax Error: Unexpected token");
    }
}

// Grammar Rules Implementation
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
        error("Syntax Error");
    }
}

// Error Handling
void error(const char *message) {
    printf("\n%s at position %d\n", message, pos);
    exit(EXIT_FAILURE);
}

// Main Function
int parser() {

    //printf("Enter the input string:\n");
    //fgets(input, sizeof(input), stdin);
    //input[strcspn(input, "\n")] = 0; // Remove newline character
    
    printf("Top-down parse tree\n");
    lookahead = nextToken();
    E();
    if (lookahead == END) {
        printf("\nInput is valid\n");
    } else {
        error("\nUnexpected tokens at the end");
    }
    return 0;
}
