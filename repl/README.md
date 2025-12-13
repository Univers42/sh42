# Readline Library

This is a custom readline implementation designed to provide a flexible, buffered input handling system for command-line applications. It wraps around GNU Readline to offer enhanced features like input buffering, history management, and customizable prompts, making it suitable for REPLs.

## Purpose

### Buffering Readline Input

Buffering allows the library to handle input incrementally, especially useful in scenarios where input might span multiple lines (e.g., due to backslash continuations or incomplete commands). Instead of processing input line-by-line immediately, the buffer accumulates data until a complete logical unit (like a newline or end-of-input) is detected. This enables:

- Handling multi-line inputs seamlessly.
- Supporting features like command continuation (e.g., when a line ends with a backslash).
- Efficient parsing in contexts where input might be piped or read from files.

### Wrapping Readline Logic

The library abstracts and extends GNU Readline's functionality to provide:

- Consistent behavior across different input methods (TTY, stdin, files).
- Signal handling (e.g., Ctrl+C, Ctrl+D).
- Integration with custom prompts and status tracking.
- Error recovery and status propagation.
  This wrapping makes the readline logic more robust and easier to integrate into larger applications without directly dealing with Readline's complexities.

### History Management

Integrated history support allows commands to be remembered, searched, and persisted across sessions. It includes:

- Loading and saving history to a file (typically `~/.history`).
- Encoding/decoding commands to handle special characters.
- Automatic addition of worthy commands to history.

## Features

- **Buffered Input Handling**: Accumulates input in a dynamic string buffer for flexible processing.
- **Multi-Mode Input**: Supports TTY (interactive), stdin (non-TTY), and file-based input.
- **Customizable Prompts**: Dynamic prompt generation based on status, context, and VCS info.
- **History Persistence**: Saves and loads command history with encoding for special characters.
- **Signal Integration**: Handles interrupts (e.g., SIGINT) and propagates status.
- **Context Tracking**: Updates context strings for error reporting or debugging.
- **VCS Integration**: Displays Git branch and dirty status in prompts.
- **Performance Timing**: Includes execution time display in prompts.

## Usage

1. **Include Headers**: Include `ft_readline.h` and related headers in your project.
2. **Initialize Structures**:
   - Use `buff_readline_init()` to set up the `t_rl` structure.
   - Initialize history with `init_history()`.
3. **Read Input**:
   - Call `xreadline()` or higher-level functions like `get_more_tokens()` to read and process input.
   - Handle returns: 0 (EOF), 2 (interrupt), 4 (success).
4. **Manage History**:
   - Use `manage_history()` after processing a command.
   - Free history with `free_hist()`.
5. **Customize Prompts**:
   - Use `prompt_normal()` or `prompt_more_input()` for prompt strings.
6. **Cleanup**: Free allocated resources, especially dynamic strings and history.

### Example

```c
#include "ft_readline.h"

int main() {
    t_rl rl;
    t_hist hist;
    t_dyn_str input;
    t_status status = {0, false};
    char *status_str = NULL;
    int input_method = INP_READLINE;
    char *context = NULL;
    char *base_context = "shell";

    buff_readline_init(&rl);
    init_history(&hist, /* env vector */);
    dyn_str_init(&input);

    while (true) {
        char *prompt = prompt_normal(&status, &status_str).buff;
        int ret = xreadline(&rl, &input, prompt, &status, &status_str, &input_method, &context, &base_context);
        free(prompt);
        if (ret == 0) break; // EOF
        if (ret == 2) continue; // Interrupt
        // Process input...
        manage_history(&hist, &rl);
    }

    free_hist(&hist);
    free(status_str);
    free(context);
    return 0;
}
```

## API Overview

- **t_rl**: Main readline structure for buffering and state.
- **t_hist**: History management structure.
- **xreadline()**: Core function to read a line with buffering.
- **get_more_tokens()**: Tokenizes input, handling continuations.
- **prompt_normal() / prompt_more_input()**: Generate prompts.
- **init_history() / manage_history() / free_hist()**: History operations.
- **set_cmd_status()**: Update command status.

## Dependencies

- GNU Readline library.
- Custom libft (for utilities like dynamic strings, vectors).
- POSIX-compliant system for signals, file I/O, and process management.

## Notes

- Ensure locale is set for wide character support (via `ensure_locale()`).
- History file path is derived from `$HOME/.history`.
- This implementation is thread-unsafe; use in single-threaded contexts.
