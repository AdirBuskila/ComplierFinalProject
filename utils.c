//
// Created by ניב on 05/04/2024.
//
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "utils.h"
#include "tabels.h"
#include <stdarg.h>

void print_error_msg(line_description *line, const char *format, ...) {
    va_list args;
    va_start(args, format);
    printf("\033[0;31m"); /* Red */
    printf("ERROR: ");
    printf("\033[0m"); /* Reset */
    vprintf(format, args);
    printf(" in the file: ");
    printf("\033[0;35m"); /* Purple */
    printf("%s", line->file_name);
    printf("\033[0m"); /* Reset */
    printf(" in line: ");
    printf("\033[0;35m"); /* Purple */
    printf("%d\n", line->line_number);
    printf("\033[0m"); /* Reset */
    va_end(args);
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
char **split_string(const char *str, const char *delim, int *count) {
    if (!str || !delim || !count) return NULL;

    int len = strlen(str);
    int i;
    char *copy = strdup(str); // Create a modifiable copy of the input string
    if (!copy) return NULL;   // Check memory allocation success

    if (copy[len - 1] == '\n') copy[len - 1] = '\0'; // Strip trailing newline

    char **results = NULL;
    int num = 0;
    char *piece = strtok(copy, delim);

    while (piece) {
        results = realloc(results, sizeof(char *) * (num + 1));
        if (!results) {
            // Free all allocated memory on failure
            for ( i = 0; i < num; i++) {
                free(results[i]);
            }
            free(results);
            free(copy);
            return NULL;
        }

        results[num] = strdup(piece); // Duplicate the token
        if (!results[num]) {
            // Free all allocated memory on failure
            for ( i = 0; i < num; i++) {
                free(results[i]);
            }
            free(results);
            free(copy);
            return NULL;
        }
        num++;
        piece = strtok(NULL, delim);
    }

    *count = num; // Set the count of found tokens
    free(copy);   // Free the temporary string
    return results;
}
void free_split_string(char **splited_str,int count){
    int i;
    for(i=0;i<count;i++){
        free(splited_str[i]);
    }
    free(splited_str);
}
bool is_label(char *str){
    char temp[MAX_LABEL_LENGTH];
    const char* known_words[]={"r0","r1","r2","r3","r4","r5","r6","r7"
                                ,"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red"
                                ,"prn","jsr","rts","hlt"};
    const int counter_known_words=sizeof(known_words)/sizeof(known_words[0]);
    int i,len= strlen(str);
    if(len>MAX_LABEL_LENGTH)
        return FALSE;
    if(str[len-1]==':'){
        len--;
    }
    strncpy(temp,str,len);
    temp[len]='\0';
    // Check if starting with an alphabetic character
    if (!isalpha(temp[0]))
        return FALSE;

    // Check that the label isn't a reserved word
    for ( i = 0; i < counter_known_words; i++) {
        if (strcmp(temp, known_words[i]) == 0)
            return FALSE;
    }

    // Check that all characters are alphanumeric
    for ( i = 0; temp[i] != '\0'; i++) {
        if (!isalnum(temp[i]))
            return FALSE;
    }

    return TRUE;  // Passed all checks

}
bool is_array(char *str){

    // Find the first occurrence of '['
     char *leftBracket = strchr(str, '[');
     char *p;
    if (!leftBracket || leftBracket == str) {
        return FALSE; // No '[' found or no characters before '['
    }

    // Find the corresponding ']'
    const char *rightBracket = strchr(leftBracket, ']');
    if (!rightBracket || *(rightBracket + 1) != '\0') {
        return FALSE; // No ']' found or characters after ']'
    }

    // Ensure the string segment before '[' is a valid identifier
    for ( p = str; p < leftBracket; p++) {
        if (p == str && !isalpha(*p)) {
            return FALSE; // Identifier must start with a letter
        } else if (p != str && !isalnum(*p) && *p != '_') {
            return FALSE; // Subsequent characters can be alphanumeric or underscore
        }
    }

    // Check the contents between '[' and ']' for being a valid identifier or a positive number
    char *contentStart = leftBracket + 1;
    if (*contentStart == '\0' || *contentStart == ']') {
        return FALSE; // Reject empty brackets or prematurely closed brackets
    }

    if (isalpha(*contentStart)) { // Identifier must start with a letter
        for (p = contentStart; p < rightBracket; p++) {
            if (!isalnum(*p) && *p != '_') {
                return FALSE; // Index must be alphanumeric or underscore
            }
        }
    } else if (isdigit(*contentStart)) { // Check if it's a positive number
        for (p = contentStart; p < rightBracket; p++) {
            if (!isdigit(*p)) {
                return FALSE; // Number must only contain digits
            }
        }
    } else {
        return FALSE; // If it starts with any other character, it's not valid
    }

    return TRUE; // All checks passed
}
bool is_number(const char *str){
    if(*str=='-')
        ++str;
    if(*str=='\0')
        return FALSE;
    while(*str!='\0'){
        if(!isdigit(*str))
            return FALSE;
        str++;
    }
    return TRUE;
}
void update_symbol_table(sym_table *table,long ic){
    int i;
    for(i=0;i<TABLE_SIZE;i++){
        sym_table_content *con=table->entries[i];
        while(con!=NULL){
            if(con->sym_t==data_sym){
                con->symbol_address+=ic;
            }
            con=con->next;
        }

    }
}
void free_code_image(machine_word code_img[],long ic){
    int i;
    for(i=0;i<ic;i++){
        if(code_img[i].label_name!=NULL){
            free(code_img[i].label_name);
            code_img[i].label_name=NULL;
        }
        if(code_img[i].type_word.first_word!=NULL){
            free(code_img[i].type_word.first_word);
            code_img[i].type_word.first_word=NULL;
        }
        else if(code_img[i].type_word.imm_or_dir_word!=NULL){
            free(code_img[i].type_word.imm_or_dir_word);
            code_img[i].type_word.imm_or_dir_word=NULL;
        }
        if(code_img[i].type_word.reg_word!=NULL){
            free(code_img[i].type_word.reg_word);
            code_img[i].type_word.reg_word=NULL;
        }
    }
}