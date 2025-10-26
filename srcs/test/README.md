# Test Driven Development

we Want a robust, automateed way to test our lexer or any part of the shell even though the shell is a live runtime program. To create this environment, we need to separate concerns and make our shell testable.

# Principles to follow
- Decouple lexer from global state & OS interactions, make it pure function:
input stiring -> token list (no forls, not termios, no global readline)
- lexer deterministic  given teh same input and environment (so tests are reliable)
- data driven by using json (reasons: easy to write, easy to write new cases, easy to parse in many languages)

# filter pattern
<name>/*
<name>/**
<name>/<name>
'*'
'**'
*feature
feature*
v[12].[0-9]+.[0-9]+
'*'
'*.jsx?'
'**.js'
docs/*
docs/**
docs/**/*.md
'**/dcs/**'
'**/README.md'
'**/*src/**'
'**/*-post.md'
'**/migrate-*.sql'
'*.md'
'!README.md'
'*.md'
'!README.md'
<name>/*
'README*'
<name>/*
<name>/*
<name>/*
<name>/*
<name>/*
<name>/*
<name>/*
