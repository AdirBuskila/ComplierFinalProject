//
// Created by ניב on 05/04/2024.
//
#include "globals.h"
#include "utils.h"
void skip_space(char **str) {
    if (str == NULL || *str == NULL) return;

    // While the current character is a whitespace character, advance the pointer.
    while (**str && isspace((unsigned char)**str)) {
        (*str)++;
    }
}
char* str_allocate_cat(const char* str1, const char* str2) {
    if (!str1 || !str2) return NULL; // Guard against NULL input strings.

    // Calculate the length needed for the new string.
    size_t length = strlen(str1) + strlen(str2) + 1; // +1 for the null terminator

    // Allocate memory for the new string.
    char* result = (char*)malloc(length);
    if (!result) return NULL; // Allocation failed.

    // Construct the new string.
    strcpy(result, str1); // Copy the first string.
    strcat(result, str2); // Concatenate the second string.

    return result; // Return the new string.
}