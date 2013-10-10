#include "bplus_tree.h"
#include "bplus_tree_private.h"

#define __NUM__   (BPLUS_TREE_ORDER*2)

int main()
{
    BplusTree *tree;
    tree = bplus_tree_new();
    
    size_t i;
    for(i = 0; i < __NUM__; ++i)
    {
        bplus_tree_insert(tree, i, NULL);
    }
    
    bplus_tree_print(tree, "");
    bplus_tree_destroy(tree);
    return 0;
}
