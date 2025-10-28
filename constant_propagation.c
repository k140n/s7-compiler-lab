#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to hold one expression (like: op arg1 arg2 res)
struct Expression {
    char op[5];
    char arg1[10];
    char arg2[10];
    char res[10];
    int flag; // Used to mark processed expressions
} exprList[20];

int n;                // Number of statements
int constValue[26];   // Stores constant value of variable (a-z)
int isConst[26];      // Marks if variable is a known constant

// Helper: Map variable (a-z) to array index 0-25
int getVarIndex(char c) {
    return c - 'a';
}

// Helper: Check if a string represents a number
int isNumber(char *s) {
    return isdigit(s[0]);
}

// Propagate a constant value to later statements
void propagate(int currentIndex, char *newValue) {
    for (int i = currentIndex + 1; i < n; i++) {
        if (strcmp(exprList[currentIndex].res, exprList[i].arg1) == 0)
            strcpy(exprList[i].arg1, newValue);
        if (strcmp(exprList[currentIndex].res, exprList[i].arg2) == 0)
            strcpy(exprList[i].arg2, newValue);
    }
}

// Input the list of expressions
void input() {
    printf("Enter number of statements: ");
    scanf("%d", &n);

    printf("Enter statements (op arg1 arg2 res):\n");
    printf("Example: + a b t1  OR  = 5 t1 x\n");

    for (int i = 0; i < n; i++) {
        scanf("%s %s %s %s", 
              exprList[i].op, exprList[i].arg1, exprList[i].arg2, exprList[i].res);
        exprList[i].flag = 0;
    }

    memset(isConst, 0, sizeof(isConst)); // Initialize all to false
}

// Perform constant propagation and folding
void constantPropagation() {
    for (int i = 0; i < n; i++) {
        char *op = exprList[i].op;
        char *a1 = exprList[i].arg1;
        char *a2 = exprList[i].arg2;
        char *r  = exprList[i].res;

        // Handle assignment: r = a1
        if (op[0] == '=') {
            // If assigning a number, mark r as constant
            if (isNumber(a1)) {
                int idx = getVarIndex(r[0]);
                constValue[idx] = atoi(a1);
                isConst[idx] = 1;
            }

            // Propagate the value of a1 to later uses of r
            propagate(i, a1);
            exprList[i].flag = 1;
        }

        // Handle arithmetic operations (+, -, *, /)
        else if (isNumber(a1) && isNumber(a2)) {
            // Both operands are constants → perform constant folding
            int val1 = atoi(a1);
            int val2 = atoi(a2);
            int idx = getVarIndex(r[0]);
            int result = 0;

            switch (op[0]) {
                case '+': result = val1 + val2; break;
                case '-': result = val1 - val2; break;
                case '*': result = val1 * val2; break;
                case '/': result = (val2 != 0) ? val1 / val2 : 0; break;
                default: break;
            }

            constValue[idx] = result;
            isConst[idx] = 1;

            // Replace r’s uses with the folded result
            char temp[10];
            sprintf(temp, "%d", result);
            propagate(i, temp);

            exprList[i].flag = 1;
        }
    }
}

// Print the final optimized code
void outputFinal() {
    printf("\nFinal Output (After Constant Propagation and Folding):\n");

    for (int i = 0; i < n; i++) {
        int idx = getVarIndex(exprList[i].res[0]);

        if (exprList[i].op[0] == '=') {
            // Assignment
            if (isConst[idx])
                printf("%s = %d\n", exprList[i].res, constValue[idx]);
            else
                printf("%s = %s\n", exprList[i].res, exprList[i].arg1);
        } else {
            // Arithmetic
            if (isConst[idx])
                printf("%s = %d\n", exprList[i].res, constValue[idx]);
            else
                printf("%s = %s %c %s\n",
                       exprList[i].res, exprList[i].arg1, exprList[i].op[0], exprList[i].arg2);
        }
    }
}

// Main function
int main() {
    input();
    constantPropagation();
    outputFinal();
    return 0;
}
