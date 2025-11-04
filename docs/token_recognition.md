# Token recognition
`Token recognition` = shell scanning the input and identifying chunks (tokens)
- it follows fixed POSIX rules:
- Words = sequences of non-special chars.
- Operators = `|`, `;`,`>`,`<`, `||`, `&&`, etc.
- Quoting changes how tokens end.
- `<<word` = special token type `io_here`

- two modes either ordinary token recognition and processing of here-documents
- The shell grammar defines token types like `word` `operator` `io_redirect`,etc-

When the shell parser sees something lie `<<EOF`, it marks that as an I/O redirection token for a here-document.
Then after the next newline, the shell treats the following lines as teh body of that here-documents, until it finds the delimiter like `EOF`

when it encounters 
```c
cat <<EOF
hello
EOF

# token recognition result
[
	WORD("cat"),
	IO_HERE("EOF"),
	NEWLINE,
	HERE_BODY("hello\n")
]
```
## Example walkthough
input:
```bash
cat << EOF | grep "foo" # comment
```

```bash
[
	("WORD", "cat"),
	("OPERATOR", "<<"),
	("WORD", "EOF"),
	("WORD", "grep"),
	("WORD", "foo")
]
```
notice that `<<` is recognized as an operator (technicall `io_hre`)
the `# comment` part is ignored after the `#`
Quotes `foo` becomes a signle **WORD** token

The Way I see it this pseudo code respond briefly on how the code should behave

# !POSIX compliance is way more nuanced than people think
The posix shell's lexical rules aren't just:
> "split  on whitespace and respect quotes"

They're context sensitive, meaning the interpretation of a character depends on:
- The current quoting state
- What came before (operator, redirect, etc..)
- Whether the parser expects an assignment or a command.
- and sometimes even on expansion rulse that happen after lexing.

So implementing that correctly requires :
1. Recursive parsing context
2. Lookhead into expansion phases
3. Grammar feedback into lexing decisions

A simple hand-rolled lexer like the one above can't do all that.

|           **POSIX rule**  |        **WHAT POSIX SAYS**|
|----------                 |---------------            |
|**quotes join tokens**     |1.1|
|**escape rules**           |1.2| 
|**line continuation**      |1.3|
|**here-doc delimiters**    |1.4|
|**operators**              |1.5|
|**Comments**               |1.6|
|**Reserved words**         |1.7|
|**I/O numbers**            |1.8|
|**Here-doc scheduling**	|1.9|

## 1.1 Quotes join tokens
## 1.2 Escape rules
## 1.3 Line Continutation
## 1.4 Here-Doc delimiters
## 1.5 Operators
## 1.6 Comments
## 1.7 Reserved Words
## 1.8 I/O numbers
## 1.9 Scheduling