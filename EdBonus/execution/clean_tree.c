/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleleux <eleleux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 15:11:29 by eleleux           #+#    #+#             */
/*   Updated: 2023/04/26 10:10:41 by eleleux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_branch	*clean(t_branch *temp)
{
	if (temp && temp->left)
		return (clean(temp->left));
	else if (temp && temp->right)
		return (clean(temp->right));
	else if (temp && temp->dad)
	{
		free_array(temp->cmd);
		temp->left = NULL;
		temp->right = NULL;
		temp->left_command = NULL;
		temp->right_command = NULL;
		return (clean(temp->dad));
	}
	return (temp);
}

int	is_parenthese(char *str)
{
	if (ft_strncmp(str, "(", 2) == 0 || ft_strncmp(str, ")", 2) == 0)
		return (1);
	return (0);
}

int	get_number_of_bonus_commands(t_toklst *user_command)
{
	t_tok	*temp;
	int		count;

	temp = user_command->start;
	count = 0;
	while (temp)
	{
		if (temp && is_parenthese(temp->var))
			temp = temp->next;
		else if (temp && is_operator(temp->var))
			temp = temp->next;
		else
		{
			count++;
			while (temp && !(is_operator(temp->var) || is_parenthese(temp->var)))
				temp = temp->next;
		}
	}
	return (count);
}

int	builtin_manager_bonus(t_shell *shell, int index)
{
	shell->pid[index] = fork();
	if (shell->pid[index] == 0)
	{
		redirection_parsing(shell, index);
		execute_builtin_cmd(shell, index);
		exit(1);
	}
	return (EXIT_SUCCESS);
}