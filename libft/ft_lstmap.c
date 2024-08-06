/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yojin <yojin@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 15:14:17 by yojin             #+#    #+#             */
/*   Updated: 2023/11/24 14:22:10 by yojin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*node;
	t_list	*new_node;
	void	*new_content;

	if (!lst)
		return (0);
	new_lst = NULL;
	node = lst;
	while (node)
	{
		new_content = f(node->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			ft_lstclear(&new_lst, del);
			if (new_content)
				del(new_content);
			return (0);
		}
		ft_lstadd_back(&new_lst, new_node);
		node = node->next;
	}
	return (new_lst);
}
