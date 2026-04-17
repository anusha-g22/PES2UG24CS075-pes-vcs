#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_ENTRIES 100

typedef struct {
    char mode[7];
    char hash[65];
    long mtime;
    size_t size;
    char path[256];
} IndexEntry;

typedef struct {
    IndexEntry entries[MAX_ENTRIES];
    int count;
} Index;

// from object.c
int object_write(const char *type, const void *data, size_t size, char *out_hash);

// Load index
int index_load(Index *index) {
    FILE *f = fopen(".pes/index", "r");

    index->count = 0;

    if (!f) return 0; // no index yet

    while (fscanf(f, "%s %s %ld %zu %s",
                  index->entries[index->count].mode,
                  index->entries[index->count].hash,
                  &index->entries[index->count].mtime,
                  &index->entries[index->count].size,
                  index->entries[index->count].path) == 5) {
        index->count++;
    }

    fclose(f);
    return 0;
}

// Save index
int index_save(Index *index) {
    FILE *f = fopen(".pes/index", "w");
    if (!f) return -1;

    for (int i = 0; i < index->count; i++) {
        fprintf(f, "%s %s %ld %zu %s\n",
                index->entries[i].mode,
                index->entries[i].hash,
                index->entries[i].mtime,
                index->entries[i].size,
                index->entries[i].path);
    }

    fclose(f);
    return 0;
}

// Add file
int index_add(Index *index, const char *filepath) {
    FILE *f = fopen(filepath, "rb");
    if (!f) {
        printf("File not found: %s\n", filepath);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    char *data = malloc(size);
    fread(data, 1, size, f);
    fclose(f);

    char hash[65];
    object_write("blob", data, size, hash);

    struct stat st;
    stat(filepath, &st);

    IndexEntry *e = &index->entries[index->count++];

    strcpy(e->mode, "100644");
    strcpy(e->hash, hash);
    e->mtime = st.st_mtime;
    e->size = size;
    strcpy(e->path, filepath);

    free(data);
    return 0;
}
