#!/bin/sh
# Make repository hooks executable and ensure git uses .git/hooks
set -e

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
HOOKS_DIR="$REPO_ROOT/.git/hooks"

# Ensure hooks directory exists
if [ ! -d "$HOOKS_DIR" ]; then
	printf "ERROR: hooks dir not found: %s\n" "$HOOKS_DIR" >&2
	exit 1
fi

# Make the two hooks executable if they exist
for h in pre-commit pre-push.sh pre-push; do
	if [ -f "$HOOKS_DIR/$h" ]; then
		chmod +x "$HOOKS_DIR/$h"
		printf "chmod +x %s\n" "$HOOKS_DIR/$h"
	fi
done

# Configure git to use .git/hooks explicitly (optional)
git -C "$REPO_ROOT" config core.hooksPath "$HOOKS_DIR" || true

printf "Hooks installed. Now try a failing commit to verify.\n"
