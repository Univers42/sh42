/* Implementation with full t_hellish definition */
#include "minishell.h"
#include "private_var.h"

char *env_expand_n(t_hellish *state, char *key, int len)
{
    t_env *curr;

    if (!state || !key)
        return (NULL);
    if (ft_strncmp(key, "?", len) == 0 && len == 1)
        return (state->last_cmd_status_s);
    else if (ft_strncmp(key, "$", len) == 0 && state->pid && len == 1)
        return (state->pid);
    else if (len == 0)
        return ("$");
    curr = env_nget(&state->env, key, len);
    if (!curr || !curr->key)
        return (NULL);
    return (curr->value);
}
