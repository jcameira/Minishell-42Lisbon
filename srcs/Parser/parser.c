/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/22 21:17:03 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->use the complete ast to parse the complete command table with all simple
//commands and redirections
//->carefull with the redirections, only one redirection for infile and one
//redirection for outfile should be available, this includes redirections for
//here documents that are in any case a redirection for infile, so if we read
//multiple in or out redirections we should update the redirection in the
//command table, for this if we read the ast I think reading from leaf to
//root and from left to right the best way, this way if we constantly update
//the in and out redirection in the end we will have the last written
//redirections which are the correct ones
//->check for any possible errors coming from bad command lines (this could and
//probably should be checked only in the lexer)
//->send the complete command table to the expander

#include <parser.h>

int	height(t_ast *node)
{
	int	leftheight;
	int	rightheight;
	int	subshell;
	int	maxheight;

	if (node == NULL)
		return (0);
	else
	{
		leftheight = height(node->left);
		rightheight = height(node->right);
		subshell = height(node->subshell_ast);
		maxheight = leftheight;
		if (subshell > maxheight)
			maxheight = subshell;
		if (rightheight > maxheight)
			maxheight = rightheight;
		return (maxheight + 1);
	}
}

void	print_message(int curr_lvl, t_ast_token_type type, char *content)
{
	char	*write_type;

	write_type = NULL;
	if (type == NO_NODE)
		write_type = PRINT_NO_NODE;
	else if (type == BAD_TYPE)
		write_type = PRINT_BAD_TYPE;
	else if (type == AND_OR_SEQUENCE)
		write_type = PRINT_AND_OR_SEQUENCE;
	else if (type == PIPE_SEQUENCE)
		write_type = PRINT_PIPE_SEQUENCE;
	else if (type == REDIRECTION)
		write_type = PRINT_REDIRECTION;
	else if (type == SUBSHELL)
		write_type = PRINT_SUBSHELL;
	else if (type == SIMPLE_COMMAND)
		write_type = PRINT_SIMPLE_COMMAND;
	printf ("LEVEL: %d	TYPE: %s		CONTENT: %s\n", curr_lvl, write_type,
		content);
}

void	print_level(t_ast *root, int level, int curr_lvl)
{
	if (root == NULL)
		return ;
	if (level == 1)
		print_message(curr_lvl, root->type, root->content);
	else if (level > 1)
	{
		print_level(root->left, level - 1, curr_lvl + 1);
		print_level(root->subshell_ast, level - 1, curr_lvl + 1);
		print_level(root->right, level - 1, curr_lvl + 1);
	}
}

void	print_ast(t_ast *root)
{
	int	h;
	int	i;

	h = height(root);
	i = 1;
	while (i <= h)
	{
		print_level(root, i, 0);
		i++;
	}
}

void	parser(t_token_list *token_list)
{
	t_ast	*root;

	root = add_ast_node(&token_list);
	if (!root)
		return ;
	print_ast(root);
	free_ast(root);
}
