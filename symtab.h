#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYM 100

FILE* symtabfptr;

typedef struct {
    char *name;
    float val;
} id_val;

typedef struct {
    struct {int count; float arr[MAX_SYM];} tempStore;
    struct {int count; id_val arr[MAX_SYM];} idStore;
} symtab;

int add_temp(symtab *s, float v) {
    int c = s->tempStore.count;
    if (c > MAX_SYM)
        return -1;
    s->tempStore.arr[c] = v;
    s->tempStore.count += 1;
    // printf("debug: t%d - %.2f added\n", c, v);
    return c;
}

int find_id (symtab *s, char *id) {
    int c = s->idStore.count;
    id_val *arr = s->idStore.arr;
    for (int i = 0; i < c; i++) {
        if (strcmp(arr[i].name, id) == 0)
            return i;
    }
    return -1;
}

int add_id (symtab *s, char *id, float v) {
    int idx = find_id(s, id);
    if (idx != -1) {
        s->idStore.arr[idx].val = v;
        // printf("debug: id%d - %s - %.2f updated\n", idx, id, v);
    } else {
        idx = s->idStore.count;
        if (idx > MAX_SYM)
            return -1;
        else {
            id_val newId = {.name = id, .val = v};
            s->idStore.arr[idx] = newId;
        }
        s->idStore.count += 1;
        // printf("debug: id%d - %s - %.2f added\n", idx, id, v);
    }
    return idx;
}

void init_tempStore (symtab *s) {
    s->tempStore.count = 0;
}

void delete_idStore (symtab *s) {
    int c = s->idStore.count;
    id_val *arr = s->idStore.arr;
    for (int i = 0; i < c; i++)
        free(arr[i].name);
    s->idStore.count = 0;
}

void init_symtab (symtab *s) {
    init_tempStore(s);
    delete_idStore(s);
}

void print_symtab (symtab *s) {
    fprintf(symtabfptr, "%-10s%10s\n", "Symbol", "Value");
    int c1 = s->idStore.count, c2 = s->tempStore.count;
    for (int i = 0; i < c1; i++) {
        fprintf(symtabfptr, "%-10s%10.2f\n", s->idStore.arr[i].name, s->idStore.arr[i].val);
    }
    for (int i = 0; i < c2; i++) {
        char t_name[3];
        sprintf(t_name, "t%d", i);
        fprintf(symtabfptr, "%-10s%10.2f\n", t_name, s->tempStore.arr[i]);
    }
    fprintf(symtabfptr, "\n\n");
}