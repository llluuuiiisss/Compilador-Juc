#include "ast.h"
type_t str2type(const char* type,bool f){
    if (type == NULL || strcmp(type, "none") == 0|| strcmp(type, "Void") == 0) return _NONE;
    else if (strcmp(type, "Int") == 0) return _INT;
    else if (strcmp(type, "Bool") == 0) return _BOOL;
    else if (strcmp(type, "Double") == 0) return _DOUBLE;
    else if (strcmp(type, "Double") == 0) return _STR;
    else if (strcmp(type, "StringArray") == 0 && !f) return _STRING;
    else if (strcmp(type, "StringArray") == 0 && f) return _STRINGF;
    else return _NONE;
}

char* type2str(type_t type){
    switch (type){
        case _INT: return "int";
        case _BOOL: return "boolean";
	case _DOUBLE: return "double";
	case _STR: return "String";
	case _STRING: return "String[]";
	case _STRINGF: return "(String[])";
        case _NONE: return "void";
        case _UNDEF: return "undef";
    }
}

node* create_node(const char* name, int line, int col){
	//printf("Creating node %s\n", name);
	node* n = (node *) malloc(sizeof(node));
	if (name == NULL){
		n->name = NULL;
	} else {
		n->name = strdup(name);
	}
	n->line = line;
	n->col = col;
	n->param = false;
	return n;
}

void add_next(node* n, node* n2){
	if (n == NULL || n2 == NULL){
		return;
	}
	if (n2->name == NULL){
		free(n2);
		return;
	}
	node* temp = n;
	//printf("Adding next %s to %s\n", n2->name, n->name);
	if (temp->next == NULL){
		temp->next = n2;
		n2->prev = temp;
	} else {
		while (temp->next != NULL){
			temp = temp->next;
		}
		temp->next = n2;
		n2->prev = temp;
	}
}

void add_son(node* n, node* son){
	if (n == NULL || son == NULL){
		return;
	}
	if (son->name == NULL){
		free(son);
		return;
	}
	//printf("Adding son %s to %s\n", son->name, n->name);
	if (n->son == NULL){
		n->son = son;
		son->prev = NULL;
	} else {
		add_next(n->son, son);
	}
}

int conta_irmaos(node *n){
	int soma=0;
	node*aux=n;
	while(aux!=NULL){
		aux=aux->next;
		soma++;	
	}
	return soma;
}

bool is_block(node* n){
	if (n == NULL || n->next == NULL) return false;
	return true;
}

bool is_null(node* n){
	if (n == NULL ) return false;
	return true;
}

node* new_id(const char* name, int line, int col){
	char* arr = (char *) malloc(strlen(name) + 5);
	sprintf(arr, "Id(%s)", name);
	return create_node(arr, line, col);
}

void print_tree(int level, node* n, bool anot){
	if (n == NULL) return;
	for (int i = 0; i < level; i++) printf("..");
	printf("%s", n->name);
	if(anot && n->callt!=NULL){
		printf(" - (");
		call_t* ax=n->callt;
		printf("%s",type2str(ax->type));
		ax=ax->next;
		while(ax!=NULL){
			printf(",%s",type2str(ax->type));
			ax=ax->next;
		}
		printf(")");
	}
	else if (anot && n->type!=_NONE){
		if (n->param){ printf(" - (%s)", type2str(n->type)); }
		else { printf(" - %s", type2str(n->type));}
	}
	else if(anot && strcmp(n->name, "Call") == 0){
		printf(" - %s", type2str(n->type));
	}
	else if(anot && n->func==1){
		printf(" - ()");
	}
	//printf("\t%d:%d", n->line, n->col);
	printf("\n");
	print_tree(level + 1, n->son, anot);
	print_tree(level, n->next, anot);
}

void free_tree(node* n){
	if (n == NULL){
		return;
	}
	if (n->son != NULL){
		free_tree(n->son);
	}
	if (n->next != NULL){
		free_tree(n->next);
	}
	free(n->name);
	free(n);
}



int get_line(struct _args* n){
	if (n == NULL) return 0;
	else return n->line;
}

int get_col(struct _args* n){
	if (n == NULL) return 0;
	else return n->col;
}

int get_line_node(node* n){
	if (n == NULL) return 0;
	else return n->line;
}

int get_col_node(node* n){
	if (n == NULL) return 0;
	else return n->col;
}

