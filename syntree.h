#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum treetype {stmt_node, unop_node, binop_node, number_node};

typedef struct tree {
    enum treetype nodetype;
    union {
        struct {char *id; struct tree *expr;} stmt;
        struct {struct tree *child; char op;} un_op;
        struct {struct tree *left, *right; char op;} bin_op;
        float num_lexeme;
    } body;
    float val;
    char code[1024];
    int idx;
} tree;
typedef struct hashnode 
{
	char* key;
	float value;
	struct hashnode *next;

}hashnode;

hashnode* map[30];
void initMap(){
    int x;
    for(x=0;x<30;x++) map[x]=NULL;
}
int hashFunc(char* k){
    int x;
    int sum=0;
    for(x=0;k[x]!='\0';x++) sum+=k[x];
    return sum%31;
}
void insert(char* symbol,float value){
    int key=hashFunc(symbol);
    hashnode *temp=(hashnode*) malloc(sizeof(hashnode));
    temp->key=symbol;
    temp->value=value;
    temp->next=NULL;
    if(map[key]==NULL) map[key]=temp;
    else{
        hashnode *cur = map[key];
        while(cur->next)
        {
            cur = cur->next;
        }

        cur->next = temp;
    }
    
}
// float getValue(char*symbol){

// }
// char codes[100][100];
// int num=0;
int tcounter=0;
float symtab[100];
// typedef struct {
//     char name[100];
//     float val;
// } id_val;

// typedef struct {
//     struct {int count; float arr[100];} temp;
//     struct {int count; id_val arr[100];} id;
// } symtab;

tree *make_stmt (char *nt, char *id, tree *e) {
    // printf("debug: make_stmt called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= stmt_node;
    result->body.stmt.expr= e;
    insert(id,e->val);
    printf("%s=t%d\n",id,e->idx);
    sprintf(result->code, "[%s[id.lexval=%s][=]%s[;]]", nt, id, e->code);
    return result;
}

tree *make_binop (char *nt, tree *l, char o, tree *r, int *flag) {
    // printf("debug: make_binop called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= binop_node;
    result->body.bin_op.left= l;
    result->body.bin_op.op= o;
    result->body.bin_op.right= r;
    switch (o) {
        case '+': result->val = l->val + r->val; break;
        case '-': result->val = l->val - r->val; break;
        case '*': result->val = l->val * r->val; break;
        case '/':
        if (r->val == 0)
            *flag = 0;
        else
            result->val = l->val / r->val; break;
    }

    printf("t%d=t%d %c t%d\n",tcounter,l->idx,o,r->idx);
    result->idx=tcounter;

    symtab[tcounter]=result->val;
    tcounter++;
    sprintf(result->code, "[%s.val=%.2f%s[%c]%s]", nt, result->val, l->code, o, r->code);
    return result;
}

tree *make_unop (char *nt, char o, tree *c) {
    // printf("debug: make_unop called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= unop_node;
    result->body.un_op.op = o;
    result->body.un_op.child = c;
    if (o == '-') {
        result->val = -(c->val);
            printf("t%d=-t%d\n",tcounter,c->idx);
            result->idx=tcounter;
            symtab[tcounter]=result->val;
            tcounter++;
        sprintf(result->code, "[%s.val=%.2f[%c]%s]", nt, result->val, o, c->code);
    }
    else {
        result->val = c->val;
        result->idx=c->idx;
        if (o == '.')
            sprintf(result->code, "[%s.val=%.2f%s]", nt, result->val, c->code);
        else
            sprintf(result->code, "[%s.val=%.2f[(]%s[)]]", nt, result->val, c->code);
    }
    return result;
}

tree *make_number (char *nt, float n) {
    // printf("debug: make_number called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= number_node;
    result->val = result->body.num_lexeme = n;
    printf("t%d=%.2f\n",tcounter,result->body.num_lexeme);
    result->idx=tcounter;
    symtab[tcounter]=result->val;
    tcounter++;
    sprintf(result->code, "[%s.val=%.2f num.lexval=%.2f]", nt, n, n);
    return result;
}

void deleteTree (tree *t) {
    // printf("debug: deleteTree called\n");
    if (t) {
        switch (t->nodetype) {
            case stmt_node:
                deleteTree (t->body.stmt.expr);
                break;
            case binop_node:
                deleteTree (t->body.bin_op.left);
                deleteTree (t->body.bin_op.right);
                break;
            case unop_node:
                deleteTree (t->body.un_op.child);
                break;
            case number_node:
                break;
        }
        free(t);
    }
}