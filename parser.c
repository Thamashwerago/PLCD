#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the structure for a parse tree node
typedef struct Node {
    char label[10];       // Node label (e.g., "E", "T", "F", "id")
    int x, y;             // Node position for SVG
    struct Node* left;    // Left child
    struct Node * middle; // Middle child (for three children)
    struct Node* right;   // Right child
} Node;

// Define token type
int pos = 0;
TokenType lookahead;

// Forward declarations of parsing functions
Node* E();
Node* EPrime();
Node* T();
Node* TPrime();
Node* F();
Node* Id();
void error(const char *message);

// Tokenizer to fetch the next token
TokenType nextToken() {
    if (pos < symbolCount)
    {
        pos++;
        return symbolTable[pos-1].type;
    } else {
        return END;
    }
}

// Create a new node
Node* createNode(const char* label) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->label, label);
    newNode->x = 0;
    newNode->y = 0;
    newNode->left = NULL;
    newNode->middle = NULL;
    newNode->right = NULL;
    return newNode;
}

// Match the expected token and advance
void match(TokenType expected) {
    if (lookahead == expected) {
        lookahead = nextToken();
    } else {
        error("Syntax Error: Unexpected token");
    }
}

int calculateSubtreeWidth(Node* root) {
    if (!root) return 0;

    int leftWidth = calculateSubtreeWidth(root->left);
    int middleWidth = calculateSubtreeWidth(root->middle);
    int rightWidth = calculateSubtreeWidth(root->right);

    // Subtree width is the sum of child widths or 1 for leaf nodes
    int subtreeWidth = leftWidth + middleWidth + rightWidth;
    return subtreeWidth > 0 ? subtreeWidth : 1;
}

int calculateTreeDepth(Node* root) {
    if (!root) return 0;

    int leftDepth = calculateTreeDepth(root->left);
    int middleDepth = calculateTreeDepth(root->middle);
    int rightDepth = calculateTreeDepth(root->right);

    // Tree depth is the maximum depth of its children + 1
    return 1 + ((leftDepth > middleDepth ? leftDepth : middleDepth) > rightDepth
                ? (leftDepth > middleDepth ? leftDepth : middleDepth)
                : rightDepth);
}

void assignPositions(Node* root, int depth, int x) {
    if (!root) return;

    int leftWidth = calculateSubtreeWidth(root->left);
    int middleWidth = calculateSubtreeWidth(root->middle);
    int rightWidth = calculateSubtreeWidth(root->right);

    // Position current node
    root->x = x;
    root->y = depth * 150; // Fixed vertical spacing

    // Recursively position children
    if (root->left)
        assignPositions(root->left, depth + 1, x - (middleWidth + rightWidth) * 50);
    if (root->middle)
        assignPositions(root->middle, depth + 1, x);
    if (root->right)
        assignPositions(root->right, depth + 1, x + (leftWidth + middleWidth) * 50);
}


// Grammar Rules Implementation
Node* E() {
    printf("E -> T E'\n");
    Node* ENode = createNode("E");
    ENode->left = T();
    ENode->right = EPrime();
    return ENode;
}

Node* EPrime() {
    Node* EPrimeNode = createNode("E'");
    if (lookahead == PLUS) {
        printf("E' -> + T E'\n");
        Node* plusNode = createNode("+");
        match(PLUS);
        EPrimeNode->left = plusNode;
        EPrimeNode->middle = T();
        EPrimeNode->right = EPrime();
    } else {
        printf("E' -> ε\n");
        Node* Epsilon = createNode("ε");
        EPrimeNode->middle = Epsilon;
    }
    return EPrimeNode;
}

Node* T() {
    printf("T -> F T'\n");
    Node* TNode = createNode("T");
    TNode->left = F();
    TNode->right = TPrime();
    return TNode;  
}

Node* TPrime() {
    Node* TPrimeNode = createNode("T'");
    if (lookahead == TIMES) {
        printf("T' -> * F T'\n");
        Node* starNode = createNode("*");
        match(TIMES);
        TPrimeNode->left = starNode;
        TPrimeNode->middle = F();
        TPrimeNode->right = TPrime();
    } else {
        printf("T' -> ε\n");
        Node* Epsilon = createNode("ε");
        TPrimeNode->middle = Epsilon;
    }
    return TPrimeNode;
}

Node* F() {
    if (lookahead == LPAREN) {
        printf("F -> ( E )\n");
        Node* FNode = createNode("F");
        Node* LeftParenNode = createNode("(");
        match(LPAREN);
        FNode->left = LeftParenNode;
        FNode->middle = E();
        Node* RightParenNode = createNode(")");
        match(RPAREN);
        FNode->right = RightParenNode;
        return FNode;
    } else if (lookahead == ID) {
        printf("F -> id\n");
        Node* FNode = createNode("F");
        Node* IDNode = Id();
        match(ID);
        FNode->middle = IDNode;
        return FNode;
    } else if (lookahead == NUMBER) {
        printf("F -> id\n");
        Node* FNode = createNode("F");
        Node* IDNode = Id();
        match(NUMBER);
        FNode->middle = IDNode;
        return FNode;
    } else {
        error("Syntax Error");
    }
}

Node* Id(){
    printf("id -> %s\n",symbolTable[pos-1].name);
    Node* IDNode = createNode("id");
    Node* ValueNode = createNode(symbolTable[pos-1].name);
    IDNode->middle = ValueNode;
    return IDNode;
}

// Error Handling
void error(const char *message) {
    printf("\n%s at position %d\n", message, pos);
    exit(EXIT_FAILURE);
}

// Recursive function to write SVG for the tree
void writeSVG(FILE* file, Node* root) {
    if (!root) return;

    // Draw edges first
    if (root->left) {
        fprintf(file, "    <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#ccc\" stroke-width=\"2\" />\n",
                root->x, root->y, root->left->x, root->left->y);
        writeSVG(file, root->left);
    }
    if (root->middle) {  // Handle the middle child
        fprintf(file, "    <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#ccc\" stroke-width=\"2\" />\n",
                root->x, root->y, root->middle->x, root->middle->y);
        writeSVG(file, root->middle);
    }
    if (root->right) {
        fprintf(file, "    <line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"#ccc\" stroke-width=\"2\" />\n",
                root->x, root->y, root->right->x, root->right->y);
        writeSVG(file, root->right);
    }

    // Draw the node
    fprintf(file, "    <circle cx=\"%d\" cy=\"%d\" r=\"20\" fill=\"#4285f4\" stroke=\"#fff\" stroke-width=\"2\" />\n", root->x, root->y);
    fprintf(file, "    <text x=\"%d\" y=\"%d\" font-size=\"12\" text-anchor=\"middle\" fill=\"#fff\">%s</text>\n",
            root->x, root->y + 4, root->label);
}


// Main Function
int parser() {
    
    printf("Top-down parse tree\n");
    lookahead = nextToken();
    Node* parseTree = E();

    if (lookahead == END) {
        printf("\nInput is valid\n");
    } else {
        error("\nUnexpected tokens at the end");
    }

    // Calculate tree dimensions
    int maxDepth = calculateTreeDepth(parseTree);
    int maxWidth = calculateSubtreeWidth(parseTree) * 100; // 100px per node horizontally

    // Determine SVG size dynamically
    int svgWidth = maxWidth + 200;  // 200px per node horizontally
    int svgHeight = (maxDepth + 1) * 150 + 100;  // 150px per level vertically

    // Assign positions to nodes dynamically
    assignPositions(parseTree, 0, svgWidth / 2);

    // Open SVG file for writing
    FILE* file = fopen("parseTree.svg", "w");
    if (file == NULL) {
        perror("Error creating SVG file");
        return EXIT_FAILURE;
    }

    // Write SVG header with viewBox for scaling and translation
    fprintf(file, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\">\n",svgWidth, svgHeight, svgWidth, svgHeight);
    fprintf(file, "  <rect width=\"%d\" height=\"%d\" fill=\"white\" />\n", svgWidth, svgHeight); // White background

    // Add a title for the SVG with space below it
    fprintf(file, "  <text x=\"%d   \" y=\"30\" font-size=\"24\" text-anchor=\"middle\" fill=\"black\">Top-Down Parse Tree</text>\n",svgWidth/2);

    // Translate the tree downward, adding space below the title
    fprintf(file, "  <g transform=\"translate(0, 70)\">\n");

    // Write the tree
    writeSVG(file, parseTree);

    // Write SVG footer
    fprintf(file, "  </g>\n");
    fprintf(file, "</svg>\n");

    // Close file
    fclose(file);

    // Open SVG in the browser
    #ifdef _WIN32
        system("start parseTree.svg"); // Windows
    #elif __APPLE__
        system("open parseTree.svg"); // macOS
    #elif __linux__
        system("xdg-open parseTree.svg"); // Linux/Unix
    #else
        printf("Please open 'parseTree.svg' manually in your web browser.\n");
    #endif

    return 0;
}
