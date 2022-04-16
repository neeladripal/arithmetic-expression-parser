%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    int yylex(void);
    void yyerror(char *);
    enum treetype {operator_node, number_node, variable_node};
    typedef struct tree {
    enum treetype nodetype;
    union {
            struct {struct tree *left, *right; char operator;} an_operator;
            float num;
            char* id;

        } body;
    } tree;
static tree *make_operator (tree *l, char o, tree *r) {
   tree *result= (tree*) malloc (sizeof(tree));
   result->nodetype= operator_node;
   result->body.an_operator.left= l;
   result->body.an_operator.operator= o;
   result->body.an_operator.right= r;
   return result;
}
 static tree *make_number (float n) {
   tree *result= (tree*) malloc (sizeof(tree));
   result->nodetype= number_node;
   result->body.num= n;
   return result;
}
 static tree *make_variable (char* v) {
    //  printf("yy\n");
   tree *result= (tree*) malloc (sizeof(tree));
   if(!result) {printf("Malloc error\n"); return NULL;}
   result->nodetype= variable_node;
   result->body.id=v;
   return result;
 }
 static void printtree (tree *t, int level) {
 #define step 4
   if (t)
     switch (t->nodetype)
     {
       case operator_node:
        printtree (t->body.an_operator.right, level+step);
        printf ("%*c%c\n", level, ' ', t->body.an_operator.operator);
        printtree (t->body.an_operator.left, level+step);
        break;
       case number_node:
        printf ("%*c%f\n", level, ' ', t->body.num);
        break;
       case variable_node:
        printf ("%*c%s\n", level, ' ', t->body.id);
     }
 }
%}


%start S
%union {
    float num;
    char* id;
    struct tree* treenode;
}
%token <num> NUMBER
%token <id> ID
%type <treenode> S E T F



%%

S:  ID '=' E ';'    { 

            printf("dd\n");
            char *id1=strdup($1);
            // strcpy(id1,$1);
            tree * temp=make_variable(id1);

            $$ = make_operator(temp,'=',$3);  ; printtree($$,1) ;    free($1); return 0;}
    ;         
E:  E '+' T         {$$ = make_operator($1,'+',$3);}
    | E '-' T       {$$ = make_operator($1,'-',$3);}
    | T             {$$ = $1;}
    ;
T:  T '*' F         {$$ = make_operator($1,'*',$3);}
    | T '/' F       {$$ = make_operator($1,'/',$3);}
    | F             {$$ = $1;}
    ;
F:  '(' E ')'       {$$ = $2;}
    
    | NUMBER        {$$ = make_number($1);}
    ;
%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
    exit(0);
}

int main() {
    // printf("%s = %.2f\n", $1, $3);
    yydebug=1;
    yyparse();
    return 0;
}