	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: eleleux <marvin@42.fr>                     +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2023/01/25 18:46:31 by eleleux           #+#    #+#             */
/*   Updated: 2023/02/16 16:31:28 by eleleux          ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	(void)av;
//	int	good;
	t_shell	shell;

	if (ac != 1)
		return (printf("Minishell is pure, no arguments please\n"));
	allocate_shell(&shell, envp);
	printf(YEL "Open Minishell\n" WHT);
	while (ft_strncmp(shell.line_readed, "exit", 5))
	{
		//good = true;
		readline_manager(&shell);
		if (!shell.line_readed)
		{
			ft_putchar_fd('\n', STDOUT_FILENO);
			break ;
		}
		if (shell.line_readed[0] != '\0')
		{
			token_parsing(shell.user_command, shell.line_readed);
			tokenisation(shell.user_command, shell.sorted_env_l);
			t_tok	*temp = shell.user_command->start;
			while (temp)
			{
				printf("%s\n", temp->var);
				temp = temp->next;
			}
			parse_wildcard(&shell, envp);
			printf("cc du main\n");
			temp = shell.user_command->start;
			while (temp)
			{
				printf("%s\n", temp->var);
				temp = temp->next;
			}
		/*	if ((shell.user_command->nb_elem != 0) && (infile_redirection_parsing(&shell) != 0 || outfile_redirection_parsing(&shell) != 0))
				good = false;
			//printf("tok = %s\n", shell.user_command->start->var);
			if (good == true)
			{
				if (pipe_command(&shell) != 0)
					printf("Error\n");
			}
			clear_toklst(shell.user_command);
			dup2(shell.saved_stdin, STDIN_FILENO);
			dup2(shell.saved_stdout, STDOUT_FILENO);*/
		}
	}
	clean_memory(&shell);
	return (0);
}
