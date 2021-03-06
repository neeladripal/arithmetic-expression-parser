#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab1.h"

FILE *syntreefptr, *tacfptr;

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
    char tstr[1024];
    int temp_var_idx;
} tree;

void tac_stmt (char *id, tree *e) {
    if (e->temp_var_idx == -1)
        fprintf(tacfptr, "%s = %.2f\n", id, e->val);
    else
        fprintf(tacfptr, "%s = t%d\n", id, e->temp_var_idx);
}

tree *make_stmt (symtab *s, char *nt, char *id, tree *e, int l) {
    // printf("debug: make_stmt called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= stmt_node;
    result->body.stmt.id = id;
    add_id(s, id, e->val, l);
    result->body.stmt.expr= e;
    sprintf(result->tstr, "[%s[id.lexeme=%s][=]%s[;]]", nt, id, e->tstr);
    tac_stmt(id, e);
    return result;
}

void tac_binop (tree *result, char o, tree *l, tree *r) {
    int lti = l->temp_var_idx, rti = r->temp_var_idx;
    if (lti != -1 && rti != -1)
        fprintf(tacfptr, "t%d = t%d %c t%d\n", result->temp_var_idx, lti, o, rti);
    else if (lti != -1)
        fprintf(tacfptr, "t%d = t%d %c %.2f\n", result->temp_var_idx, lti, o, r->val);
    else if (rti != -1)
        fprintf(tacfptr, "t%d = %.2f %c t%d\n", result->temp_var_idx, l->val, o, rti);
    else
        fprintf(tacfptr, "t%d = %.2f %c %.2f\n", result->temp_var_idx, l->val, o, r->val);
}

void tac_unop (tree *result, char o, tree *c) {
    fprintf(tacfptr, "t%d = %c %.2f\n", result->temp_var_idx, o, c->val);
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
    sprintf(result->tstr, "[%s.val=%.2f%s[%c]%s]", nt, result->val, l->tstr, o, r->tstr);
    tac_binop(result, o, l, r);
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
        sprintf(result->tstr, "[%s.val=%.2f[%c]%s]", nt, result->val, o, c->tstr);
        tac_unop(result, o, c);
    }
    else {
        result->val = c->val;
        result->temp_var_idx = c->temp_var_idx;
        if (o == '.')
            sprintf(result->tstr, "[%s.val=%.2f%s]", nt, result->val, c->tstr);
        else
            sprintf(result->tstr, "[%s.val=%.2f[(]%s[)]]", nt, result->val, c->tstr);
    }
    return result;
}

tree *make_number (char *nt, float n) {
    // printf("debug: make_number called\n");
    tree *result= (tree*) malloc (sizeof(tree));
    result->nodetype= number_node;
    result->val = result->body.num_lexeme = n;
    result->temp_var_idx = -1;
    sprintf(result->tstr, "[%s.val=%.2f num.lexeme=%.2f]", nt, n, n);
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
    fprintf(syntreefptr, "%s\n", t->tstr);
}