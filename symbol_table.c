#include "symbol_table.h"

void get_id(const char* id){
    memset(aux, 0, 1000);
    strcpy(aux, id + 3);
    aux[strlen(aux)-1] = '\0';
}



void print_table(table_t* table){
    if (table == NULL) return;
    printf("===== %s Symbol Table =====\n", table->name);
    symbol_t* s = table->first;
    while (s != NULL){
        printf("%s\t", s->name);
        if (table->classe == 1){
            if (s->func) printf("(");
            if (s->paramtypes != NULL){
                printf("%s", type2str(s->paramtypes->type));
                paramtypes_t* pt = s->paramtypes->next;
                while (pt != NULL){
                    printf(",%s", type2str(pt->type));
                    pt = pt->next;
                }
            }
            if (s->func) printf(")");
        }
        printf("\t");
        printf("%s", type2str(s->type));
        if (s->param) printf("\tparam");
        printf("\n");
        s = s->next;
    }
    printf("\n");
    print_table(table->next);
}


table_t* get_table(table_t* table,int id){
    while(table!=NULL){
	if(table->id==id){
	return table;
}
	table=table->next;
	}
    return NULL;
}

table_t* new_table(const char* name){
    table_t* table = (table_t *) malloc(sizeof(table_t));
    table->name = strdup(name);
    table->classe=0;
    table->first = NULL;
    table->last = NULL;
    table->next = NULL;
    return table;
}

table_t* new_table_class(const char* name){
    table_t* table = (table_t *) malloc(sizeof(table_t));
    char arr[1000];
    strcpy(arr, "Class ");
    get_id(name);
    strcat(arr, aux);
    table->name = strdup(arr);
    table->classe=1;
    table->first = NULL;
    table->last = NULL;
    table->next = NULL;
    return table;
}



table_t* new_function_table(const char* name, char* return_type, paramtypes_t* pt){
    char arr[1000];
    strcpy(arr, "Method ");
    get_id(name);
    strcat(arr, aux);
    strcat(arr, "(");
    if (pt != NULL){
        strcat(arr, type2str(pt->type));
        pt = pt->next;
        while (pt != NULL){
            strcat(arr, ","); strcat(arr, type2str(pt->type));
            pt = pt->next;
        }
    }
    strcat(arr, ")");
    table_t* functable = new_table(arr);
    if (strcmp(return_type,"Void")==0){ insert_symbol(functable, new_symbol("return", "none", NULL, false, false, 0, 0));
    }else{ insert_symbol(functable, new_symbol("return", return_type, NULL, false, false, 0, 0));}
    return functable;
}

symbol_t* new_symbol(const char* name,const char* type, paramtypes_t* paramtypes, bool param, bool func, int line, int col){
    symbol_t* symbol = (symbol_t *) malloc(sizeof(symbol_t));
    symbol->name = strdup(name);
    if(func==true){
	symbol->type = str2type(type,true);
    }else{
   	symbol->type = str2type(type,false);
    } 

    symbol->param = param;
    symbol->paramtypes = paramtypes;
    symbol->func = func;
    // Se é funcao é "usado" por default, se for variavel nao é usado automaticamente
    symbol->used = func;
    symbol->line = line;
    symbol->col = col;
    symbol->id=-1;
    symbol->next = NULL;
    return symbol;
}

symbol_t* new_var_symbol(const char* name, const char* type, bool param, int line, int col){
symbol_t* symbol=new_symbol(name, type, NULL, param, false, line, col);
    return symbol;
}

symbol_t* new_function_symbol(node* funcheader){
    node* id = funcheader->son->next;
    get_id(id->name);
    //type_t type = str2type(funcheader->son->name,false);
	
    if (strcmp(id->next->name, "MethodParams") == 0){ // nó type nao esta no funcheader, logo no[1] = params
        return new_symbol(aux, funcheader->son->name, new_paramtypes(id->next->son), false, true, funcheader->line, funcheader->col);
    } 
    else { // no[2] = params
        return new_symbol(aux, id->next->name, new_paramtypes(id->next->next->son), false, true, funcheader->line, funcheader->col);
    }
}

paramtypes_t* new_parameter(const char* type){
    paramtypes_t* param = (paramtypes_t *) malloc(sizeof(paramtypes_t));
    param->type = str2type(type,false);
    param->next = NULL;
    return param;
}

paramtypes_t* new_paramtypes(node* paramdecl){
    if (paramdecl == NULL) return NULL;
    paramtypes_t* newparam = (paramtypes_t *) malloc(sizeof(paramtypes_t));
    newparam->type = str2type(paramdecl->son->name,false);
    newparam->next = new_paramtypes(paramdecl->next);
    return newparam;
}

void insert_symbol(table_t* table, symbol_t* symbol){
    if (table->first == NULL){
        table->first = symbol;
        table->last = symbol;
    } else {
        if (find_symbol_table(table, symbol->name,symbol->func) == NULL){
            table->last->next = symbol;
            table->last = symbol;
        } else {
            printf("Line %d, col %d: Symbol %s already defined\n", symbol->line, symbol->col, symbol->name);
        }
    }
}

void insert_symbol_func(table_t* table, symbol_t* symbol){
    if (table->first == NULL){
        table->first = symbol;
        table->last = symbol;
    } else {
            table->last->next = symbol;
            table->last = symbol;
    }
}

int check_exist_func(table_t* table, char* name,node* params){
    symbol_t* s = table->first;
	int verifica=0;
    while (s != NULL){
        if (strcmp(s->name, name) == 0 ){
	    verifica=1;
            int len_funcparams = 0; 
		paramtypes_t* p = s->paramtypes;
		while (p != NULL){ 
			len_funcparams++; 
			p = p->next; 
		}
		int len_callparams = 0; 
		node* p2 = params->son;
		while (p2 != NULL){ 
			len_callparams++; 
			p2 = p2->next; 
		}
		if (len_callparams != len_funcparams){
		    verifica=0;
		} else {
		    p = s->paramtypes; 
		    p2 = params->son;
		    while (p2 != NULL){
		        if (str2type(p2->son->name,false) != p->type){
		            verifica= 0;
				break;
		        }
		        p2 = p2->next;
		        p = p->next;
		    }
		}
        }
        s = s->next;
    }
	
    return verifica;
}


symbol_t* find_func_symbol(table_t* table, int i){
    symbol_t* s = table->first;
    while (s != NULL){
        if (s->id==i){
            return s;
        }
        s = s->next;
    }
    if (table->classe == 1) return NULL;
    else return find_func_symbol(global, i);
}



symbol_t* find_symbol(table_t* table, char* name,bool f){
    symbol_t* s = table->first;
    while (s != NULL){
        if (strcmp(s->name, name) == 0 && s->func==f){
            return s;
        }
        s = s->next;
    }
    if (table->classe == 1) return NULL;
    else return find_symbol(global, name,f);
}

symbol_t* find_symbol_table(table_t* table, char* name,bool f){
    symbol_t* s = table->first;
    while (s != NULL){
        if (strcmp(s->name, name) == 0 && s->func==f){
            return s;
        }
        s = s->next;
    }
    return NULL;
    
}

symbol_t* find_par(table_t* table, char* name){
    symbol_t* s = table->first;
    while (s != NULL){
	
        if (strcmp(s->name, name) == 0 ){
            return s;
        }
        s = s->next;
    }
    return NULL;
    
}



symbol_t* find_symbol_call_parecido(table_t* table, char* name,bool f, node* origin){
    symbol_t* s = table->first;
	symbol_t* save; 
	int amb=0;
	while (s != NULL){
	node* cur_node=origin;
	int verifica=0;
        if (strcmp(s->name, name) == 0 ){		
		verifica=1;
		int len_funcparams = 0; 
		paramtypes_t* p = s->paramtypes;
		while (p != NULL){ 
			len_funcparams++; 
			p = p->next; 
		}
		int len_callparams = 0; 
		node* p2 = cur_node->son->next;
		while (p2 != NULL){ 
			len_callparams++; 
			p2 = p2->next; 
		}
		if (len_callparams != len_funcparams){
		    get_id(cur_node->son->name);
		    verifica=0;
		} else {
		    //bool erro;
		    p = s->paramtypes; 
		    p2 = cur_node->son->next;
		    while (p2 != NULL){
		        if (p2->type != p->type){
				if(!(p->type==_DOUBLE && p2->type== _INT)){
					verifica=0;

				}
		        }
		        p2 = p2->next;
		        p = p->next;
		    }
		}
		
        }
	if(verifica==1){amb++;save=s;}
        s = s->next;
    }    
	if(amb==1){return save;}
	else if(amb>1){
		ambiguo=1;
		node* cur_node=origin;
		cur_node->son->type=_UNDEF;
		char vec[1000];
		get_id(cur_node->son->name);
		strcpy(vec,aux);
		strcat(vec,"(");
		node* zx=cur_node->son->next;
		if (zx!=NULL){
			check_tp(table,zx,cur_node);
			strcat(vec,type2str(zx->type));
			zx=zx->next;
			if (zx!=NULL){
				while(zx!=NULL){
					strcat(vec,",");
					check_tp(table,zx,cur_node);
					strcat(vec,type2str(zx->type));
					zx=zx->next;
				}	
			}
		}
		strcat(vec,")");
		strcpy(aux,vec);
		printf("Line %d, col %d: Reference to method %s is ambiguous\n",cur_node->line,cur_node->col,aux);
		return save;
	}
	
	
    if (table->classe == 1) return NULL;
    else return find_symbol_call(global, name,f,origin);
}

void check_tp(table_t* table, node* cur_node,node* pai){


    if (starts_w("Id", cur_node->name)){
        get_id(cur_node->name);
		symbol_t* s = find_symbol(table, aux,false);
		symbol_t* s2 = find_symbol(table, aux,true);
		if (s != NULL){
		    cur_node->type = s->type;
		    s->used = true;
		}else if (s==NULL && s2 != NULL){
		    cur_node->type = s2->type;
		    s2->used = true;
		}  
	
    } else if (starts_w("DecLit", cur_node->name)){
	
	cur_node->type = _INT;
    } else if (starts_w("RealLit", cur_node->name)){
 	
	cur_node->type = _DOUBLE;

    } else if (starts_w("BoolLit", cur_node->name)){
        cur_node->type = _BOOL;
    } 
}

bool starts_w(char* start, char* str){
    return strncmp(start, str, strlen(start)) == 0;
}


symbol_t* find_symbol_call(table_t* table, char* name,bool f, node* origin){
    symbol_t* s = table->first;
	while (s != NULL){
	node* cur_node=origin;
	int verifica=0;
        if (strcmp(s->name, name) == 0 ){		
		verifica=1;
		int len_funcparams = 0; 
		paramtypes_t* p = s->paramtypes;
		while (p != NULL){ 
			len_funcparams++; 
			p = p->next; 
		}
		int len_callparams = 0; 
		node* p2 = cur_node->son->next;
		while (p2 != NULL){ 
			len_callparams++; 
			p2 = p2->next; 
		}
		if (len_callparams != len_funcparams){
		    get_id(cur_node->son->name);
		    verifica=0;
		} else {
		    //bool erro;
		    p = s->paramtypes; 
		    p2 = cur_node->son->next;
		    while (p2 != NULL){
		        if (p2->type != p->type){
					verifica=0;
		        }
		        p2 = p2->next;
		        p = p->next;
		    }
		}
		
        }
	if(verifica==1){return s;}
        s = s->next;
    }    

	
    if (table->classe == 1) return NULL;
    else return find_symbol_call(global, name,f,origin);
}

