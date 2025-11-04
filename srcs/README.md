# Shell Build System

## Conditional Compilation

This shell uses compile-time feature flags to build only what you need.

### Build Targets

- `make` or `make all` - Full-featured build
- `make help-only` - Minimal build with only --help support
- `make debug-only` - Build with debug and help modes only
- `make minimal` - Core shell without any optional features

### Manual Configuration

You can also set flags manually:

```bash
make CFLAGS="-DFEATURE_HELP_ONLY" clean all
make CFLAGS="-DFEATURE_DEBUG -DFEATURE_HELP" clean all
```

### Usage

```bash
./shell              # Normal mode (no debug output)
./shell --help       # Show help and exit
./shell --debug      # Debug mode with lexer output
./shell --login      # Login shell mode
./shell --posix      # POSIX compliance mode
./shell --pretty-print  # Pretty-print parse tree
```

### Binary Size Comparison

- Full build: ~XXX KB
- help-only: ~YYY KB (smallest, quickest --help response)
- debug-only: ~ZZZ KB
- minimal: ~AAA KB (core only)
