#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum treetype {stmt_node, unop_node, binop_node, number_node};

typedef struct tree {
    enum treetype nodetype;
    union {
        struct {struct tree *expr;} stmt;
        struct {struct tree *child; char op;} un_op;
        struct {struct tree *left, *right; char op;} bin_op;
        float num_lexeme;
        char id_lexeme[100];
    } body;
    float val;
    char code[100];
} tree;

// typedef struct {
//     char name[100];
//     float val;
// } id_val;

// typedef struct {
//     struct {int count; float arr[100];} temp;
//     struct {int count; id_val arr[100];} id;
// } symtab;

tree *make_stmt (char *nt, char *id, tree *e) {
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= stmt_node;
    result->body.stmt.expr= e;
    sprintf(result->code, "[%s[id.lexval=%s][=][%s][;]]", nt, id, e->code);
    return result;
}

tree *make_binop (char *nt, tree *l, char o, tree *r, int *flag) {
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
    sprintf(result->code, "[%s.val=%.2f[%s][%c][%s]]", nt, result->val, l->code, o, r->code);
    return result;
}

tree *make_unop (char *nt, char o, tree *c) {
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= unop_node;
    result->body.bin_op.op= o;
    result->body.bin_op.right= c;
    if (o == '-') {
        result->val = -(c->val);
        sprintf(result->code, "[%s.val=%2f[%c][%s]]", nt, result->val, o, c->code);
    }
    else {
        result->val = c->val;
        if (o == '.')
            sprintf(result->code, "[%s.val=%2f[%s]]", nt, result->val, c->code);
        else
            sprintf(result->code, "[%s.val=%2f[(][%s][)]]", nt, result->val, c->code);
    }
    return result;
}

tree *make_number (char *nt, float n) {
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= number_node;
    result->val = n;
    sprintf(result->code, "[%s num.lexval=%.2f]", nt, n);
    return result;
}

void deleteTree (tree *t) {
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