#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BYTES_PER_LINE 12

static void make_identifier(const char *filename, char *out, size_t out_size)
{
    size_t i = 0;
    const char *p = filename;
    const char *slash = strrchr(filename, '/');
    const char *backslash = strrchr(filename, '\\');

    if (slash && slash + 1 > p) p = slash + 1;
    if (backslash && backslash + 1 > p) p = backslash + 1;

    while (*p && i + 1 < out_size) {
        char c = *p++;
        if (isalnum((unsigned char)c))
            out[i++] = c;
        else
            out[i++] = '_';
    }
    out[i] = '\0';

    if (i == 0 || isdigit((unsigned char)out[0])) {
        if (i + 1 < out_size) {
            memmove(out + 1, out, i + 1);
            out[0] = '_';
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "usage: blob2c <input-file>\n");
        return 1;
    }

    char name[1024];
    make_identifier(argv[1], name, sizeof(name));
    if (name[0] == '\0') {
        fprintf(stderr, "error: invalid variable name\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        fprintf(stderr, "error: cannot open '%s'\n", argv[1]);
        return 1;
    }

    printf("unsigned char %s[] = {\n", name);

    unsigned char buf[4096];
    size_t total = 0;
    int col = 0;

    for (;;) {
        size_t n = fread(buf, 1, sizeof(buf), f);
        if (n == 0) break;

        for (size_t i = 0; i < n; i++) {
            if (col == 0)
                printf("  ");
            printf("0x%02x", buf[i]);
            total++;
            col++;
            if (col < BYTES_PER_LINE)
                printf(", ");
            else
                printf(",");
            if (col == BYTES_PER_LINE) {
                printf("\n");
                col = 0;
            }
        }
    }

    if (col > 0)
        printf("\n");

    printf("};\n");
    printf("unsigned int %s_len = %zu;\n", name, total);

    fclose(f);
    return 0;
}
