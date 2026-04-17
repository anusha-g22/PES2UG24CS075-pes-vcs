#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <unistd.h>

#define HASH_HEX_SIZE 65

void sha256_hex(const unsigned char *data, size_t len, char *out) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(data, len, hash);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out + (i * 2), "%02x", hash[i]);
    }
    out[64] = '\0';
}

int object_write(const char *type, const void *data, size_t size, char *out_hash) {
    char header[64];
    int header_len = snprintf(header, sizeof(header), "%s %zu", type, size) + 1;

    size_t total_size = header_len + size;
    unsigned char *buffer = malloc(total_size);
    if (!buffer) return -1;

    memcpy(buffer, header, header_len);
    memcpy(buffer + header_len, data, size);

    sha256_hex(buffer, total_size, out_hash);

    // Create directories
    mkdir(".pes", 0755);
    mkdir(".pes/objects", 0755);

    char dir[256];
    snprintf(dir, sizeof(dir), ".pes/objects/%.2s", out_hash);
    mkdir(dir, 0755);

    // Build path safely (NO warning version)
    char path[512];
    int written = snprintf(path, sizeof(path), "%s/%s", dir, out_hash + 2);

    if (written < 0 || written >= (int)sizeof(path)) {
        free(buffer);
        return -1; // prevent truncation
    }

    FILE *f = fopen(path, "wb");
    if (!f) {
        free(buffer);
        return -1;
    }

    fwrite(buffer, 1, total_size, f);
    fclose(f);

    free(buffer);
    return 0;
}

void *object_read(const char *hash, char *type, size_t *size_out) {
    char path[512];

    int written = snprintf(path, sizeof(path), ".pes/objects/%.2s/%s", hash, hash + 2);
    if (written < 0 || written >= (int)sizeof(path)) {
        return NULL;
    }

    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);

    unsigned char *buffer = malloc(file_size);
    if (!buffer) {
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, file_size, f);
    fclose(f);

    // Verify hash
    char computed_hash[HASH_HEX_SIZE];
    sha256_hex(buffer, file_size, computed_hash);

    if (strcmp(computed_hash, hash) != 0) {
        free(buffer);
        return NULL;
    }

    // Parse header
    char *space = strchr((char *)buffer, ' ');
    char *null_byte = memchr(buffer, '\0', file_size);

    if (!space || !null_byte) {
        free(buffer);
        return NULL;
    }

    strncpy(type, (char *)buffer, space - (char *)buffer);
    type[space - (char *)buffer] = '\0';

    *size_out = atoi(space + 1);

    void *data = malloc(*size_out);
    if (!data) {
        free(buffer);
        return NULL;
    }

    memcpy(data, null_byte + 1, *size_out);

    free(buffer);
    return data;
}
