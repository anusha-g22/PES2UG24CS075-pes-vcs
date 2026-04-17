#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

// from other modules
int tree_from_index(char *out_hash);
int object_write(const char *type, const void *data, size_t size, char *out_hash);
void *object_read(const char *hash, char *type, size_t *size_out);

// read HEAD (parent)
void read_head(char *parent_hash) {
    FILE *f = fopen(".pes/refs/heads/main", "r");
    if (!f) {
        parent_hash[0] = '\0';
        return;
    }
    fscanf(f, "%64s", parent_hash);
    fclose(f);
}

// update HEAD
void update_head(const char *hash) {
    mkdir(".pes", 0755);
    mkdir(".pes/refs", 0755);
    mkdir(".pes/refs/heads", 0755);

    FILE *f = fopen(".pes/refs/heads/main", "w");
    if (!f) return;

    fprintf(f, "%s", hash);
    fclose(f);
}

// CREATE COMMIT
int commit_create(const char *message) {
    char tree_hash[65];
    tree_from_index(tree_hash);

    char parent[65];
    read_head(parent);

    char author[100] = "PES User <pes@localhost>";
    time_t now = time(NULL);

    char content[1024];

    if (strlen(parent) > 0) {
        snprintf(content, sizeof(content),
                 "tree %s\nparent %s\nauthor %s %ld\ncommitter %s %ld\n\n%s\n",
                 tree_hash, parent, author, now, author, now, message);
    } else {
        snprintf(content, sizeof(content),
                 "tree %s\nauthor %s %ld\ncommitter %s %ld\n\n%s\n",
                 tree_hash, author, now, author, now, message);
    }

    char commit_hash[65];
    object_write("commit", content, strlen(content), commit_hash);

    update_head(commit_hash);

    printf("Commit created: %s\n", commit_hash);

    return 0;
}

// LOG FUNCTION
void commit_log() {
    char current[65];

    FILE *f = fopen(".pes/refs/heads/main", "r");
    if (!f) {
        printf("No commits yet.\n");
        return;
    }

    fscanf(f, "%64s", current);
    fclose(f);

    while (strlen(current) > 0) {
        char type[10];
        size_t size;

        char *data = object_read(current, type, &size);
        if (!data) break;

        printf("commit %s\n", current);

        char parent[65] = "";
        char *parent_line = strstr(data, "parent ");
        if (parent_line) {
            sscanf(parent_line, "parent %64s", parent);
        }

        char *author_line = strstr(data, "author ");
        if (author_line) {
            char author[200];
            sscanf(author_line, "author %[^\n]", author);
            printf("Author: %s\n", author);
        }

        char *msg = strstr(data, "\n\n");
        if (msg) {
            printf("\n    %s\n", msg + 2);
        }

        printf("\n");

        strcpy(current, parent);
        free(data);
    }
}
