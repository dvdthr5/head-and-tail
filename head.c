#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

int integerCheck(const char *k);

int main(int argc, char *argv[]) {
    const size_t buffer_size = 4096;
    const char *program_name = "head";

    if (argc != 3) {
        fprintf(stderr, "Usage: %s  <number-of-lines> <file-name>\n", program_name);
        return EXIT_FAILURE;
    }

    const char *filename = argv[2];
    const char *kValue = argv[1];

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "invalid file\n");
        return 1;
    }

    if (integerCheck(kValue) != 0) {
        fprintf(stderr, "invalid k\n");
        return 1;
    }
    const int maxLines = atoi(kValue);

    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "%s: unable to allocate %zu byte buffer\n", program_name, buffer_size);
        close(fd);
        return EXIT_FAILURE;
    }

    ssize_t bytes_read = 0;
    int lines = 0;
    bool breakPoint = false;
    while ((bytes_read = read(fd, buffer, buffer_size)) > 0 && lines < maxLines) {
        int beginning = 0;
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                int len = i - beginning + 1;
                write(STDOUT_FILENO, buffer + beginning, len);
                beginning = i + 1;
                lines++;
                if (lines == maxLines) {
                    breakPoint = true;
                    break;
                }
            }
        }
        if (breakPoint) {
            break;
        }
        if (!breakPoint && beginning < bytes_read) {
            write(STDOUT_FILENO, buffer + beginning, bytes_read - beginning);
        }
    }

    if (bytes_read == -1) {
        free(buffer);
        close(fd);
        return 0;
    }

    free(buffer);
    return 0;
}

int integerCheck(const char *k) {
    if (*k == '\0') {
        return 1;
    }
    if (*k == '+') {
        k++;
    }
    if (*k == '-') {
        return 1;
    }
    for (; *k; k++) {
        if (!isdigit(*k)) {
            return 1;
        }
    }
    return 0;
}
