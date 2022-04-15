%{
    #include <stdio.h>
    #include <stdlib.h>
    int yylex(void);
    int yyerror(char *);

    
%}


%start P
%union {
    float num;
    char* id;
}
%token <num> NUMBER
%token <id> ID
%type <num> E T F
%type <id> S


%%
P:  L;
L:  L S '\n'
    |
    ;
S:  ID '=' E ';'    {printf("%s = %.2f\n", $1, $3); free($1);}
    ;         
E:  E '+' T         {$$ = $1 + $3;}
    | E '-' T       {$$ = $1 - $3;}
    | T             {$$ = $1;}
    ;
T:  T '*' F         {$$ = $1 * $3;}
    | T '/' F       {$$ = $1 / $3;}
    | F             {$$ = $1;}
    ;
F:  '(' E ')'       {$$ = $2;}
    | '-' F         {$$ = -$2;}
    | NUMBER        {$$ = $1;}
    ;
%%

int yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    return 0;
}

int main() {
    yyparse();
    return 0;
}