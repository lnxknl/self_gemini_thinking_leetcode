#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MAX

// Part 1: Minimum Number of Coins
int coinChangeMin(int amount, int* coins, int coinsSize) {
    int* dp = (int*)malloc(sizeof(int) * (amount + 1));
    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1; // Initialize to infinity
    }
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int j = 0; j < coinsSize; j++) {
            if (i - coins[j] >= 0) {
                dp[i] = (dp[i] < dp[i - coins[j]] + 1) ? dp[i] : dp[i - coins[j]] + 1;
            }
        }
    }

    int result = (dp[amount] > amount) ? -1 : dp[amount];
    free(dp);
    return result;
}

// Part 2: Number of Combinations
int coinChangeCombinations(int amount, int* coins, int coinsSize) {
    int* dp = (int*)malloc(sizeof(int) * (amount + 1));
    for (int i = 0; i <= amount; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;

    for (int i = 0; i < coinsSize; i++) {
        for (int j = coins[i]; j <= amount; j++) {
            dp[j] += dp[j - coins[i]];
        }
    }

    int result = dp[amount];
    free(dp);
    return result;
}

int main() {
    int amount = 20; // From the image "二十块钱"
    int coins[] = {1, 5, 10}; // Example coin denominations
    int coinsSize = sizeof(coins) / sizeof(coins[0]);

    // Part 1: Minimum Number of Coins
    int minCoins = coinChangeMin(amount, coins, coinsSize);
    printf("Minimum number of coins to make %d: %d\n", amount, minCoins);

    // Part 2: Number of Combinations
    int combinations = coinChangeCombinations(amount, coins, coinsSize);
    printf("Number of combinations to make %d: %d\n", amount, combinations);

    // Test with different input
    int amount2 = 7;
    int coins2[] = {2, 5};
    int coinsSize2 = sizeof(coins2) / sizeof(coins2[0]);
    minCoins = coinChangeMin(amount2, coins2, coinsSize2);
    printf("Minimum number of coins to make %d: %d\n", amount2, minCoins);
    combinations = coinChangeCombinations(amount2, coins2, coinsSize2);
    printf("Number of combinations to make %d: %d\n", amount2, combinations);

    int amount3 = 3;
    int coins3[] = {2};
    int coinsSize3 = sizeof(coins3) / sizeof(coins3[0]);
    minCoins = coinChangeMin(amount3, coins3, coinsSize3);
    printf("Minimum number of coins to make %d: %d\n", amount3, minCoins);
    combinations = coinChangeCombinations(amount3, coins3, coinsSize3);
    printf("Number of combinations to make %d: %d\n", amount3, combinations);

    return 0;
}
