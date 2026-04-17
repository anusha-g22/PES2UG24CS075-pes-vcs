#include <stdio.h>

typedef struct {
    char mode[7];
    char hash[65];
    long mtime;
    size_t size;
    char path[256];
} IndexEntry;

typedef struct {
    IndexEntry entries[100];
    int count;
} Index;

int index_load(Index *index);
int index_save(Index *index);
int index_add(Index *index, const char *filepath);

int main() {
    Index idx;

    index_load(&idx);

    index_add(&idx, "file1.txt");

    index_save(&idx);

    printf("File staged successfully!\n");
    return 0;
}
