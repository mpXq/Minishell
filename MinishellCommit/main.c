/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleleux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:51:51 by eleleux           #+#    #+#             */
/*   Updated: 2023/03/11 18:45:41 by eleleux          ###   ########.fr       */
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

int	is_builtin_command(t_cmd *cmd)
{
	if (ft_strncmp(cmd->var[0], "pwd", 4) == 0
		|| ft_strncmp(cmd->var[0], "cd", 3) == 0
		|| ft_strncmp(cmd->var[0], "export", 7) == 0
		|| ft_strncmp(cmd->var[0], "env", 4) == 0
		|| ft_strncmp(cmd->var[0], "unset", 6) == 0
		|| ft_strncmp(cmd->var[0], "exit", 5) == 0
		|| ft_strncmp(cmd->var[0], "echo", 5) == 0)
			return (TRUE);
	return (FALSE);
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	int	good;
	t_shell	shell;

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
		/*		t_tok	*tst = shell.user_command->start;
				while (tst)
				{
					printf("command = %s\n", tst->var);
					tst = tst->next;
				}*/
				while (cmd_has_wildcard(&shell))
				{
					parse_wildcard(&shell, envp);
					if (shell.nb_of_sub == 0)
						break ;
				}
				if ((shell.user_command->nb_elem != 0) && (infile_redirection_parsing(&shell) != 0 || outfile_redirection_parsing(&shell) != 0))
					good = FALSE;
				if (good == TRUE)
					pipe_command(&shell);
				clear_toklst(shell.user_command);
				dup2(shell.saved_stdin, STDIN_FILENO);
				dup2(shell.saved_stdout, STDOUT_FILENO);
			}
		}
		//printf("errorcode = %d\n",  g_err);
	}
	clean_memory(&shell);
	printf(YEL "Exit Minishell\n" WHT);
	system("leaks minishell");
	return (0);
}
