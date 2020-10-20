#include "semantics.h"
#include <limits.h>
void sem(node* tree){
    int id=0;
int vez=0;
    node* temp = tree->son;
    global = new_table_class(temp->name);
    global->id=-1;
    table_t* cur_table = global;
    temp = temp->next;
    //node* tempor=temp;
int* arr;
    arr=(int*)calloc(300,sizeof(int));
    define_methods(temp,arr);
	
    while (temp){
        if (strcmp(temp->name, "MethodDecl") == 0){
		if(arr[vez]!=0){
			 node* funcheader = temp->son;
		    //symbol_t* func = find_func_symbol(global,id);
		    table_t* func_table=get_table(global,id);
		     cur_table->next = func_table;
		     cur_table = cur_table->next;
			
		    id++;
		   // funcparams->son = 1º paramdecl ou null
		    check_func_body(cur_table, funcheader->next->son,funcheader->next);
	 // funcheader->next = funcbody->son = vardecl ou statement
		    //check_unused_symbols(cur_table);
		}
		vez++;
        } 
        temp = temp->next;
    }
    //check_unused_symbols(global);
}

int* define_methods(node* tree,int* arr){
	int i=0;
	int vez=0;
	node* temp = tree;
	table_t* cur_table = global;
	while(temp!=NULL){
		if (strcmp(temp->name, "MethodDecl") == 0){
			node* funcheader = temp->son;
			node* funcparams;
			funcparams = funcheader->son->next->next;
			get_id(temp->son->son->next->name);
			int asd=check_exist_func(global, aux,funcparams);
			if(asd==0){
				arr[vez]=1;
				funcheader = temp->son;
				symbol_t* func=new_function_symbol(funcheader);
				func->id=i;
				insert_symbol_func(global, func);
				table_t* func_table;
			    	func_table = new_function_table(funcheader->son->next->name, funcheader->son->name, func->paramtypes);
				func_table->id=i;
			    	cur_table->next = func_table;
			    	cur_table = cur_table->next;
			    	check_func_parameters(cur_table, funcparams->son);
				i++;
			}
			else{
				check_func_parameters(cur_table, funcparams->son);
				char arr[1000]="";
				get_id(temp->son->son->next->name);
				    strcat(arr, aux);
				    strcat(arr, "(");
				node* hj=temp->son->son->next->next->son;
				    if(hj!=NULL){
					strcat(arr, type2str(str2type(hj->son->name,false)));
					}
					hj=hj->next;
					while(hj!=NULL){
						strcat(arr, ",");
						strcat(arr, type2str(str2type(hj->son->name,false)));
						hj=hj->next;
					}
				    strcat(arr, ")");
				strcpy(aux,arr);
				printf("Line %d, col %d: Symbol %s already defined\n", funcheader->line, funcheader->col, aux);
				
			}
			vez++;
			
		}
		else{
		    symbol_t* bob=new_vardecl(temp, false);
			if(strcmp(bob->name,"_")==0){
				printf("Line %d, col %d: Symbol _ is reserved\n",bob->line,bob->col);
				
			}
			else{
				insert_symbol(global, bob);
			}
		    
		}
		temp=temp->next;
	}
	return arr;
}




void check_func_parameters(table_t* table, node* cur_node){ // cur_node = paramdecl
	
    while (cur_node != NULL){
	symbol_t* bob=new_vardecl(cur_node, true);
	if(strcmp(bob->name,"_")==0){
		printf("Line %d, col %d: Symbol _ is reserved\n",bob->line,bob->col);	
	}
	else{
		insert_symbol(table, bob);
			
		
	}
        cur_node = cur_node->next;
    }
	
}

void check_func_body(table_t* table, node* cur_node,node* pai){ // cur_node = vardecl ou statement
	
    if (cur_node == NULL) {
	return;
    }
    if (strcmp(cur_node->name, "VarDecl") == 0){

	symbol_t* bob=new_vardecl(cur_node, false);
	if(strcmp(bob->name,"_")==0){
		printf("Line %d, col %d: Symbol _ is reserved\n",bob->line,bob->col);	
	}
	else{
		insert_symbol(table, bob);
	}
        check_func_body(table, cur_node->next,pai);
    } else {//statement
        check_func_body(table, cur_node->son,cur_node);
	check_type(table, cur_node,pai);//factorial chamado aqui
        check_statement(table, cur_node);
	check_func_body(table, cur_node->next,pai);

    }
}

bool starts_with(char* start, char* str){
    return strncmp(start, str, strlen(start)) == 0;
}

void check_return_statement(node* cur_node, type_t func_return, type_t expr_return){
    if(func_return!=expr_return && strcmp(type2str(expr_return),"void")==0){
	printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->line, cur_node->col-6, type2str(expr_return));}
 	
    else if(func_return!=expr_return && strcmp(type2str(func_return),"void")==0){
        printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->line+1, cur_node->col-1, type2str(expr_return));}

   else if(func_return==_INT && expr_return == _STRING){
        printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->line+1, cur_node->col-1, type2str(expr_return));}

    else if (func_return != expr_return && strcmp(type2str(func_return),"int")==0 && strcmp(type2str(expr_return),"double")==0){
        printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->son->line, cur_node->son->col, type2str(expr_return));	
	}
	 else if(func_return!=_UNDEF && expr_return==_UNDEF ){
	printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->son->line, cur_node->son->col, type2str(expr_return));
	}
	else if(func_return==_NONE && expr_return==_NONE ){
		if(cur_node->son!=NULL){
			if(strcmp(cur_node->son->name,"Call")==0){
				printf("Line %d, col %d: Incompatible type %s in return statement\n", cur_node->son->line, cur_node->son->col, type2str(expr_return));
			}
		}

	}


   } 
  

void check_if_statement(node* cur_node, char* s, type_t expr_type){
    if (expr_type != _BOOL)
        printf("Line %d, col %d: Incompatible type %s in if statement\n", cur_node->son->line, cur_node->son->col, type2str(expr_type));
}

void check_while_statement(node* cur_node, char* s, type_t expr_type){
    if (expr_type != _BOOL)
        printf("Line %d, col %d: Incompatible type %s in while statement\n", cur_node->son->line, cur_node->son->col, type2str(expr_type));
}





void check_assign_statement(node* cur_node, type_t expr1_type, type_t expr2_type){
    if(strcmp(cur_node->son->next->name, "Length") == 0){
	cur_node->son->next->type=_INT;
	if(cur_node->son->next->son->type!=_STRING){
		printf("Line %d, col %d: Operator .length cannot be applied to type %s\n", cur_node->son->next->line, cur_node->son->next->col,type2str(cur_node->son->next->son->type));
	}
	
	if(expr1_type == _UNDEF){  //expr1=cur_node->son->type      
		cur_node->type = _UNDEF;
	}else{
		cur_node->type =expr1_type;	
	}
	if(cur_node->son->type!=cur_node->son->next->type){
		if(!((expr1_type==_DOUBLE && expr2_type==_INT))){
			if(expr2_type==_NONE){printf("Line %d, col %d: Operator = cannot be applied to types %s, none\n", cur_node->line, cur_node->col, type2str(cur_node->son->type));
			}else if(expr1_type==_NONE){printf("Line %d, col %d: Operator = cannot be applied to types none, %s\n", cur_node->line, cur_node->col, type2str(cur_node->son->next->type));}	
			else{ 
		printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", cur_node->line, cur_node->col, type2str(cur_node->son->type), type2str(cur_node->son->next->type));}
		}
	}
    }
    
    else if (expr1_type != expr2_type || expr1_type == _UNDEF || expr2_type == _UNDEF || ( strcmp(type2str(expr1_type),"String[]")==0 && strcmp(type2str(expr2_type),"String[]")==0)){
	if(!((expr1_type==_DOUBLE && expr2_type==_INT))){
		if(expr2_type==_NONE){printf("Line %d, col %d: Operator = cannot be applied to types %s, none\n", cur_node->line, cur_node->col, type2str(cur_node->son->type));
		}else if(expr1_type==_NONE){printf("Line %d, col %d: Operator = cannot be applied to types none, %s\n", cur_node->line, cur_node->col, type2str(cur_node->son->next->type));}	
			else{ 
		printf("Line %d, col %d: Operator = cannot be applied to types %s, %s\n", cur_node->line, cur_node->col, type2str(expr1_type), type2str(expr2_type));
		}	
	}
        
	if(expr1_type == _UNDEF){        
		cur_node->type = _UNDEF;
	}else{
		cur_node->type =expr1_type;	
	}
    } else {
        cur_node->type = expr1_type;
    }
}

void check_call_statement(symbol_t* func, node* cur_node,table_t* table){
    if (func == NULL){
        cur_node->type = _UNDEF;
	cur_node->son->type=_UNDEF;
	char vec[1000];
	get_id(cur_node->son->name);
	strcpy(vec,aux);
	strcat(vec,"(");
	node* zx=cur_node->son->next;
	if (zx!=NULL){
		check_type(table,zx,cur_node);
		strcat(vec,type2str(zx->type));
		zx=zx->next;
		if (zx!=NULL){
			while(zx!=NULL){
				strcat(vec,",");
				check_type(table,zx,cur_node);
				strcat(vec,type2str(zx->type));
				zx=zx->next;
			}	
		}
	}
	strcat(vec,")");
	strcpy(aux,vec);
	printf("Line %d, col %d: Cannot find symbol %s\n", cur_node->son->line, cur_node->son->col, aux);
		

    } else {
        int len_funcparams = 0; 
	paramtypes_t* p = func->paramtypes;
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
            printf("Line %d, col %d: Cannot find symbol %s\n", cur_node->son->line, cur_node->son->col, aux);
            cur_node->type = _UNDEF;
        } else {
            bool erro;
            p = func->paramtypes; 
	    p2 = cur_node->son->next;
            while (p2 != NULL){
                if (p2->type != p->type){
			if(!(p->type==_DOUBLE && p2->type== _INT)){
					printf("Line %d, col %d: Incompatible type %s in %s statement\n", p2->line, p2->col, type2str(p2->type), func->name);
                    		erro = true;

				}
			  

		
                  
                }
                p2 = p2->next;
                p = p->next;
            }
            if (erro){ cur_node->type = _UNDEF;}
            else { 
		cur_node->type = func->type; 
	    	//call_t* ajuda = (call_t *) malloc(sizeof(call_t));
		call_t* ante=NULL;
		call_t* primeiro=NULL;
		p2 = cur_node->son->next;
		 p = func->paramtypes; 
            	while (p2 != NULL){
			call_t* ajuda = (call_t *) malloc(sizeof(call_t));
                	ajuda->type=p->type;
			ajuda->next=NULL;
			if(ante!=NULL){
				ante->next=ajuda;
			}
			else{
				primeiro=ajuda;
			}
			ante=ajuda;
                	p2 = p2->next;
			p = p->next;
            	}
		cur_node->son->callt=primeiro;
		cur_node->son->func=1;
            }
        }
    }
}

void check_parseargs_statement(node* cur_node, node* expr1, node* expr2){
	if(strcmp(expr2->name,"Length")==0){
		expr2->type=_INT;
	}
	cur_node->type = _INT;
	 if (expr1->type == _STRING  && expr2->type != _UNDEF){}
   else{
        printf("Line %d, col %d: Operator Integer.parseInt cannot be applied to types %s, %s\n", cur_node->line, cur_node->col-17, type2str(expr1->type),type2str(expr2->type));
    }
	
}

void check_binary_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type){
    if (expr1_type == _INT && expr2_type == _INT){
		cur_node->type = _INT;
	}
    else if (expr1_type == _DOUBLE && expr2_type == _DOUBLE){
        cur_node->type = _DOUBLE;
	}
	else if (expr1_type == _DOUBLE && expr2_type == _INT){
        cur_node->type = _DOUBLE;
	}
	else if (expr1_type == _INT && expr2_type == _DOUBLE){
        cur_node->type = _DOUBLE;
	}

    else {
	if(expr2_type==_NONE){printf("Line %d, col %d: Operator %s cannot be applied to types %s, none\n",
        cur_node->line, cur_node->col, s, type2str(expr1_type));} 
else if(expr1_type==_NONE){printf("Line %d, col %d: Operator %s cannot be applied to types none, %s\n",
        cur_node->line, cur_node->col, s, type2str(expr2_type));}	
else{ 
	cur_node->type = _UNDEF;
	printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
        cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
	}
    }
}

void check_plus_minus_expr(node* cur_node, char* s, type_t expr_type){
    if (expr_type != _INT && expr_type != _DOUBLE){
        printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", cur_node->line, cur_node->col, s, type2str(expr_type));
        cur_node->type = _UNDEF;
    } else {
        cur_node->type = expr_type;
    }
}

void check_operators(node* cur_node, char* s, type_t expr_type){
    if (expr_type == _STRING){
        printf("Line %d, col %d: Operator %s cannot be applied to type %s\n", cur_node->line, cur_node->col, s, type2str(expr_type));
        cur_node->type = _UNDEF;
    } else {
        cur_node->type = expr_type;
    }
}

void check_not_expr(node* cur_node, type_t expr_type){
    if (expr_type == _BOOL){
        cur_node->type = _BOOL;
    } else {
        printf("Line %d, col %d: Operator ! cannot be applied to type %s\n", cur_node->line, cur_node->col, type2str(expr_type));
        cur_node->type = _UNDEF;
    }
}

void check_logical_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type){
    	if (expr1_type == _BOOL && expr2_type == _BOOL && strcmp(type2str(expr2_type),"String[]")!=0 && strcmp(type2str(expr1_type),"String[]")!=0){
        	cur_node->type = _BOOL;
    	} else if (expr1_type == _INT && expr2_type == _INT){
		cur_node->type = _INT;
}	
	else {
		cur_node->type = _BOOL;
		printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
		cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
		
    	}
}

void check_comp_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type){
    if ((expr1_type == expr2_type)&& (strcmp(type2str(expr2_type),"String[]")!=0)){
        cur_node->type = _BOOL;
	if(expr2_type==_BOOL){
		printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
	}
    } 
	else if (expr1_type ==_DOUBLE && expr2_type==_INT){
        cur_node->type = _BOOL;
    } 
	else if (expr1_type ==_INT && expr2_type==_DOUBLE){
        cur_node->type = _BOOL;
    } 

	else {
        cur_node->type = _BOOL;
        if(expr2_type==_NONE)printf("Line %d, col %d: Operator %s cannot be applied to types %s, none\n", cur_node->line, cur_node->col, s, type2str(expr1_type));
	else if(expr1_type==_NONE)printf("Line %d, col %d: Operator %s cannot be applied to types none, %s\n", cur_node->line, cur_node->col, s, type2str(expr1_type));

	else printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
    }
}

void check_comp_expr_ig(node* cur_node, char* s, type_t expr1_type, type_t expr2_type){
    if ((expr1_type == expr2_type)&& (strcmp(type2str(expr2_type),"String[]")!=0)){
        cur_node->type = _BOOL;
    } 
	else if (expr1_type ==_DOUBLE && expr2_type==_INT){
        cur_node->type = _BOOL;
    } 
	else if (expr1_type ==_INT && expr2_type==_DOUBLE){
        cur_node->type = _BOOL;
    } 

	else {
        cur_node->type = _BOOL;
        if(expr2_type==_NONE)printf("Line %d, col %d: Operator %s cannot be applied to types %s, none\n", cur_node->line, cur_node->col, s, type2str(expr1_type));
	else if(expr1_type==_NONE)printf("Line %d, col %d: Operator %s cannot be applied to types none, %s\n", cur_node->line, cur_node->col, s,  type2str(expr2_type));

	else printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n", cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
    }
}


void check_print(node*n,type_t expr_type){
	if(strcmp(n->name,"Length")==0){
		n->type=_INT;
	}
	expr_type=n->type;
	if((strcmp(type2str(expr_type),"void")==0)){
	printf("Line %d, col %d: Incompatible type void in System.out.print statement\n", n->line, n->col);
	}
	else if((strcmp(type2str(expr_type),"undef")==0)){
	printf("Line %d, col %d: Incompatible type undef in System.out.print statement\n", n->line, n->col);
	}	




	

}


void check_lshift_expr(node* cur_node, char* s, type_t expr1_type, type_t expr2_type){
    if (expr1_type == _INT && expr2_type == _INT){
		cur_node->type = _INT;
	}

    else {
        printf("Line %d, col %d: Operator %s cannot be applied to types %s, %s\n",
        cur_node->line, cur_node->col, s, type2str(expr1_type), type2str(expr2_type));
        cur_node->type = _UNDEF;
    }
}


void clean_sons(node* cur_node){
	cur_node->type=_NONE;
	if(cur_node->son){
		clean_sons(cur_node->son);
	}
	if(cur_node->next){
	        clean_sons(cur_node->next);
	}

}

void check_statement(table_t* table, node* cur_node){
	
    if (strcmp(cur_node->name, "Return") == 0){
        if (cur_node->son == NULL){
            check_return_statement(cur_node, table->first->type, _NONE);
        } else {
            check_return_statement(cur_node, table->first->type, cur_node->son->type);
        }
    } else if (strcmp(cur_node->name, "If") == 0){
        check_if_statement(cur_node, "if", cur_node->son->type);
    } else if (strcmp(cur_node->name, "While") == 0){
        check_while_statement(cur_node, "while", cur_node->son->type);
    } else if (strcmp(cur_node->name, "Assign") == 0){
        check_assign_statement(cur_node, cur_node->son->type, cur_node->son->next->type);
	
    } else if (strcmp(cur_node->name, "Call") == 0){
        get_id(cur_node->son->name);
	symbol_t* s =find_symbol_call(global, aux,true,cur_node);
	ambiguo=0;
	if(s==NULL){
		s =find_symbol_call_parecido(global, aux,true,cur_node);
	}
	if(ambiguo==0){
		check_call_statement(s, cur_node,table);
	}
	else{
		cur_node->type=_UNDEF;
		cur_node->son->type=_UNDEF;
	}
	
	
        
    } else if (strcmp(cur_node->name, "ParseArgs") == 0){
        check_parseargs_statement(cur_node, cur_node->son, cur_node->son->next);
    } else if (strcmp(cur_node->name, "Add") == 0){
        check_binary_expr(cur_node, "+", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Sub") == 0){
        check_binary_expr(cur_node, "-", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Mul") == 0){
        check_binary_expr(cur_node, "*", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Div") == 0){
        check_binary_expr(cur_node, "/", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Mod") == 0){
        check_binary_expr(cur_node, "%", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Plus") == 0){
        check_plus_minus_expr(cur_node, "+", cur_node->son->type);
    } else if (strcmp(cur_node->name, "Minus") == 0){
        check_plus_minus_expr(cur_node, "-", cur_node->son->type);
    } else if (strcmp(cur_node->name, "Not") == 0){
        check_not_expr(cur_node, cur_node->son->type);
	cur_node->type=_BOOL;
    } else if (strcmp(cur_node->name, "And") == 0){
        check_logical_expr(cur_node, "&&", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Or") == 0){
        check_logical_expr(cur_node, "||", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Xor") == 0){
        check_logical_expr(cur_node, "^", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Lshift") == 0){
        
        	check_lshift_expr(cur_node, "<<", cur_node->son->type, cur_node->son->next->type);
	cur_node->type=_NONE;
	cur_node->son->type=_NONE;
	cur_node->son->next->type=_NONE;

    } else if (strcmp(cur_node->name, "Rshift") == 0){
        check_logical_expr(cur_node, ">>", cur_node->son->type, cur_node->son->next->type);
	cur_node->type=_NONE;
	cur_node->son->type=_NONE;
	cur_node->son->next->type=_NONE;
    } else if (strcmp(cur_node->name, "Lt") == 0){
        check_comp_expr(cur_node, "<", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Gt") == 0){
        check_comp_expr(cur_node, ">", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Eq") == 0){
        check_comp_expr_ig(cur_node, "==", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Ne") == 0){
        check_comp_expr_ig(cur_node, "!=", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Le") == 0){
        check_comp_expr(cur_node, "<=", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Ge") == 0){
        check_comp_expr(cur_node, ">=", cur_node->son->type, cur_node->son->next->type);
    } else if (strcmp(cur_node->name, "Print") == 0){
        check_print(cur_node->son,cur_node->son->type);
    }
    else{
	char str_aux[1024];
	strcpy(str_aux,cur_node->name);
	str_aux[6]='\0';
	if(strcmp(str_aux,"StrLit")==0 ){
		cur_node->type=_STR;
	}
	
	
	}
}



void check_double(node* n){
	char auxint[1000];
	char numero[1000]="";
	double final=0;
	strcpy(auxint, n->name + 8);
    	auxint[strlen(auxint)-1] = '\0';
	for(int i=0;i<strlen(auxint);i++){
		char c[2];c[1]='\0';
		c[0]=auxint[i];
	    	if(strcmp(c,"_")!=0){
			if(strcmp(c,"E")!=0 && strcmp(c,"e")!=0){
				strcat(numero,c);
			}else{
				char e[1000]="";
				for(int j=i+1;j<strlen(auxint);j++){
					c[0]=auxint[j];
					strcat(e,c);
				}

				int p =atoi(e);
				
				final=strtod(numero,NULL);
				
				
				if(p<0){
					p *= -1;
					printf("%d\n",p);
					for(int f=0; f<p; f++){
						final /= final;
					}
				}else{
					for(int f=0; f<p; f++){
						final *= final;
					}
				}
				i=strlen(auxint);
			}
		}
	}

    	if (final > INT_MAX || final < INT_MIN){
     		printf("Line %d, col %d: Number %s out of bounds\n",n->line, n->col, auxint);
    	}
}




void check_int(node* n){
	char auxint[1000];
	char after[1000];
	char numero[1000]="";
	int final=0;
	strcpy(auxint, n->name + 7);
    	auxint[strlen(auxint)-1] = '\0';
	for(int i=0;i<strlen(auxint);i++){
		char c[2];c[1]='\0';
		c[0]=auxint[i];
	    	if(strcmp(c,"_")!=0){
			if(strcmp(c,"E")!=0 ||strcmp(c,"e")!=0){
				strcat(numero,c);
			}else{
				char e[1000]="";
				for(int j=i+1;j<strlen(auxint);j++){
					c[0]=auxint[j];
					strcat(e,c);
				}
				char f[1000]="";
				int p =atoi(e);
				
				if(p<0){
					p *= -1;
					for(i=0; i<p; i++){
						final /= final;
					}
				}else{
					for(i=0; i<p; i++){
						final *= final;
					}
				}
				sprintf(f,"%d",final);
				strcpy(numero,f);
				//printf("%d\n",numero);
				i=strlen(auxint);
			}
		}
	}
	
	sprintf(after,"%d",atoi(numero));

    	if (strcmp(numero,after)!=0 ){
     		printf("Line %d, col %d: Number %s out of bounds\n",n->line, n->col, auxint);
    	}
}




void check_type(table_t* table, node* cur_node,node* pai){


    if (starts_with("Id", cur_node->name)){
        get_id(cur_node->name);
		symbol_t* s = find_symbol(table, aux,false);
		symbol_t* s2 = find_symbol(table, aux,true);
		if (s != NULL){
		    cur_node->type = s->type;
		    s->used = true;
		}else if (s==NULL && s2 != NULL){
		    cur_node->type = s2->type;
		    s2->used = true;
		}  else {
			/*char vec[1000];
			if(strcmp(pai->name, "Call") == 0){
				strcpy(vec,aux);
				strcat(vec,"(");
				node* zx=cur_node->next;
				if(pai!=NULL && table!=NULL && zx!=NULL){
					check_type(table,zx,pai);
					strcat(vec,type2str(zx->type));
					zx=zx->next;
					while(zx!=NULL){
						strcat(vec,",");
						check_type(table,zx,pai);
						strcat(vec,type2str(zx->type));
						zx=zx->next;
					}		
					strcat(vec,")");
					strcpy(aux,vec);
				}
				
			}
			*/
		    if(strcmp(pai->name, "Call") != 0){
		    printf("Line %d, col %d: Cannot find symbol %s\n", cur_node->line, cur_node->col, aux);}
		    cur_node->type = _UNDEF;
		}
	
    } else if (starts_with("DecLit", cur_node->name)){
	check_int(cur_node);
	cur_node->type = _INT;
    } else if (starts_with("RealLit", cur_node->name)){
 	check_double(cur_node);
	cur_node->type = _DOUBLE;

    } else if (starts_with("BoolLit", cur_node->name)){
        cur_node->type = _BOOL;
    } 
}

void check_unused_symbols(table_t* table){
    symbol_t* s = table->first->next;
    while (s != NULL){
        if (!(s->used)){
            printf("Line %d, col %d: Symbol %s declared but never used\n", s->line, s->col, s->name);
        }
        s = s->next;
    }
}

symbol_t* new_vardecl(node* vardecl, bool param){
    get_id(vardecl->son->next->name);
symbol_t*symbol=new_var_symbol(aux, vardecl->son->name, param, vardecl->son->next->line, vardecl->son->next->col);
    return symbol;
}
