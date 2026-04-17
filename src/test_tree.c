#include <stdio.h>

int tree_from_index(char *out_hash);

int main() {
    char hash[65];

    if (tree_from_index(hash) == 0) {
        printf("PASS: Tree created\nHash: %s\n", hash);
    } else {
        printf("FAIL: Tree creation failed\n");
    }

    return 0;
}
