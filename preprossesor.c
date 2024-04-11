//
// Created by ניב on 05/04/2024.
//

#include "preprossesor.h"
#include "globals.h"
void extract_macro_name(const char *line, char *buffer, size_t buffer_size) {
    if (!line || !buffer || buffer_size == 0) return;

    const char *start = line;  // Start of the macro name.
    const char *end = line;    // End of the macro name.

    // Find the end of the macro name.
    while (*end && !isspace((unsigned char)*end)) {
        end++;
    }

    // Calculate the length of the macro name.
    size_t length = end - start;

    // Ensure the length does not exceed the buffer size minus one (for the null terminator).
    if (length >= buffer_size) {
        length = buffer_size - 1;
    }

    // Copy the macro name into the buffer and null-terminate it.
    strncpy(buffer, start, length);
    buffer[length] = '\0';
}
macro_states line_examination(char *line, macro_table *table) {

    char macro_name[MAX_LINE_LENGTH]; // Ensure this size is adequate for your needs.

    // Check for the end of a macro declaration.
    if (strstr(line, END_MACRO_DEC)) {
        return end_macro_definition;
    }

    // Check for the start of a new macro declaration.
    char *tok = strstr(line, MACRO_DEC);
    if (tok) {
        tok += strlen(MACRO_DEC)+1; // Move past declaration keyword.
        skip_space(&tok); // Adjust 'tok' to skip any whitespace.
        extract_macro_name(tok, macro_name, sizeof(macro_name));

        if (!get_lines(table, macro_name)) {
            // New macro, insert into the table with an initial placeholder for content.
            insert_macro_content(table, macro_name, "");
        }
        return macro_definition;
    }

    // Handle macro invocation or any other line.
    extract_macro_name(line, macro_name, sizeof(macro_name));
    if (get_lines(table, macro_name)) {
        return macro_call; // Macro name found, treat the line as a macro call.
    } else {
        return any_other_line; // Treat the line as any other line.
    }
}
const char *preprocessor(char *file_name, char *str) {
    char line_buffer[MAX_LINE_LENGTH] = {0};
    macro_states result;
    size_t as_file_name_len,am_file_name_len;
    char *as_file_name, *am_file_name;
    FILE *am_file, *as_file;
    macro_table *table = create_macro_table(); // Use create_macro_table to initialize the macro table.
    struct macro_table_content *current_macro = NULL;
    int line_count = 1;

    as_file_name_len = strlen(file_name);
    as_file_name = strcat(strcpy(malloc(as_file_name_len + strlen(AS_FILE_EXTENSION) + 1), file_name), AS_FILE_EXTENSION);
    file_name = strcat(strcpy(malloc(strlen(str) + strlen(file_name) + 1), str), file_name);
    am_file_name_len = strlen(file_name);
    am_file_name = strcat(strcpy(malloc(am_file_name_len + strlen(AM_FILE_EXTENSION) + 1), file_name), AM_FILE_EXTENSION);

    as_file = fopen(as_file_name, "r");
    if (as_file == NULL) {
        free(as_file_name);
        free(am_file_name);
        return NULL;
    }

    am_file = fopen(am_file_name, "w");
    if (am_file == NULL) {
        free(as_file_name);
        free(am_file_name);
        return NULL;
    }

    while (fgets(line_buffer, sizeof(line_buffer), as_file)) {
        result = line_examination(line_buffer, &current_macro);
        switch (result) {
            case macro_definition:
                // Macro definition start, handled within line_examination
                break;
            case end_macro_definition:
                // End of macro definition, current_macro set to NULL within line_examination
                break;
            case macro_call:
                // For a macro call, insert the macro's lines to the output file
                fputs(get_lines(table, current_macro->macro_name), am_file);
                current_macro = NULL;
                break;
            case any_other_line:
                // For any other line, either append to the current macro or write directly to the file
                if (current_macro) {
                    // Assuming there's a way to append lines to the current macro's content in your implementation
                    append_macro_lines(table, current_macro->macro_name, line_buffer);
                } else {
                    fputs(line_buffer, am_file);
                }
                break;
        }
        line_count++;
    }

    // Cleanup and close files
    free_macro_table(table);
    free(as_file_name);
    fclose(as_file);
    fclose(am_file);
    return am_file_name; // Caller should handle freeing this memory
}