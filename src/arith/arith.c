#include "arith.h"
#include "shell.h"
#include "libft.h"
#include <stdlib.h>

char	*arith_expand(t_shell *state, const char *expr, int len)
{
	t_arith_lexer	lexer;
	t_arith_ctx		ctx;
	long			result;
	char			*str;

	arith_lexer_init(&lexer, expr, len);
	ctx.state = state;
	ctx.lexer = &lexer;
	ctx.error = 0;
	arith_lexer_next(&lexer);
	result = arith_eval_expr(&ctx);
	arith_lexer_free_token(&lexer.current);
	if (ctx.error || lexer.error)
	{
		/* Match bash error format */
		ft_eprintf("%s: syntax error in expression (error token is \"%.*s\")\n",
			state->context, len, expr);
		set_cmd_status(state, res_status(1));
		return (NULL);
	}
	str = ft_itoa((int)result);
	return (str);
}
