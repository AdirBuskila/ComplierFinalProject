//
// Created by ניב on 05/04/2024.
//

#include "tabels.h"

size_t hash(const char *key){
    size_t hash = 0;
    int c;

    while ((c = *key++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    return hash % TABLE_SIZE;
}
macro_table *create_macro_table(){
    macro_table *table = (macro_table *)calloc(1, sizeof(macro_table));
    if (!table) {
        // Handle allocation failure if necessary
        return NULL;
    }

    // Allocate and zero-initialize an array of pointers for the entries
    table->entries = (macro_table_content **)calloc(TABLE_SIZE, sizeof(macro_table_content *));
    if (!table->entries) {
        // Handle allocation failure if necessary
        free(table); // Clean up previously allocated table before returning
        return NULL;
    }

    return table;

}
macro_table_content *create_macro_content(const char *macro_name,const char *lines){

    macro_table_content *entry= malloc(sizeof(macro_table_content));
    if(entry==NULL)
        return NULL;
    entry->macro_name= (char*)malloc(strlen(macro_name)+1);
    if(entry->macro_name==NULL) {
        free(entry);
        return NULL;
    }
    entry->lines= (char*)malloc(strlen(lines)+1);
    if(entry->lines==NULL){
        free(entry->macro_name);
        free(entry);
        return NULL;
    }
    strcpy(entry->macro_name,macro_name);
    strcpy(entry->lines,lines);
    entry->next==NULL;
    return entry;
}
void insert_macro_content(macro_table *table,const char *macro_name,const char *lines){
    if(!table)
        return;
    size_t bucket= hash(macro_name);
    macro_table_content *curr=table->entries[bucket];
    macro_table_content *prev=NULL;
    while(curr!=NULL&& strcmp(curr->macro_name,macro_name)!=0){
        prev=curr;
        curr=curr->next;
    }
    if (curr != NULL) {
        // Entry exists, update the macro content
        free(curr->lines); // Free the old content
        curr->lines = strdup(lines); // Set new content
        if (!curr->lines) {
            // Handle allocation failure if necessary (reverting to the previous state could be complex)
            // This is a simplification. In a full implementation, you might want to restore the original state.
            return;
        }
    }else{
        macro_table_content *new_content= create_macro_content(macro_name,lines);

        // Entry does not exist, create and insert a new one

        if (!new_content) {
            // Handle allocation failure if necessary
            return;
        }
        if (prev == NULL) {
            // Inserting as the first entry in the bucket
            table->entries[bucket] = new_content;
        } else {
            // Inserting after an existing entry
            prev->next = new_content;
        }
    }

}
char *get_lines(const macro_table *table, const char *macro_name) {
    if (!table)
        return NULL; // Safeguard against null input.

    // Calculate the index for the macro name using the hash function.
    size_t index = hash(macro_name);
    macro_table_content *con=table->entries[index];
    // Iterate through the linked list at the calculated bucket index.
    for (; con != NULL; con = con->next) {
        // Check if the current entry's macro name matches the search query.
        if (strcmp(con->macro_name, macro_name) == 0) {
            // If a match is found, return the macro content.
            return con->lines;
        }
    }

    // If no matching entry is found, return NULL.
    return NULL;
}
void delete_macro_content(macro_table *table, const char *macro_name) {
    if (!table ) return; // Safety check

    unsigned int bucket = hash(macro_name);
   macro_table_content **con_ptr = &table->entries[bucket];

    while (*con_ptr) {
        if (strcmp((*con_ptr)->macro_name, macro_name) == 0) {
            macro_table_content *temp_con = *con_ptr; // Temporarily store the entry to delete
            *con_ptr = temp_con->next; // Unlink the entry from the list

            // Free the memory allocated for the entry
            free(temp_con->macro_name);
            free(temp_con->lines);
            free(temp_con);

            return; // Exit after deletion
        }
        con_ptr = &(*con_ptr)->next; // Move to the next entry
    }
}
void print_macro_table(const macro_table *table) {
    if (!table) return; // Safety check to ensure table is not NULL.

    for (int i = 0; i < TABLE_SIZE; ++i) {
        macro_table_content *con = table->entries[i];
        // Check if the current bucket is not empty before printing its contents.
        if (!con) continue; // Skip empty buckets.

        printf("Bucket[%d]:", i);
        while (con) { // Iterate through each entry in the bucket.
            printf(" %s=\"%s\"", con->macro_name, con->lines);
            con = con->next; // Move to the next entry.
            if (con) printf(","); // Print a comma between entries for clarity.
        }
        printf("\n"); // Newline after each bucket for readability.
    }
}
void free_macro_table(macro_table *hash){
    int i;
    for(i=0;i<TABLE_SIZE;i++){
        if(hash->entries[i]!=NULL){
            macro_table_content *con=hash->entries[i];
            while(con!=NULL){
                macro_table_content *prev=con;
                con=con->next;
                free(prev->macro_name);
                free(prev->lines);
                free(prev);
            }
        }
    }
    free(hash->entries);
    free(hash);
}
macro_table_content *get_macro_content(const macro_table *table, const char *macro_name) {
    if (!table || !macro_name) return NULL; // Safety checks.

    // Calculate the bucket index using the hash function for the macro name.
    size_t index = hash(macro_name);

    // Get the head of the list for this bucket.
    macro_table_content *entry = table->entries[index];

    // Traverse the linked list in the bucket to find the entry with the matching name.
    while (entry != NULL) {
        if (strcmp(entry->macro_name, macro_name) == 0) {
            // If a match is found, return the pointer to this macro content.
            return entry;
        }
        entry = entry->next; // Move to the next entry in the bucket.
    }

    // If no entry is found with the matching name, return NULL.
    return NULL;
}
void append_macro_lines(macro_table *table, const char *macro_name, const char *new_line) {
    if (!table || !macro_name || !new_line) return; // Basic validation.

    // Find the macro in the table.
    macro_table_content *macro = get_macro_content(table, macro_name);
    if (!macro) {
        // If the macro doesn't exist, there's nothing to append to.
        return;
    }

    // Calculate new content size.
    size_t new_content_length = macro->lines ? strlen(macro->lines) + strlen(new_line) + 2 : strlen(new_line) + 1;
    // Allocate/Reallocate memory for the new content size.
    char *new_content = realloc(macro->lines, new_content_length);
    if (!new_content) {
        // Handle allocation failure.
        return;
    }

    macro->lines = new_content;
    // If it's not the first line, add a newline separator.
    if (new_content_length > strlen(new_line) + 1) {
        strcat(macro->lines, "\n");
    }
    // Append the new line.
    strcat(macro->lines, new_line);
}
sym_table *create_sym_table(){
    sym_table *table = (sym_table *)calloc(1, sizeof(sym_table));
    if (!table) {
        // Handle allocation failure if necessary
        return NULL;
    }

    // Allocate and zero-initialize an array of pointers for the entries
    table->entries = (sym_table_content **)calloc(TABLE_SIZE, sizeof(sym_table_content *));
    if (!table->entries) {
        // Handle allocation failure if necessary
        free(table); // Clean up previously allocated table before returning
        return NULL;
    }

    return table;

}
sym_table_content *create_sym_content(const char *sym_name,int sym_add,sym_types sym_t){
    if(sym_name==NULL)
        return NULL;
    sym_table_content *con=(sym_table_content*) malloc(sizeof(sym_table_content));
    if(!con)
        return NULL;
    con->symbol= strdup(sym_name);
    if(!con->symbol){
        free(con);
        return NULL;
    }
    con->symbol_address=sym_add;
    con->sym_t=sym_t;
    con->next=NULL;
    return con;
}
void insert_symbol_content(sym_table *table,const char *sym_name,int sym_add,sym_types sym_t){
    if (!table || !sym_name) return; // Safety checks.

    size_t bucket = hash(sym_name); // Use the hash function to find the correct bucket.
    sym_table_content **con_ptr = &table->entries[bucket];

    // Iterate through each entry in the bucket until the end is reached or a match is found.
    while (*con_ptr) {
        if (strcmp((*con_ptr)->symbol, sym_name) == 0) {
            // Match found - update the existing entry.
            (*con_ptr)->symbol_address = sym_add;
            (*con_ptr)->sym_t = sym_t;
            return; // No need to allocate a new entry, just update the existing one.
        }
        con_ptr = &(*con_ptr)->next; // Move to the next node.
    }

    // No existing entry found, create a new one.
    *con_ptr = create_sym_content(sym_name, sym_add, sym_t);

}
sym_table_content *find_sym_content(const sym_table *table,const char *sym_name){
    if(!table||!sym_name)
        return NULL;
    size_t index= hash(sym_name);
    sym_table_content *con=table->entries[index];
    for(;con!=NULL;con=con->next){
        if(strcmp(con->symbol,sym_name)==0){
            return con;
        }
    }
    return NULL;
}
void delete_sym_content(sym_table *table,const char *sym_name){
  if(!table||!sym_name)
      return;
  size_t index= hash(sym_name);
  sym_table_content  **con_ptr=&table->entries[index];
    while (*con_ptr) {
        if (strcmp((*con_ptr)->symbol, sym_name) == 0) {
            sym_table_content *to_delete = *con_ptr; // Entry to delete.
            *con_ptr = to_delete->next; // Bypass the deleted entry.

            // Free the memory allocated for the symbol and the entry itself.
            free(to_delete->symbol);
            free(to_delete);
            return; // Early return after deletion.
        }
        con_ptr = &(*con_ptr)->next; // Move to the next entry.
    }
}
void dump_symbol_table(const sym_table *table) {
    if (!table) return; // Check for NULL table.
     int i;
     for(i=0;i<TABLE_SIZE;i++){
         sym_table_content *con=table->entries[i];
         if(!con)
             continue;
         for(;;){
             printf("%s[%d] in %d\n",con->symbol,con->sym_t,(con->symbol_address)+100);
             if(con->next==NULL)
                 break;
             con=con->next;
         }
     }
}
void destroy_symbol_table(sym_table *table){
    if(!table)
        return;
    int i;
    for(i=0;i<TABLE_SIZE;i++){
        sym_table_content *con = table->entries[i];
        while (con) {
            sym_table_content *toFree = con;
            con = con->next; // Move to the next entry before freeing the current one.
            free(toFree->symbol); // Assume symbol was dynamically allocated.
            free(toFree); // Free the entry itself.
        }
    }

    // After all entries have been freed, free the entries array and the table structure.
    free(table->entries);
    table->entries = NULL; // Guard against dangling pointer.
    free(table);
    }
