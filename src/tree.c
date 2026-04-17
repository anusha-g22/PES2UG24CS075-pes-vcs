#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100

// Structure for a tree entry
typedef struct {
    char mode[7];     // "100644" or "040000"
    char hash[65];
    char name[256];
} TreeEntry;

// Simple tree struct
typedef struct {
    TreeEntry entries[MAX_ENTRIES];
    int count;
} Tree;

// Function prototype from object.c
int object_write(const char *type, const void *data, size_t size, char *out_hash);

// Compare for sorting
int cmp_entries(const void *a, const void *b) {
    TreeEntry *ea = (TreeEntry *)a;
    TreeEntry *eb = (TreeEntry *)b;
    return strcmp(ea->name, eb->name);
}

// Serialize tree
void serialize_tree(Tree *tree, char **out_buf, size_t *out_size) {
    size_t size = 0;
    char *buffer = malloc(1024);

    for (int i = 0; i < tree->count; i++) {
        char line[512];
        int len = snprintf(line, sizeof(line), "%s %s %s\n",
                           tree->entries[i].mode,
                           tree->entries[i].hash,
                           tree->entries[i].name);

        memcpy(buffer + size, line, len);
        size += len;
    }

    *out_buf = buffer;
    *out_size = size;
}

// MAIN FUNCTION YOU NEED
int tree_from_index(char *out_hash) {
    // For now: dummy tree (we'll improve later in index phase)
    Tree tree;
    tree.count = 0;

    // Sort entries (even if empty)
    qsort(tree.entries, tree.count, sizeof(TreeEntry), cmp_entries);

    char *buf;
    size_t size;
    serialize_tree(&tree, &buf, &size);

    int result = object_write("tree", buf, size, out_hash);

    free(buf);
    return result;
}
