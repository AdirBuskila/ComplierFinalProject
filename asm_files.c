//
// Created by ניב on 08/04/2024.
//

#include "asm_files.h"
void write_obj_file(char *file_name, machine_word code_image[], long ic, data_word data_image[], long dc){
    int i,j;
    unsigned int value;
    if (!file_name) return; // Basic check

    char *obj_file_name = str_allocate_cat(file_name, OB_FILE_EXTENSION);
    if (!obj_file_name) {
        printf("Memory allocation failed for object file name.\n");
        return;
    }

    FILE *file = fopen(obj_file_name, "w");
    if (!file) {
        printf("Couldn't open the file %s for writing.\n", obj_file_name);
        free(obj_file_name);
        return;
    }

    // Writing header information
    fprintf(file, "\t\t%ld %ld\n", ic, dc);

    // Writing code image
    for ( i = 0; i < ic; ++i) {

        if (code_image[i].type_word.first_word) {
            value=*(unsigned int *)code_image[i].type_word.first_word;
        }
        fprintf(file, "0%d\t\t", i + DATA_SEGMENT_START_ADDRESS);
            for ( j = 13; j >= 0; --j) {
                fprintf(file, "%c", ((value >> j) & 1) ? '/' : '.');
            }

        fprintf(file, "\n");
    }

    // Writing data image
    for (i = 0; i < dc; ++i) {
        fprintf(file, "0%ld\t\t", i + ic + DATA_SEGMENT_START_ADDRESS);
        for ( j = 13; j >= 0; --j) {
            fprintf(file, "%c", (((data_image[i].data) >> j) & 1) ? '/' : '.');
        }
        fprintf(file, "\n");
    }

    fclose(file); // Ensure the file is closed properly.
    free(obj_file_name); // Clean up dynamically allocated memory.
}
void write_entry_file(char *file_name,sym_table *table){
    if(!file_name||!table)
        return;
    int i;
    char *entry_file_name= str_allocate_cat(file_name,ENT_FILE_EXTENSION);
    if(!entry_file_name){
        printf("Memory allocation failed for entry file name.\n");
        return;
    }
    FILE *file= fopen(entry_file_name,"w");
    if(!file){
        printf("Couldn't open the file %s for writing.\n", entry_file_name);
        free(entry_file_name);
        return;
    }
    for ( i = 0; i < TABLE_SIZE; ++i) {
        sym_table_content *temp= table->entries[i];
        for ( ; temp != NULL; temp = temp->next) {
            if (temp->sym_t == entry_sym) {
                fprintf(file, "%s\t\t%d\n", temp->symbol, temp->symbol_address + DATA_SEGMENT_START_ADDRESS);
            }
        }
    }
    free(entry_file_name);
    fclose(file);

}
void write_extern_file(char *file_name,sym_table *table,machine_word code_img[],long ic){
    if(!file_name || !table)
        return; // Check for valid input.

    char *ext_file_name = str_allocate_cat(file_name, EXT_FILE_EXTENSION);
    if (!ext_file_name) {
        printf("Error allocating memory for the external file name.\n");
        return;
    }
    int i;
    int has_extern = 0; // Use int as a boolean flag: 0 = false, non-zero = true.
    for ( i = 0; i < TABLE_SIZE; ++i) {
        sym_table_content *temp= table->entries[i];
        for (; temp; temp = temp->next) {
            if (temp->sym_t == extern_sym) {
                has_extern = 1; // Found an external symbol, set flag to true.
                break;
            }
        }
        if (has_extern) break; // Break the outer loop if an extern symbol is found.
    }

    if (!has_extern) {
        free(ext_file_name); // No externs found, skip file creation.
        return;
    }

    FILE *file = fopen(ext_file_name, "w");
    if (!file) {
        printf("Couldn't open the file %s for writing.\n", ext_file_name);
        free(ext_file_name);
        return;
    }

    for (i = 0; i < ic; ++i) {
        if (code_img[i].label_name != NULL) {
            sym_table_content *temp = find_sym_content(table,code_img[i].label_name);
            if (temp && temp->sym_t == extern_sym) {
                fprintf(file, "%s   %d\n", temp->symbol, i + DATA_SEGMENT_START_ADDRESS);
            }
        }
    }

    fclose(file); // Close the file.
    free(ext_file_name); // Free allocated filename memory.
}
