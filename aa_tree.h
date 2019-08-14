#include<stdint.h>


typedef unsigned long long int aatree_val;

typedef struct aatree_node_{
	int32_t level;
	aatree_val val;
	struct aatree_node_ *left;
	struct aatree_node_ *right;
} aatree_node;

static aatree_node nil = {0, 0, &nil, &nil};

struct del_info{
	aatree_val val;
	aatree_node *last;
	aatree_node *del;
};

aatree_node *aa_tree_insert(aatree_val x, aatree_node *tree, int32_t *flag);
aatree_node *aa_tree_delete(aatree_val x, aatree_node *tree, int32_t *flag);
int32_t aa_tree_head(aatree_node *tree, aatree_val *val);
void aa_tree_free(aatree_node *tree);
void aa_tree_print(aatree_node *tree, FILE *fp);
void aa_tree2vec(aatree_node *tree, aatree_val *dst);
int32_t aa_tree_search(aatree_node *tree, aatree_val x);
