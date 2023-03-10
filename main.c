/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleleux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:51:51 by eleleux           #+#    #+#             */
/*   Updated: 2023/02/22 19:03:43 by eleleux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	readline_manager(t_shell *shell)
{
	free(shell->line_readed);
	shell->line_readed = readline(MAG "Minishell >> " WHT);
	if (!shell->line_readed)
		return (EXIT_FAILURE);
	if (shell->line_readed[0] != '\0')
		add_history(shell->line_readed);
	return (EXIT_SUCCESS);
}

int	is_builtin_command(t_shell *shell, int i)
{
	if (ft_strncmp(shell->multi_cmd[i][0], "pwd", 4) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "cd", 3) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "export", 7) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "env", 4) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "unset", 6) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "exit", 5) == 0
		|| ft_strncmp(shell->multi_cmd[i][0], "echo", 5) == 0)
			return (true);
	return (false);
}

int	increment_sh_level(t_shell *shell)
{
	t_node	*temp;

	temp = shell->env_l->start;
	while (temp && ft_strncmp(temp->variable, "SHLVL=", 6))
		temp = temp->next;
	printf("sh is %s\n", temp->variable);
	printf("lvl is %c\n", temp->variable[6]);
	if (temp)
		temp->variable[6]++;
	printf("sh is %s\n", temp->variable);
	printf("lvl is %c\n", temp->variable[6]);
	return (EXIT_SUCCESS);
}

int	decrement_sh_level(t_shell *shell)
{
	t_node	*temp;

	temp = shell->env_l->start;
	while (temp && ft_strncmp(temp->variable, "SHLVL=", 6))
		temp = temp->next;
	if (temp)
		temp->variable[6]--;
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	int	good;
	t_shell	shell;

	if (ac != 1)
		return (printf("Minishell is pure, no arguments please\n"));
	signal(SIGINT, &handler);
	signal(SIGQUIT, &handler);
	allocate_shell(&shell, envp);
	printf(YEL "Open Minishell\n" WHT);
	//increment_sh_level(&shell);
	while (ft_strncmp(shell.line_readed, "exit", 5))
	{
		good = true;
		readline_manager(&shell);
		if (!shell.line_readed)
		{
				ft_putchar_fd('\n', STDOUT_FILENO);
				break ;
		}
		if (shell.line_readed[0] != '\0')
		{
			token_parsing(shell.user_command, shell.line_readed);
			if (shell.user_command->nb_elem)
			{
				tokenisation(shell.user_command, shell.sorted_env_l);
		/*		if (token_checker(&shell))
				{
					printf("minishell: syntax error\n");
					clear_toklst(shell.user_command);
				}
				else
				{*/
				while (cmd_has_wildcard(&shell))
				{
					parse_wildcard(&shell, envp);
					if (shell.nb_of_sub == 0)
						break ;
				}
				if ((shell.user_command->nb_elem != 0) && (infile_redirection_parsing(&shell) != 0 || outfile_redirection_parsing(&shell) != 0))
					good = false;
				if (good == true)
				{
					printf("cc\n");
					pipe_command(&shell);
				}
				clear_toklst(shell.user_command);
				dup2(shell.saved_stdin, STDIN_FILENO);
				dup2(shell.saved_stdout, STDOUT_FILENO);
			//	}
			}
		}
		printf("errorcode = %d\n",  g_err);
	}
	//decrement_sh_level(&shell);
	clean_memory(&shell);
	printf(YEL "Exit Minishell\n" WHT);
	return (0);
}
