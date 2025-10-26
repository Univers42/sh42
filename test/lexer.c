#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "json_parser.h"
# define JSON_PATH "lexer.json"

int main(int argc, char **argv)
{
	char			*json_path;
	t_json_array	*cases;
	t_token			*tokens;
	char			*line;
	int				fd;
	int				fail_count;
	char			*input;
	int				status;

	fail_count = 0;
	json_path = JSON_PATH;
	cases = load_json_array(json_path);
	while (tokens)
	{
		input = case->input;
		tokens = NULL;
		status = 0;
		tokens = lexer_tokenizer(input, &status);
		if (!matches_expected(tokens, case->expected))
		{
			printf("Fail %s\n input: %s\n", case->name, input);
			printf_tokens(tokens);
			fail_count++;
		}
		else
			printf("OK %s\n", case->name);
		free_tokens(tokens);
	}
	return (faile_count == 0 ? 0 : 1);
}