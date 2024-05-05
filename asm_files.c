//
// Created by ניב on 08/04/2024.
//

#include "asm_files.h"
#include "globals.h"
#include "tabels.h"
#include <stdlib.h>
#include <stdio.h>
void write_obj_file(char *filename, machine_word code_image[], long ic, data_word data_image[], long dc) {
    FILE *file_des;
    int i, j;
    unsigned int value;
    char *obj_filename = str_allocate_cat(filename, ".ob");

    /* Try to create a new .ob file */
    file_des = fopen(obj_filename, "w");
    if(file_des == NULL) {
        printf("Couldn't open the file %s.\n", obj_filename);
        free(obj_filename);
        return;
    }

    fprintf(file_des, "\t\t%ld %ld\n", ic, dc); /* Print the header */

    /* Print the code image */
    for (i = 0; i < ic; i++) {
        if (code_image[i].type_word.first_word != NULL) {
            value = *(unsigned int*) code_image[i].type_word.first_word;
        }
        fprintf(file_des, "0%d\t\t", i + 100); // Assuming IC_INIT_VALUE is 100
        for (j = 14; j >= 0; j -= 2) { // Process two bits at a time
            switch ((value >> j) & 3) { // Extract two bits
                case 0: fprintf(file_des, "*"); break;
                case 1: fprintf(file_des, "#"); break;
                case 2: fprintf(file_des, "%"); break;
                case 3: fprintf(file_des, "!"); break;
            }
        }
        fprintf(file_des, "\n");
    }

    /* Print the data image */
    for (i = 0; i < dc; i++) {
        fprintf(file_des, "0%ld\t\t", i + ic + 100); // Assuming IC_INIT_VALUE is 100
        for (j = 14; j >= 0; j -= 2) { // Process two bits at a time
            switch ((data_image[i].data >> j) & 3) { // Extract two bits
                case 0: fprintf(file_des, "*"); break;
                case 1: fprintf(file_des, "#"); break;
                case 2: fprintf(file_des, "%"); break;
                case 3: fprintf(file_des, "!"); break;
            }
        }
        fprintf(file_des, "\n");
    }

    /* Free the allocated memory */
    free(obj_filename);
    fclose(file_des);
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
