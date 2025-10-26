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
    const char *program_name = "tail";

    if (argc != 3) {
        fprintf(stderr, "Usage: %s  <number-of-lines> <file-name>\n", program_name);
        return EXIT_FAILURE;
    }

    const char *filename = argv[2];
    const char *kValue = argv[1];

    if (integerCheck(kValue) != 0) {
        fprintf(stderr, "invalid k\n");
        return 1;
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "invalid file\n");
        return 1;
    }

    const int kVal = atoi(kValue);

    char *buffer = malloc(buffer_size);
    if (buffer == NULL) {
        fprintf(stderr, "%s: unable to allocate %zu byte buffer\n", program_name, buffer_size);
        close(fd);
        return EXIT_FAILURE;
    }

    ssize_t bytes_read = 0;
    long byte_position = 0;
    int total_lines = 0;
    const int MAX_TRACKED_LINES = 100000;
    int track_lines = (kVal < MAX_TRACKED_LINES) ? kVal : MAX_TRACKED_LINES;
    long *offsets = malloc((track_lines + 1) * sizeof(long));
    char last_char = '\n';
    while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                offsets[total_lines % (track_lines + 1)] = byte_position + 1;
                total_lines++;
            }
            byte_position++;
            last_char = buffer[i];
        }
    }

    if (bytes_read == -1) {
        free(buffer);
        close(fd);
        return 0;
    }

    if (last_char != '\n' && total_lines > 0) {
        offsets[total_lines % (track_lines + 1)] = byte_position;
        total_lines++;
    }

    bytes_read = 0;
    long start_offset = 0;
    if (total_lines > kVal) {
        start_offset = offsets[(total_lines - kVal - 1) % (track_lines + 1)];
    } else {
        start_offset = 0;
    }

    if (total_lines == 0) {
        lseek(fd, 0, SEEK_SET);
        while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        free(offsets);
        free(buffer);
        close(fd);
        return 0;
    } else {
        lseek(fd, start_offset, SEEK_SET);
    }

    while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
        last_char = buffer[bytes_read - 1];
        memset(buffer, 0, bytes_read);
    }
    // if (last_char != '\n' && isprint(last_char)) {
    //     write(STDOUT_FILENO, "\n", 1);
    // }
    free(offsets);
    free(buffer);
    return 0;
}

int integerCheck(const char *k) {
    if (*k == '\0') {
        return 1;
    }
    if (*k == '+') {
        return 1;
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
