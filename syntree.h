#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

FILE *syntreefptr;

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
    int temp_var_idx;
} tree;

tree *make_stmt (symtab *s, char *nt, char *id, tree *e) {
    // printf("debug: make_stmt called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= stmt_node;
    result->body.stmt.id = id;
    add_id(s, id, e->val);
    result->body.stmt.expr= e;
    sprintf(result->code, "[%s[id.lexval=%s][=]%s[;]]", nt, id, e->code);
    return result;
}

tree *make_binop (symtab *s, char *nt, tree *l, char o, tree *r, int *flag) {
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
    result->temp_var_idx = add_temp(s, result->val);
    sprintf(result->code, "[%s.val=%.2f%s[%c]%s]", nt, result->val, l->code, o, r->code);
    return result;
}

tree *make_unop (symtab *s, char *nt, char o, tree *c) {
    // printf("debug: make_unop called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= unop_node;
    result->body.un_op.op = o;
    result->body.un_op.child = c;
    if (o == '-') {
        result->val = -(c->val);
        result->temp_var_idx = add_temp(s, result->val);
        sprintf(result->code, "[%s.val=%.2f[%c]%s]", nt, result->val, o, c->code);
    }
    else {
        result->val = c->val;
        result->temp_var_idx = c->temp_var_idx;
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
    result->temp_var_idx = -1;
    sprintf(result->code, "[%s.val=%.2f num.lexval=%.2f]", nt, n, n);
    return result;
}

void delete_tree (tree *t) {
    // printf("debug: delete_tree called\n");
    if (t) {
        switch (t->nodetype) {
            case stmt_node:
                delete_tree (t->body.stmt.expr);
                break;
            case binop_node:
                delete_tree (t->body.bin_op.left);
                delete_tree (t->body.bin_op.right);
                break;
            case unop_node:
                delete_tree (t->body.un_op.child);
                break;
            case number_node:
                break;
        }
        free(t);
    }
}

void print_tree (tree *t) {
    fprintf(syntreefptr, "%s\n", t->code);
}