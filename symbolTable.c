typedef enum { NUMBER, ID, PLUS, TIMES, LPAREN, RPAREN, END } TokenType;

struct Symbol {
    char name[10];
    TokenType type;
} symbolTable[100];
int symbolCount = 0;

void addToSymbolTable(char *name, TokenType type) {
    strcpy(symbolTable[symbolCount].name, name);
    symbolTable[symbolCount].type = type;
    symbolCount++;
}

void printSymbolTable() {
    printf("\nSymbol Table\n");
    printf("Name\tType\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\t", symbolTable[i].name);
		switch (symbolTable[i].type)
		{
		case NUMBER:
			printf("NUMBER\n");
			break;
		
		case ID:
			printf("ID\n");
			break;

		case PLUS:
			printf("PLUS\n");
			break;

		case TIMES:
			printf("TIMES\n");
			break;

		case LPAREN:
			printf("LPAREN\n");
			break;

		case RPAREN:
			printf("RPAREN\n");
			break;
		
		default:
			break;
		}
    }
}