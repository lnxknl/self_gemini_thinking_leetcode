#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *extractRoleName(const char *credits_text, const char *target_role) {
    if (credits_text == NULL || target_role == NULL) {
        return NULL;
    }

    // Convert both strings to lowercase for case-insensitive search
    char *lower_credits = strdup(credits_text);
    char *lower_role = strdup(target_role);
    if (lower_credits == NULL || lower_role == NULL) {
        free(lower_credits);
        free(lower_role);
        return NULL;
    }
    for (int i = 0; lower_credits[i]; i++) {
        lower_credits[i] = tolower(lower_credits[i]);
    }
    for (int i = 0; lower_role[i]; i++) {
        lower_role[i] = tolower(lower_role[i]);
    }

    // Search for the target role
    char *role_ptr = strstr(lower_credits, lower_role);
    if (role_ptr == NULL) {
        free(lower_credits);
        free(lower_role);
        return NULL;
    }

    // Find the position of "by" after the role
    const char *by_keyword = "by";
    char *by_ptr = strstr(role_ptr + strlen(lower_role), by_keyword);
    if (by_ptr == NULL) {
        free(lower_credits);
        free(lower_role);
        return NULL;
    }

    // Find the start of the name
    int name_start_index = (by_ptr - lower_credits) + strlen(by_keyword);

    // Find the end of the name (until the next comma or end of string)
    int name_end_index = name_start_index;
    while (credits_text[name_end_index] != '\0' && credits_text[name_end_index] != ',') {
        name_end_index++;
    }

    // Extract the name
    int name_length = name_end_index - name_start_index;
    if (name_length <= 0) {
        free(lower_credits);
        free(lower_role);
        return NULL;
    }

    char *name = (char *)malloc(sizeof(char) * (name_length + 1));
    if (name == NULL) {
        free(lower_credits);
        free(lower_role);
        return NULL;
    }
    strncpy(name, credits_text + name_start_index, name_length);
    name[name_length] = '\0';

    // Trim leading/trailing whitespace (optional, but good practice)
    int start = 0;
    while (isspace((unsigned char)name[start])) {
        start++;
    }
    int end = strlen(name) - 1;
    while (end > start && isspace((unsigned char)name[end])) {
        end--;
    }
    name[end + 1] = '\0';

    // Shift the string to remove leading spaces
    if (start > 0) {
        memmove(name, name + start, end - start + 2);
    }

    free(lower_credits);
    free(lower_role);
    return name;
}

int main() {
    const char *credits = "Directed by Steven Spielberg, Produced by Kathleen Kennedy, Edited by NOELLE BOISSON, Music by Hans Zimmer";
    const char *target_role = "Edited";
    char *editor_name = extractRoleName(credits, target_role);

    if (editor_name != NULL) {
        printf("The editor is: %s\n", editor_name);
        free(editor_name);
    } else {
        printf("Role not found or name extraction failed.\n");
    }

    const char *credits2 = "Casting by Sarah Finn";
    const char *target_role2 = "Directed";
    char *director_name = extractRoleName(credits2, target_role2);

    if (director_name != NULL) {
        printf("The director is: %s\n", director_name);
        free(director_name);
    } else {
        printf("Role not found or name extraction failed.\n");
    }

    const char *credits3 = "Music by Junkie XL";
    const char *target_role3 = "Music";
    char *editor_name2 = extractRoleName(credits3, target_role3);

    if (editor_name2 != NULL) {
        printf("The editor is: %s\n", editor_name2);
        free(editor_name2);
    } else {
        printf("Role not found or name extraction failed.\n");
    }

    return 0;
}
