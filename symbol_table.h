#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

struct st;
struct sym;
struct pt;

struct st* global;
char aux[1000];
int ambiguo;

typedef struct st {
    char* name;
    int classe;
    int id;
    struct sym* first;
    struct sym* last;
    struct st* next;
} table_t;

typedef struct sym {
    char* name;
    type_t type;
    struct pt* paramtypes;
    int line, col;
    bool param;
    bool func;
    bool used;
    struct sym* next;
    int id;
} symbol_t;

typedef struct pt {
    type_t type;
    struct pt* next;
} paramtypes_t;

bool starts_w(char* start, char* str);
void check_tp(table_t* table, node* cur_node,node* pai);
symbol_t* find_symbol_call_parecido(table_t* table, char* name,bool f, node* origin);
table_t* get_table(table_t* table,int id);
int check_exist_func(table_t* table, char* name,node* params);
symbol_t* find_func_symbol(table_t* table, int i);
void insert_symbol_func(table_t* table, symbol_t* symbol);
void get_id(const char* id);
void print_table(table_t* table);
symbol_t* find_symbol_table(table_t* table, char* name,bool f);
symbol_t* find_par(table_t* table, char* name);
table_t* new_table(const char* name);
table_t* new_table_class(const char* name);
table_t* new_function_table(const char* name, char* return_type, paramtypes_t* pt);
symbol_t* new_symbol(const char* name, const char* type, paramtypes_t* paramtypes, bool param, bool func, int line, int col);
symbol_t* new_var_symbol(const char* name, const char* type, bool param, int line, int col);
symbol_t* new_function_symbol(node* funcheader);
paramtypes_t* new_parameter(const char* type);
paramtypes_t* new_paramtypes(node* paramdecl);
void insert_symbol(table_t* table, symbol_t* symbol);
symbol_t* find_symbol(table_t* table, char* name,bool f);
symbol_t* find_symbol_call(table_t* table, char* name,bool f, node* origin);
