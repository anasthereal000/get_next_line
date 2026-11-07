_This project has been created as part of the 42 curriculum by **anboujel**_

# get_next_line

## Description

**get_next_line :** is a C function that reads a file descriptor and returns `one line at a time`, each time it is called. The returned line includes the terminating `\n` (except possibly for the last line of a file, if it does not end with one). When there is nothing left to read, or if an error occurs, it returns `NULL`.

```c
char *get_next_line(int fd);
```

**Goal of the project:** learn how to work with file descriptors, the `read()` system call, **static variables**, and **dynamic memory management** (`malloc` / `free`), while avoiding leaks and undefined behavior. The function must work with any `BUFFER_SIZE` (defined at compile time), with files, standard input, and redirections.

### Files

| File | Role |
|------|------|
| `get_next_line.c` | Main logic: `get_next_line`, `read_buffer`, `get_the_line`, `save_more` |
| `get_next_line_utils.c` | Helpers: `ft_strlen`, `ft_strjoin`, `check_newline`, `len_befor_new_line` |
| `get_next_line.h` | Prototypes, includes, and the default `BUFFER_SIZE` |
| `get_next_line_bonus.c` | Bonus: same logic, one saved string per file descriptor |
| `get_next_line_utils_bonus.c` | Bonus helpers |
| `get_next_line_bonus.h` | Bonus header |

### Allowed functions

`read`, `malloc`, `free`.

## Instructions

### Compilation

The buffer size is set at compile time with the `-D BUFFER_SIZE=n` flag. If it is not given, the header defines a default value.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Bonus (multiple file descriptors at the same time):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

### Usage example

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("text", O_RDONLY);
    if (fd < 0)
        return (1);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

Run it:

```bash
./gnl
```

Check for memory errors and leaks:

```bash
cc -Wall -Wextra -Werror -g -fsanitize=address -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

Try several buffer sizes, for example `1`, `3`, `42`, `9999`, and `10000000`.

### Notes

- The caller must `free()` every line returned by `get_next_line`.
- The function returns `NULL` when the end of the file is reached, or when `read()` fails.
- The bonus version supports reading from several file descriptors alternately, without losing the reading position of each one.

## Algorithm

### The problem

`read()` returns chunks of `BUFFER_SIZE` bytes. A chunk does not match line boundaries: it may contain half a line, exactly one line, or several lines. Bytes that were read but do not belong to the current line must not be lost, because the next call needs them.

### The solution: a static "stash"

A `static char *stokage` keeps the bytes that were read but not yet returned. Static variables keep their value between calls, so the stash works as the function's memory.

Each call to `get_next_line(fd)` does three steps:

1. **`read_buffer`: fill the stash.**
   Allocate a temporary buffer of `BUFFER_SIZE + 1` bytes. While the stash does not contain a `'\n'`, call `read()`. After each successful read, add a `'\0'` and append the buffer to the stash with `ft_strjoin` (which builds a new string; the old stash is freed). Stop when a newline appears in the stash, when `read()` returns `0` (end of file), or when it returns `-1` (error, everything is freed and `NULL` is returned).

2. **`get_the_line`: extract the line.**
   Measure the length up to and including the first `'\n'`, allocate that length plus 1 for `'\0'`, and copy the characters. If the stash is `NULL` or empty, there is nothing to return, so the result is `NULL`. This function only reads the stash; it does not modify it.

3. **`save_more`: keep the rest.**
   Allocate a new string containing everything **after** the first `'\n'`, copy it, and free the old stash. If there is no `'\n'` (the last line was just returned), free the stash and return `NULL`, which resets the memory for the next call.

### Example

File content: `"ab\ncd\nef"` with `BUFFER_SIZE = 3`

| Call | Reads | Stash before extraction | Returned line | Stash after |
|------|-------|-------------------------|---------------|-------------|
| 1 | `"ab\n"` | `"ab\n"` | `"ab\n"` | `""` |
| 2 | `"cd\n"` | `"cd\n"` | `"cd\n"` | `""` |
| 3 | `"ef"` then EOF | `"ef"` | `"ef"` | `NULL` |
| 4 | EOF | `NULL` | `NULL` | `NULL` |

### Why this algorithm

- **Static stash:** it is the simplest way to keep leftover data between calls without global variables (which are forbidden) and without re-reading the file.
- **Reading only until a newline appears:** the function never reads more of the file than necessary, so it works with large files and with standard input.
- **Dynamic strings (`ft_strjoin`):** the length of a line is unknown in advance and can be larger than `BUFFER_SIZE`, so the stash must be able to grow. Each join allocates exactly the required size.
- **Three separate functions:** reading, extracting, and trimming each have one job. This keeps each function short (Norm compliant) and easy to test and free correctly.
- **Bonus with `stoke[1024]`:** an array of stashes indexed by file descriptor gives each descriptor its own memory, so several files can be read alternately.

### Complexity and trade-offs

- Reading a file of `N` bytes takes `O(N)` for the copies when lines are short.
- For a very long line with a very small `BUFFER_SIZE`, repeated `ft_strjoin` copies the stash again each time, which costs more (up to `O(L² / BUFFER_SIZE)` for a line of length `L`). This is an accepted trade-off for simplicity and safety. Larger buffer sizes reduce the number of joins.
- Memory used at any time is the current stash plus one temporary buffer of `BUFFER_SIZE + 1` bytes.

## Resources

### References

- `man 2 read`: the `read()` system call
- `man 3 malloc` / `man 3 free`: dynamic memory allocation
- [GNU C Library manual: low-level input/output](https://www.gnu.org/software/libc/manual/html_node/Low_002dLevel-I_002fO.html)
- [Static variables in C](https://en.wikipedia.org/wiki/Static_variable)
- [File descriptors](https://en.wikipedia.org/wiki/File_descriptor)
- [AddressSanitizer documentation](https://github.com/google/sanitizers/wiki/AddressSanitizer)
- [Valgrind](https://valgrind.org/docs/manual/quick-start.html)
    > used to detect leaks and invalid memory access
