#include <stdio.h>

int commit_create(const char *message);

int main() {
    commit_create("Initial commit");
    commit_create("Second commit");
    commit_create("Third commit");
    return 0;
}
