#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100 // Define a maximum size for the stack

typedef struct {
    int items[MAX_SIZE];
    int top;
} ItemStack;

// Function to initialize the stack
void initStack(ItemStack *stack) {
    stack->top = -1;
}

// Function to check if the stack is full
bool isFull(const ItemStack *stack) {
    return stack->top == MAX_SIZE - 1;
}

// Function to check if the stack is empty
bool isEmpty(const ItemStack *stack) {
    return stack->top == -1;
}

// Function to place an item onto the stack
bool place(ItemStack *stack, int item_id) {
    if (isFull(stack)) {
        printf("Stack Overflow: Cannot place item %d.\n", item_id);
        return false;
    }
    stack->top++;
    stack->items[stack->top] = item_id;
    printf("Placed item %d.\n", item_id);
    return true;
}

// Function to retrieve an item from the stack
int retrieve(ItemStack *stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow: No items to retrieve.\n");
        return -1; // Indicate no item available
    }
    int item_id = stack->items[stack->top];
    stack->top--;
    printf("Retrieved item %d.\n", item_id);
    return item_id;
}

int main() {
    ItemStack myStack;
    initStack(&myStack);

    // Test Example Input
    place(&myStack, 10);
    place(&myStack, 25);
    place(&myStack, 5);

    retrieve(&myStack); // Should retrieve 5
    retrieve(&myStack); // Should retrieve 25
    retrieve(&myStack); // Should retrieve 10
    retrieve(&myStack); // Should indicate underflow

    return 0;
}
