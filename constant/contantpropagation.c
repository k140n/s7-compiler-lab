#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 20

// Structure representing one 3-address statement (op arg1 arg2 res)
typedef struct {
    char op[5];
    char arg1[10];
    char arg2[10];
    char res[10];
    int flag; // 1 if processed or folded
} Expression;

Expression exprList[MAX_EXPR];
int n;

// Constant tracking for variables a-z
int constValue[26];
int isConst[26]; // 1 if variable has known constant value

// Helper to get index of variable a–z
int getVarIndex(char c) {
    return c - 'a';
}

// Check if string is numeric constant
int isNumber(const char *s) {
    if (s == NULL || s[0] == '\0') return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// Replace occurrences of exprList[curr].res with newValue in later statements
void propagate(int curr, const char *newValue) {
    for (int i = curr + 1; i < n; i++) {
        if (strcmp(exprList[curr].res, exprList[i].arg1) == 0)
            strcpy(exprList[i].arg1, newValue);
        if (strcmp(exprList[curr].res, exprList[i].arg2) == 0)
            strcpy(exprList[i].arg2, newValue);
    }
}

// Input the list of expressions
void input() {
    printf("Enter number of statements: ");
    scanf("%d", &n);
    getchar();

    printf("Enter statements (op arg1 arg2 res)\n");
    printf("Examples: '+ a b t1' or '= 5 t1 x'\n");

    for (int i = 0; i < n; i++) {
        scanf("%s %s %s %s",
              exprList[i].op, exprList[i].arg1, exprList[i].arg2, exprList[i].res);
        exprList[i].flag = 0;
    }

    memset(isConst, 0, sizeof(isConst));
    memset(constValue, 0, sizeof(constValue));
}

// Perform constant propagation and folding
void constantPropagation() {
    for (int i = 0; i < n; i++) {
        Expression *e = &exprList[i];

        // Handle assignment
        if (strcmp(e->op, "=") == 0) {
            if (isNumber(e->arg1)) {
                int idx = getVarIndex(e->res[0]);
                constValue[idx] = atoi(e->arg1);
                isConst[idx] = 1;
            }
            propagate(i, e->arg1);
            e->flag = 1;
        }

        // Handle arithmetic: +, -, *, /
        else if (isNumber(e->arg1) && isNumber(e->arg2)) {
            int val1 = atoi(e->arg1);
            int val2 = atoi(e->arg2);
            int result = 0;

            switch (e->op[0]) {
                case '+': result = val1 + val2; break;
                case '-': result = val1 - val2; break;
                case '*': result = val1 * val2; break;
                case '/': result = (val2 != 0) ? val1 / val2 : 0; break;
                default: printf("Unsupported operator: %s\n", e->op); continue;
            }

            int idx = getVarIndex(e->res[0]);
            constValue[idx] = result;
            isConst[idx] = 1;

            char temp[10];
            sprintf(temp, "%d", result);
            propagate(i, temp);

            e->flag = 1;
        }
    }
}

// Output final optimized expressions
void outputFinal() {
    printf("\nFinal Output (After Constant Propagation and Folding):\n");

    for (int i = 0; i < n; i++) {
        Expression *e = &exprList[i];
        int idx = getVarIndex(e->res[0]);

        // If result is constant, print simplified statement
        if (isConst[idx]) {
            printf("%s = %d\n", e->res, constValue[idx]);
        } else if (strcmp(e->op, "=") == 0) {
            printf("%s = %s\n", e->res, e->arg1);
        } else {
            printf("%s = %s %c %s\n", e->res, e->arg1, e->op[0], e->arg2);
        }
    }
}

int main() {
    input();
    constantPropagation();
    outputFinal();
    return 0;
}
