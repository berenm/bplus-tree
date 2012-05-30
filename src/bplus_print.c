#include "bplus_print.h"

#include "bplus_tree_private.h"
#include "bplus_foreach.h"

static int bplus_tree_print(BplusTree const* tree, char const* format)
{
    return 0;
}

static void bplus_node_debug(BplusNode* node, void* argument)
{
    FILE* file = (FILE*) argument;

    if (bplus_node_is_leaf(node))
    {
        fprintf(file, "  node_%p [label = \"{{<p> prev|%lu|size: %zu|<n> right}|{", node, node->key, node->size);
        for (size_t i = 0; i < BPLUS_TREE_ORDER; ++i)
        {
            if (i > 0)
                fprintf(file, "| ");

            if (i < node->size)
                fprintf(file, "{<k%zu> %lu|%p}", i, node->items[i].key, node->items[i].data);

        }
        fprintf(file, "}}\"];\n");

        if (node->leaf_link->left)
            fprintf(file, "  node_%p:p -> node_%p:n;\n", node, node->leaf_link->left);

        if (node->leaf_link->right)
            fprintf(file, "  node_%p:n -> node_%p:p;\n", node, node->leaf_link->right);

    }
    else
    {
        fprintf(file, "  node_%p [label = \"{%lu|size: %zu|{", node, node->key, node->size);
        for (size_t i = 0; i < BPLUS_TREE_ORDER; ++i)
        {
            if (i > 0)
                fprintf(file, "| ");

            if (i < node->size)
                fprintf(file, "<k%zu> %lu", i, node->items[i].key);

        }
        fprintf(file, "}}\"];\n");

        for (size_t i = 0; i < node->size; ++i)
            fprintf(file, "  node_%p:k%zu -> node_%p;\n", node, i, node->items[i].data);

    }
}

static void bplus_tree_debug(BplusTree const* tree, FILE* file)
{
    fprintf(file, "digraph tree {\n");
    fprintf(file, "  node [shape=record];\n");
    fprintf(file, "  tree [label = \"<f> first|<r> root|<l> last\"];\n");
    fprintf(file, "  tree:f -> node_%p;\n", tree->first);
    fprintf(file, "  tree:r -> node_%p;\n", tree->root);
    fprintf(file, "  tree:l -> node_%p;\n", tree->last);

    bplus_foreach_node_in_tree(tree, &bplus_node_debug, file);
    fprintf(file, "}\n");
}

static void bplus_tree_debug_to_file(BplusTree* tree)
{
    char buffer[128];

    snprintf(buffer, 128, "tree_%p.dot.tmp", tree);
    FILE* file = fopen(buffer, "w");
    bplus_tree_debug(tree, file);
    fclose(file);

    char buffer2[128];
    snprintf(buffer2, 128, "tree_%p.dot", tree);
    rename(buffer, buffer2);
}
