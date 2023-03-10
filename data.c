/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleleux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 16:49:27 by eleleux           #+#    #+#             */
/*   Updated: 2023/02/20 19:19:37 by eleleux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	allocate_shell(t_shell *shell, char **envp)
{
	int	i;

	shell->line_readed = ft_calloc(1, 1);
	shell->env_l = malloc(sizeof(*(shell->env_l)));
	if (!shell->env_l)
		return (printf("Malloc: Env failed\n"));
	shell->user_command = malloc(sizeof(*(shell->user_command)));
	if (!shell->user_command)
		return (printf("Malloc: UserCommand failed\n"));
	shell->env_l->nb_elem = 0;
	i = -1;
	while (envp[++i])
		new_back_node(shell->env_l, envp[i]);
	if (i == 0)
		fill_basic_env(shell);
	shell->sorted_env_l = malloc(sizeof(*(shell->env_l)));
	if (!shell->sorted_env_l)
		return (printf("Malloc: Sorted Env failed\n"));
	shell->sorted_env_l = sort_list(shell->env_l);
	shell->ls_cmd = malloc(sizeof(char *) * 2);
	if (!shell->ls_cmd)
		return (printf("Malloc: LsCommand failed\n"));
	initialize_variables(shell);
	return (EXIT_SUCCESS);
}

int	initialize_variables(t_shell *shell)
{
	shell->all_path = NULL;
	shell->array_env = NULL;
	shell->user_command->nb_elem = 0;
	shell->multi_cmd = NULL;
	shell->out = false;
	shell->current_dir_path = getcwd(NULL, 0);
	shell->previous_dir_path = getcwd(NULL, 0);
	shell->wild_before = NULL;
	shell->wild_after = NULL;
	shell->ls_cmd[0] = ft_strdup("ls");
	shell->ls_cmd[1] = NULL;
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	shell->error_value = NULL;
	return (EXIT_SUCCESS);
}

int	get_number_of_commands(t_shell *shell)
{
	t_tok	*temp;
	int		nb_of_cmd;

	nb_of_cmd = 1;
	temp = shell->user_command->start;
	while (temp)
	{
		if (ft_strncmp(temp->var, "|", 2) == 0 && temp->quote == 0)
			nb_of_cmd++;
		temp = temp->next;
	}
	return (nb_of_cmd);
}
