#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum t{
	_NONE, _UNDEF, _INT, _DOUBLE,_BOOL,_STR,_STRING,_STRINGF
} type_t;

struct _args{
	int line;
	int col;
	char* val;
};

typedef struct c{
	type_t type;
	struct c* next;
} call_t;

typedef struct n{
	char *name;
	int line, col;
	type_t type;
	call_t* callt;
	int func;
	bool param;
	struct n* son;
	struct n* next;
	struct n* prev;
} node;




char* type2str(type_t type);
node* create_node(const char* name, int line, int col);
void add_next(node* n, node* n2);
void add_son(node* n, node* son);
int conta_irmaos(node *n);
bool is_block(node* n);
bool is_null(node* n);
node* new_id(const char* name, int line, int col);
void print_tree(int level, node* n, bool anot);
void free_tree(node* n);
type_t str2type(const char* type,bool f);
int get_line(struct _args* n);
int get_col(struct _args* n);
int get_line_node(node* n);
int get_col_node(node* n);


