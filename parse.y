%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "syntree.h"
    int yylex(void);
    int yyerror(char *);
    FILE *fptr;
    int flag = 1;
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
L:  L S '\n'        {deleteTree($2);}
    |
    ;
S:  ID '=' E ';'    {$$ = make_stmt("S", $1, $3); free($1); fprintf(fptr, "%s\n", $$->code);}
    ;         
E:  E '+' T         {$$ = make_binop("E", $1, '+', $3, &flag);}
    | E '-' T       {$$ = make_binop("E", $1, '-', $3, &flag);}
    | T             {$$ = make_unop("E", '.', $1);}
    ;
T:  T '*' F         {$$ = make_binop("T", $1, '*', $3, &flag);}
    | T '/' F       {$$ = make_binop("T", $1, '/', $3, &flag); if(flag == 0) yyerror("divide by zero");}
    | F             {$$ = make_unop("T", '.', $1);}
    ;
F:  '(' E ')'       {$$ = make_unop("F", 'b', $2);}
    | '-' F         {$$ = make_unop("F", '-', $2);}
    | NUMBER        {$$ = make_number("F", $1);}
    ;
%%

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

int main() {
    fptr = fopen ("output.txt", "w");
    if(fptr == NULL) {
        printf("Error!");   
        exit(1);             
    }
    yyparse();
    fclose(fptr);
    return 0;
}