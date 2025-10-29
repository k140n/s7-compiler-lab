#include <stdio.h>
#include <string.h>

// ---------------------------
// Global Variables
// ---------------------------
int i = 0, j = 0, z = 0, length = 0;
char input[32], stack[32], action[16], reduceMsg[32];
FILE *inp;

// Function Declaration
void check();

// ---------------------------
// Main Function
// ---------------------------
int main() {
    printf("GRAMMAR:\n");
    printf("E -> E + E\n");
    printf("E -> E * E\n");
    printf("E -> (E)\n");
    printf("E -> id\n\n");

    inp = fopen("input.txt", "r");
    if (inp == NULL) {
        printf("Error: Could not open input.txt\n");
        return 1;
    }

    fscanf(inp, "%s", input);
    fclose(inp);

    length = strlen(input);
    strcpy(action, "SHIFT->");

    printf("STACK\t\tINPUT\t\tACTION\n");

    // ---------------------------
    // Shift phase
    // ---------------------------
    for (i = 0, j = 0; j < length; i++, j++) {
        // If token is "id"
        if (input[j] == 'i' && input[j + 1] == 'd') {
            stack[i] = input[j];
            stack[i + 1] = input[j + 1];
            stack[i + 2] = '\0';

            input[j] = ' ';
            input[j + 1] = ' ';

            printf("\n$%s\t\t%s$\t%sid", stack, input, action);
            check();

            j++; // Skip the next character ('d') of "id"
        } 
        else {
            // Shift single symbol
            stack[i] = input[j];
            stack[i + 1] = '\0';
            input[j] = ' ';

            printf("\n$%s\t\t%s$\t%ssymbol", stack, input, action);
            check();
        }
    }

    printf("\n");

    // ---------------------------
    // Final validation
    // ---------------------------
    if (strcmp(stack, "E") == 0)
        printf("\n✅ Expression is valid!\n");
    else
        printf("\n❌ Not a valid expression!\n");

    return 0;
}

// ---------------------------
// Reduction Function
// ---------------------------
void check() {
    strcpy(reduceMsg, "REDUCE TO E");

    // Rule: E -> id
    for (z = 0; z < length; z++) {
        if (stack[z] == 'i' && stack[z + 1] == 'd') {
            stack[z] = 'E';
            stack[z + 1] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, reduceMsg);
        }
    }

    // Rule: E -> E + E
    for (z = 0; z < length; z++) {
        if (stack[z] == 'E' && stack[z + 1] == '+' && stack[z + 2] == 'E') {
            stack[z] = 'E';
            stack[z + 1] = stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, reduceMsg);
        }
    }

    // Rule: E -> E * E
    for (z = 0; z < length; z++) {
        if (stack[z] == 'E' && stack[z + 1] == '*' && stack[z + 2] == 'E') {
            stack[z] = 'E';
            stack[z + 1] = stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, reduceMsg);
        }
    }

    // Rule: E -> (E)
    for (z = 0; z < length; z++) {
        if (stack[z] == '(' && stack[z + 1] == 'E' && stack[z + 2] == ')') {
            stack[z] = 'E';
            stack[z + 1] = stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, reduceMsg);
        }
    }
}

