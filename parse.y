%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int yylex(void);
    void yyerror(char *);
    class node { // The abstract base class for all nodes.
        protected :
        virtual node () {} ;
        public :
        virtual void print_tree(void) = O; /* Descend ~ree, print contents.*/ 
    };
    class non_leaf_node: public node{
        char operator;
        string symbol;
        float value;
        node ** children;
        
    }
%}


%start S
%union {
    float num;
    char* id;
}
%token <num> NUMBER
%token <id> ID
%type <num> E T F
%type <id> S


%%

S:  ID '=' E ';'    {printf("%s = %.2f\n", $1, $3); return 0;}
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
    | '-' F         {$$ = $2;}
    | NUMBER        {$$ = $1;}
    ;
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    exit(0);
}

int main() {
    yyparse();
    return 0;
}