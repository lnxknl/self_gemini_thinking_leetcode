#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char findSingleNonRepeatingCharacter(const char *str) {
    if (str == NULL) {
        return '\0'; // Or handle error appropriately
    }

    // Use an array as a hash table for lowercase English letters (a-z)
    int charCounts[26] = {0};
    int n = strlen(str);

    // Pass 1: Count character occurrences
    for (int i = 0; i < n; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            charCounts[str[i] - 'a']++;
        }
        // You might want to handle non-lowercase characters differently
    }

    // Pass 2: Find the single non-repeating character
    for (int i = 0; i < n; i++) {
        if (str[i] >= 'a' && str[i] <= 'z' && charCounts[str[i] - 'a'] == 1) {
            return str[i];
        }
    }

    return '\0'; // Return null character if no single non-repeating character found
}

int main() {
    // Test Example Input
    const char *input1 = "aabbccddeeefghhijklmnoopqrrstuvwxyyz";
    char result1 = findSingleNonRepeatingCharacter(input1);
    printf("Input: \"%s\", Single non-repeating character: '%c'\n", input1, result1); // Expected: 'f'

    const char *input2 = "aaaaaaaaabbbbbbbbcccccccdddddddeeeeeeefffffgggghhhhhiiiiiijjjjjjkkkkkkllllllmmmmmnnnnnnooooooppppppqqqqqqrrrrrrssssssttttttuuuuuuvvvvvvwwwwwwxxxxxxzzzzzzy";
    char result2 = findSingleNonRepeatingCharacter(input2);
    printf("Input: \"%s\", Single non-repeating character: '%c'\n", input2, result2); // Expected: 'y'

    const char *input3 = "aabbccddeeffgghhiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz";
    char result3 = findSingleNonRepeatingCharacter(input3);
    printf("Input: \"%s\", Single non-repeating character: '%c'\n", input3, result3); // Expected: '\0' (or some indication of no unique character)

    const char *input4 = "z";
    char result4 = findSingleNonRepeatingCharacter(input4);
    printf("Input: \"%s\", Single non-repeating character: '%c'\n", input4, result4); // Expected: 'z'

    const char *input5 = "";
    char result5 = findSingleNonRepeatingCharacter(input5);
    printf("Input: \"%s\", Single non-repeating character: '%c'\n", input5, result5); // Expected: '\0'

    return 0;
}
