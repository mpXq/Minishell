#include "minishell.h"

int			allocate_shell(t_shell *shell, char **envp)
{
	int	i;

	shell->line_readed = ft_calloc(1, 1);
	shell->env_l = malloc(sizeof(*(shell->env_l)));
	if (!shell->env_l)
		return (printf("Malloc: Env failed\n"));
	shell->env_l->nb_elem = 0;
	i = -1;
	while (envp[++i])
		new_back_node(shell->env_l, envp[i]);

	return (0);
}