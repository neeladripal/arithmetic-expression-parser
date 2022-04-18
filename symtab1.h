#include <stdio.h>
#include <stdlib.h>
#include<string.h>


#define MAX_SYM 100

FILE* symtabfptr;

struct Node
{
	char* key;
    float value;
    int lno;
	struct Node* left;
	struct Node* right;
};

//struct Node* ROOT;

typedef struct{
    struct {int count; float arr[MAX_SYM];} tempStore;
    struct Node* root;
}symtab;

struct Node* NewNode(char* key, float value, int lno)
{
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->left = NULL;
	temp->key = key;
    temp->value = value;
	temp->right = NULL;
    temp->lno = lno;

	return temp;
};

struct Node* insert(struct Node* root,char* key, float value, int lno)
{
	if(root==NULL)
		root =  NewNode(key, value,lno);

    int cmp = strcmp(key, root->key);
    if (cmp < 0) 
        root->left = insert(root->left,key,value,lno);
	else if(cmp > 0)
		root->right = insert(root->right,key,value,lno);
	
	else {
        root->value = value;
        root->lno = lno;
    }

	return root;

}


struct Node* get(symtab* s, char* id) {
    struct Node* x = s->root;
    while (x != NULL) {
        int cmp = strcmp(id, x->key);
        if (cmp < 0) x = x->left;
        else if (cmp > 0) x = x->right;
        else return x;
    }

    return NULL; //key not found
}

struct Node* FindMin(struct Node* root)
{
	while(root->left!=NULL)
		root = root->left;

	return root;
}

struct Node* Delete(struct Node* root,char* id)
{

    int cmp = strcmp(id, root->key);

	if(root==NULL)
		return root;

	else if(cmp < 0)
		root->left = Delete(root->left,id);

	else if(cmp > 0)
		root->right = Delete(root->right,id);

	else
	{
		if(root->left==NULL && root->right==NULL)
		{
			free(root);
			root = NULL;
		}

		else if(root->left!=NULL && root->right==NULL)
		{
			struct Node* temp = root->left;
			free(root);
			root = temp;
		}
		else if(root->right!=NULL&& root->left==NULL)
		{
			struct Node* temp = root->right;
			free(root);
			root = temp;
		}

		else
		{
			struct Node* temp = FindMin(root->right);
			root->key = temp->key;
            root->value = temp->value;
            root->lno = temp->lno;
			root->right = Delete(root->right,temp->key); 
		}

	}
	return root;
}

int add_temp(symtab *s, float v) {
    int c = s->tempStore.count;
    if (c > MAX_SYM)
        return -1;
    s->tempStore.arr[c] = v;
    s->tempStore.count += 1;
    // printf("debug: t%d - %.2f added\n", c, v);
    return c;
}

struct Node* add_id(symtab* s, char* id, float val, int lno) {
    return s->root = insert(s->root, id, val, lno);
}

void init_tempStore (symtab *s) {
    s->tempStore.count = 0;
}

//deletes symtab
void delete_idList(struct Node* root) {
    if (root == NULL) return;
    delete_idList(root->left);
    delete_idList(root->right);

    free(root);
}

void delete_idStore(symtab* s) {
    delete_idList(s->root);
    s->root = NULL;
}

void init_symtab (symtab *s) {
    init_tempStore(s);
    delete_idStore(s);
}

void print_idList(struct Node* root) {
    if (root == NULL) return;
    print_idList(root->left);
    print_idList(root->right);

    fprintf(symtabfptr, "%-10s%10.2f%10d\n", root->key, root->value,root->lno);
}

void print_symtab (symtab *s) {
    fprintf(symtabfptr, "%-10s%10s%10s\n", "Symbol", "Value", "Line No.");
    print_idList(s->root);
    for (int i = 0; i < s->tempStore.count; i++) {
        char t_name[3];
        sprintf(t_name, "t%d", i);
        fprintf(symtabfptr, "%-10s%10.2f\n", t_name, s->tempStore.arr[i]);
    }
    fprintf(symtabfptr, "\n\n");
}
