#include <stdio.h>
#include <string.h>

// Global variables
int k = 0, z = 0, i = 0, j = 0, length = 0;
char input[16], actionStr[20], stack[15], action[10];
FILE *inp;

// Function prototype
void check();

// Main function
int main() {
    printf("GRAMMAR is:\n");
    printf("E -> E + E\n");
    printf("E -> E * E\n");
    printf("E -> (E)\n");
    printf("E -> id\n\n");

    inp = fopen("input.txt", "r");
    if (inp == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    fscanf(inp, "%s", input);
    fclose(inp);

    length = strlen(input);
    strcpy(action, "SHIFT->");

    printf("STACK\t\tINPUT\t\tACTION\n");

    // Parsing logic
    for (k = 0, i = 0; j < length; k++, i++, j++) {
        // If current token is 'id'
        if (input[j] == 'i' && input[j + 1] == 'd') {
            stack[i] = input[j];
            stack[i + 1] = input[j + 1];
            stack[i + 2] = '\0';

            input[j] = ' ';
            input[j + 1] = ' ';

            printf("\n$%s\t\t%s$\t%sid", stack, input, action);
            check();
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

    // Final result
    if (strcmp(stack, "E") == 0)
        printf("\nExpression is valid!\n");
    else
        printf("\nNot a valid expression!\n");

    return 0;
}

// Function to check and reduce according to grammar
void check() {
    strcpy(actionStr, "REDUCE TO E");

    // Rule: E -> id
    for (z = 0; z < length; z++) {
        if (stack[z] == 'i' && stack[z + 1] == 'd') {
            stack[z] = 'E';
            stack[z + 1] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, actionStr);
            j++;
        }
    }

    // Rule: E -> E + E
    for (z = 0; z < length; z++) {
        if (stack[z] == 'E' && stack[z + 1] == '+' && stack[z + 2] == 'E') {
            stack[z] = 'E';
            stack[z + 1] = '\0';
            stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, actionStr);
            i = i - 2;
        }
    }

    // Rule: E -> E * E
    for (z = 0; z < length; z++) {
        if (stack[z] == 'E' && stack[z + 1] == '*' && stack[z + 2] == 'E') {
            stack[z] = 'E';
            stack[z + 1] = '\0';
            stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, actionStr);
            i = i - 2;
        }
    }

    // Rule: E -> (E)
    for (z = 0; z < length; z++) {
        if (stack[z] == '(' && stack[z + 1] == 'E' && stack[z + 2] == ')') {
            stack[z] = 'E';
            stack[z + 1] = '\0';
            stack[z + 2] = '\0';
            printf("\n$%s\t\t%s$\t%s", stack, input, actionStr);
            i = i - 2;
        }
    }
}
