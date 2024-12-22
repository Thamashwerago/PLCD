# README for PLCD Repository

## Project Overview

This repository contains the implementation of a **Lexical Analyzer** and **Parser** for a given grammar. It is designed to:

- Perform lexical analysis to tokenize input strings.
- Parse input strings to validate their structure based on predefined grammar rules.
- Report syntax and lexical errors.
- Provide a symbol table for efficient data management.

---

## File Structure

```
├── LSP               # Main executable
├── input.txt         # Input string file
├── lex.1             # Lexical analyzer specification file
├── lex.yy.c          # Generated lexical analyzer code
├── parser.c          # Parser implementation
├── symbolTable.c     # Symbol table implementation
```

---

## Installation and Execution (Linux Only)

### Step 1: Clone the Repository
1. Open a terminal.
2. Change the current directory to the desired location.
3. Clone the repository using:

   ```bash
   git clone https://github.com/Thamashwerago/PLCD.git
   ```

4. Navigate to the cloned repository:

   ```bash
   cd PLCD
   ```

### Step 2: Modify Input
Edit the `input.txt` file with your preferred input string. Use any text editor, such as `nano` or `vim`:

```bash
nano input.txt
```

### Step 3: Run the Program
Run the program with the input file:

```bash
./LSP < input.txt
```

### Output
- For valid inputs, the output includes the graphical representation of the **Top-Down Parser Tree**.
- For invalid inputs, detailed error logs are generated.

---

## Features

1. **Tokenization**: Converts input strings into lexemes and tokens.
2. **Parsing**: Verifies the structure of the input against grammar rules.
3. **Error Handling**:
   - Lexical errors (e.g., invalid characters).
   - Syntax errors (e.g., unexpected tokens).
4. **Symbol Table**: Maintains metadata for lexemes, including type and position.
