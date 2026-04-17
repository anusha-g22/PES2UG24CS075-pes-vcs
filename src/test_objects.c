#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int object_write(const char *type, const void *data, size_t size, char *out_hash);
void *object_read(const char *hash, char *type, size_t *size_out);

int main() {
    char hash1[65], hash2[65];
    char type[10];
    size_t size;

    const char *content = "Hello, World!\n";

    printf("=== TEST 1: Write Object ===\n");
    if (object_write("blob", content, strlen(content), hash1) == 0) {
        printf("PASS: Object written\nHash: %s\n\n", hash1);
    } else {
        printf("FAIL: Write failed\n");
        return 1;
    }

    printf("=== TEST 2: Read Object ===\n");
    char *data = object_read(hash1, type, &size);
    if (data && strcmp(data, content) == 0) {
        printf("PASS: Read successful\nType: %s\nSize: %zu\n\n", type, size);
    } else {
        printf("FAIL: Read failed\n");
        return 1;
    }
    free(data);

    printf("=== TEST 3: Deduplication ===\n");
    object_write("blob", content, strlen(content), hash2);
    if (strcmp(hash1, hash2) == 0) {
        printf("PASS: Same content → same hash\n\n");
    } else {
        printf("FAIL: Hash mismatch\n");
        return 1;
    }

    printf("🎉 ALL TESTS PASSED!\n");
    return 0;
}
