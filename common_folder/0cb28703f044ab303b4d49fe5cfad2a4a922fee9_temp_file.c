#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100  // Adjust size as needed

// Structure to represent an ingredient
typedef struct {
    char* name;
} Ingredient;

// Structure to represent a drink recipe
typedef struct Recipe {
    char* drink_name;
    Ingredient* ingredients;
    int num_ingredients;
    struct Recipe* next; // For collision handling (separate chaining)
} Recipe;

// Hash table structure
typedef struct {
    Recipe* table[TABLE_SIZE];
} HashTable;

// Simple hash function (can be improved)
unsigned int hash(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % TABLE_SIZE;
}

// Function to create a new recipe
Recipe* createRecipe(const char* drink_name, const char** ingredient_names, int num_ingredients) {
    Recipe* new_recipe = (Recipe*)malloc(sizeof(Recipe));
    if (!new_recipe) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_recipe->drink_name = strdup(drink_name);
    new_recipe->ingredients = (Ingredient*)malloc(sizeof(Ingredient) * num_ingredients);
    if (!new_recipe->ingredients) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_recipe->num_ingredients = num_ingredients;
    for (int i = 0; i < num_ingredients; i++) {
        new_recipe->ingredients[i].name = strdup(ingredient_names[i]);
    }
    new_recipe->next = NULL;
    return new_recipe;
}

// Function to initialize the hash table
HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

// Function to add a recipe to the hash table
void addRecipe(HashTable* ht, const char* drink_name, const char** ingredient_names, int num_ingredients) {
    unsigned int index = hash(drink_name);
    Recipe* new_recipe = createRecipe(drink_name, ingredient_names, num_ingredients);
    if (!ht->table[index]) {
        ht->table[index] = new_recipe;
    } else {
        // Handle collision: add to the beginning of the linked list
        new_recipe->next = ht->table[index];
        ht->table[index] = new_recipe;
    }
}

// Function to get a recipe from the hash table
Recipe* getRecipe(HashTable* ht, const char* drink_name) {
    unsigned int index = hash(drink_name);
    Recipe* current = ht->table[index];
    while (current) {
        if (strcmp(current->drink_name, drink_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Recipe not found
}

// Function to free the memory allocated for a recipe
void freeRecipe(Recipe* recipe) {
    if (recipe) {
        free(recipe->drink_name);
        for (int i = 0; i < recipe->num_ingredients; i++) {
            free(recipe->ingredients[i].name);
        }
        free(recipe->ingredients);
        free(recipe);
    }
}

// Function to free the memory allocated for the hash table
void freeHashTable(HashTable* ht) {
    if (ht) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Recipe* current = ht->table[i];
            while (current) {
                Recipe* temp = current;
                current = current->next;
                freeRecipe(temp);
            }
        }
        free(ht);
    }
}

int main() {
    // Initialize the bartender's recipe book (hash table)
    HashTable* recipe_book = createHashTable();

    // Add some popular drink recipes
    const char* martini_ingredients[] = {"Gin", "Dry Vermouth", "Olive or Lemon Twist"};
    addRecipe(recipe_book, "Martini", martini_ingredients, 3);

    const char* margarita_ingredients[] = {"Tequila", "Lime Juice", "Cointreau"};
    addRecipe(recipe_book, "Margarita", margarita_ingredients, 3);

    const char* mojito_ingredients[] = {"White Rum", "Sugar", "Lime Juice", "Soda Water", "Mint"};
    addRecipe(recipe_book, "Mojito", mojito_ingredients, 5);

    // Test customer orders
    const char* order1 = "Martini";
    Recipe* recipe1 = getRecipe(recipe_book, order1);
    if (recipe1) {
        printf("Bartender knows how to make a %s. Ingredients:\n", recipe1->drink_name);
        for (int i = 0; i < recipe1->num_ingredients; i++) {
            printf("- %s\n", recipe1->ingredients[i].name);
        }
    } else {
        printf("Bartender doesn't know how to make a %s.\n", order1);
    }

    const char* order2 = "Cosmopolitan";
    Recipe* recipe2 = getRecipe(recipe_book, order2);
    if (recipe2) {
        printf("Bartender knows how to make a %s. Ingredients:\n", recipe2->drink_name);
        for (int i = 0; i < recipe2->num_ingredients; i++) {
            printf("- %s\n", recipe2->ingredients[i].name);
        }
    } else {
        printf("Bartender doesn't know how to make a %s.\n", order2);
    }

    // Clean up memory
    freeHashTable(recipe_book);

    return 0;
}
