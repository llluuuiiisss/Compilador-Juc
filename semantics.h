#include "symbol_table.h"

extern struct st* global;
extern struct st* cur_table;
extern char aux[1000];
extern int ambiguo;


int* define_methods(node* tree,int* arr);
void sem(node* tree);
void check_func_parameters(table_t* table, node* cur_node);
void check_func_body(table_t* table, node* cur_node,node* pai);
bool starts_with(char* start, char* str);
void check_return_statement(node* cur_node, type_t func_return, type_t expr_return);
void check_if_for_statement(node* cur_node, char* s, type_t expr_type);
void check_assign_statement(node* cur_node, type_t expr1_type, type_t expr2_type);
void check_call_statement(symbol_t* func, node* cur_node,table_t* table);
void check_parseargs_statement(node* cur_node, node* expr1, node* expr2);
void check_binary_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type);
void check_plus_minus_expr(node* cur_node, char* s, type_t expr_type);
void check_not_expr(node* cur_node, type_t expr_type);
void check_logical_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type);
void check_comp_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type);
void check_comp_expr_ig(node* cur_node, char* s, type_t expr1_type, type_t expr2_type);
void check_statement(table_t* table, node* cur_node);
void check_int(node* n);
void check_double(node* n);
void check_type(table_t* table, node* cur_node,node* pai);
void check_unused_symbols(table_t* table);
symbol_t* new_vardecl(node* vardecl, bool param);
symbol_t* new_func_global_symbol(node* funcheader);
