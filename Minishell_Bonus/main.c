/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pfaria-d <pfaria-d@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:51:51 by eleleux           #+#    #+#             */
/*   Updated: 2023/04/26 11:13:09 by pfaria-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	readline_manager(t_shell *shell)
{
	free(shell->line_readed);
	signal(SIGINT, &handler);
	signal(SIGQUIT, &handler);
	remove_ctrlc(1);
	shell->line_readed = readline("Minishell >> ");
	remove_ctrlc(0);
	if (!shell->line_readed)
		return (EXIT_FAILURE);
	if (shell->line_readed && *shell->line_readed)
		add_history(shell->line_readed);
	return (EXIT_SUCCESS);
}

int	is_builtin_command(t_shell *shell, int index)
{
	if (ft_strncmp(shell->multi_cmd[index][0], "pwd", 4) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "cd", 3) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "export", 7) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "env", 4) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "unset", 6) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "exit", 5) == 0
		|| ft_strncmp(shell->multi_cmd[index][0], "echo", 5) == 0)
		return (TRUE);
	return (FALSE);
}

int	main(int ac, char **av, char **envp)
{
	int		good;
	t_shell	shell;

	(void)av;
	if (ac != 1)
		return (printf("Minishell is pure, no arguments please\n"));
	allocate_shell(&shell, envp);
	printf(YEL "Open Minishell\n" WHT);
	while (1)
	{
		good = TRUE;
		readline_manager(&shell);
		if (!shell.line_readed)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			break ;
		}
		if (*shell.line_readed)
		{
			token_parsing(shell.user_command, shell.line_readed);
			if (shell.user_command->nb_elem && token_checker(&shell))
			{
				printf("Minishell: syntax error\n");
				clear_toklst(shell.user_command);
			}
			if (shell.user_command->nb_elem)
			{
				tokenisation(shell.user_command, shell.sorted_env_l);
				if ((shell.user_command->nb_elem != 0)
					&& (infile_redirection_parsing(&shell) != 0
						|| outfile_redirection_parsing(&shell) != 0))
					good = FALSE;
				if (good == TRUE && and_or_in_cmd(shell.user_command))
				{
					shell.pid = malloc (sizeof(pid_t)
							* get_number_of_bonus_commands(shell.user_command));
					fill_trinary_tree(shell.user_command, &shell);
					execution_bonus(&shell, shell.tree->map);
				}
				else if (good == TRUE)
					pipe_command(&shell);
				clear_toklst(shell.user_command);
				dup2(shell.saved_stdin, STDIN_FILENO);
				dup2(shell.saved_stdout, STDOUT_FILENO);
			}
		}
	}
	clean_memory(&shell);
	printf(YEL "Exit Minishell\n" WHT);
	return (0);
}
