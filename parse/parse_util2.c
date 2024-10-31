/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_util2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeongbel <jeongbel@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 08:57:27 by jeongbel          #+#    #+#             */
/*   Updated: 2024/10/31 12:23:22 by jeongbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**return_tokens(t_token_state *state)
{
	char	**tokens;

	tokens = state->tokens;
	free(state);
	return (tokens);
}

char	**rm_redi_from_tokens(char **tokens)
{
	char	**new_tokens;
	int		i;
	int		j;

	new_tokens = (char **)malloc(sizeof(char *) * (ft_strslen(tokens) + 1));
	if (!new_tokens)
		handle_systemcall_error();
	i = 0;
	j = 0;
	while (tokens[i] && ft_strcmp(tokens[i], "|"))
	{
		if (!ft_strcmp(tokens[i], ">") || !ft_strcmp(tokens[i], ">>")
			|| !ft_strcmp(tokens[i], "<") || !ft_strcmp(tokens[i], "<<"))
		{
			i += 2;
			continue ;
		}
		new_tokens[j] = ft_strdup(tokens[i]);
		if (!new_tokens[j])
			handle_systemcall_error();
		i++;
		j++;
	}
	new_tokens[j] = NULL;
	return (new_tokens);
}