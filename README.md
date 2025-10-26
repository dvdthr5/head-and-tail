# C File Utilities: Head and Tail

This repository contains two simple command-line programs written in C that replicate the functionality of the Unix `head` and `tail` commands. Both were implemented using low-level Unix I/O (`open`, `read`, `lseek`, `close`) instead of the standard C `FILE*` library.

## Programs

### `head`
The `head` program prints the first *k* lines of a file to standard output.

**Usage:**
```bash
./head <k> <filename>
```
• <k> is the number of lines to print
• <filename> is the path to the file

### 'tail'
The 'tail' program prints the last k lines of a file to standard output.
It uses a circular buffer to minimize memory usage and supports both text and binary files.

**Usage:**
```bash
./tail <k> <filename>
```
• <k> is the number of lines to print
• <filename> is the path to the file

### Error Handling
Both programs handle invalid input gracefully:
	•	Prints invalid k if <k> is not a valid positive integer.
	•	Prints invalid file if the specified file cannot be opened.

### Notes
• Both Programs limit memory usage to under 10MB
• Both implemented using Unix I/O for performance and direct control over file operations
