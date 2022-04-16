%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "syntree.h"

    int yylex(void);
    int yyerror(char *);
    int flag = 1;
    symtab st;
%}


%start L
%union {
    float num;
    char *id;
    struct tree *s_tree;
}
%token <num> NUMBER
%token <id> ID
%type <s_tree> E T F S


%%
L:  L S '\n'        {delete_tree($2); init_tempStore(&st);}
    |
    ;
S:  ID '=' E ';'    {$$ = make_stmt(&st, "S", $1, $3); print_tree($$); print_symtab(&st);}
    ;         
E:  E '+' T         {$$ = make_binop(&st, "E", $1, '+', $3, &flag);}
    | E '-' T       {$$ = make_binop(&st, "E", $1, '-', $3, &flag);}
    | T             {$$ = make_unop(&st, "E", '.', $1);}
    ;
T:  T '*' F         {$$ = make_binop(&st, "T", $1, '*', $3, &flag);}
    | T '/' F       {$$ = make_binop(&st, "T", $1, '/', $3, &flag); if(flag == 0) yyerror("divide by zero");}
    | F             {$$ = make_unop(&st, "T", '.', $1);}
    ;
F:  '(' E ')'       {$$ = make_unop(&st, "F", 'b', $2);}
    | '-' F         {$$ = make_unop(&st, "F", '-', $2);}
    | NUMBER        {$$ = make_number("F", $1);}
    ;
%%

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

int main() {
    init_symtab(&st);
    symtabfptr = fopen ("symtab.txt", "w");
    if(symtabfptr == NULL) {
        printf("Error writing symbol table!");   
        exit(1);             
    }
    syntreefptr = fopen ("syntree.txt", "w");
    if(syntreefptr == NULL) {
        printf("Error writing syntax tree!");   
        exit(1);             
    }
    tacfptr = fopen ("tac.txt", "w");
    if(tacfptr == NULL) {
        printf("Error writing three address code!");   
        exit(1);             
    }
    yyparse();
    delete_idStore(&st);
    fclose(symtabfptr);
    fclose(syntreefptr);
    fclose(tacfptr);
    return 0;
}