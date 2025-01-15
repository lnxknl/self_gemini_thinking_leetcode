#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool isInterleave(const char *s1, const char *s2, const char *s3) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    int len3 = strlen(s3);

    if (len1 + len2 != len3) {
        return false;
    }

    bool dp[len1 + 1][len2 + 1];
    memset(dp, false, sizeof(dp));

    dp[0][0] = true;

    // Initialize first column
    for (int i = 1; i <= len1; i++) {
        if (s1[i - 1] == s3[i - 1]) {
            dp[i][0] = dp[i - 1][0];
        }
    }

    // Initialize first row
    for (int j = 1; j <= len2; j++) {
        if (s2[j - 1] == s3[j - 1]) {
            dp[0][j] = dp[0][j - 1];
        }
    }

    // Fill the dp table
    for (int i = 1; i <= len1; i++) {
        for (int j = 1; j <= len2; j++) {
            if (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j]) {
                dp[i][j] = true;
            }
            if (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]) {
                dp[i][j] = true;
            }
        }
    }

    return dp[len1][len2];
}

int main() {
    // Test cases
    char *s1_1 = "abc";
    char *s2_1 = "de";
    char *s3_1 = "adbec";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_1, s2_1, s3_1, isInterleave(s1_1, s2_1, s3_1) ? "true" : "false");

    char *s1_2 = "abc";
    char *s2_2 = "de";
    char *s3_2 = "abdec";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_2, s2_2, s3_2, isInterleave(s1_2, s2_2, s3_2) ? "true" : "false");

    char *s1_3 = "abc";
    char *s2_3 = "de";
    char *s3_3 = "acebd";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_3, s2_3, s3_3, isInterleave(s1_3, s2_3, s3_3) ? "true" : "false");

    char *s1_4 = "abc";
    char *s2_4 = "de";
    char *s3_4 = "abxde";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_4, s2_4, s3_4, isInterleave(s1_4, s2_4, s3_4) ? "true" : "false");

    char *s1_5 = "";
    char *s2_5 = "";
    char *s3_5 = "";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_5, s2_5, s3_5, isInterleave(s1_5, s2_5, s3_5) ? "true" : "false");

    char *s1_6 = "aabcc";
    char *s2_6 = "dbbca";
    char *s3_6 = "aadbbcbcac";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_6, s2_6, s3_6, isInterleave(s1_6, s2_6, s3_6) ? "true" : "false");

    char *s1_7 = "aabcc";
    char *s2_7 = "dbbca";
    char *s3_7 = "aadbbbaccc";
    printf("isInterleave(\"%s\", \"%s\", \"%s\") = %s\n", s1_7, s2_7, s3_7, isInterleave(s1_7, s2_7, s3_7) ? "true" : "false");

    return 0;
}
