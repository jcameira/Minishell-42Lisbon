/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/05/15 22:35:39 by jcameira         ###   ########.fr       */
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

int height(t_ast *node)
{
    if (node == NULL) {
        return 0;
    } else {
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        if (leftHeight > rightHeight) {
            return leftHeight + 1;
        } else {
            return rightHeight + 1;
        }
    }
}

void print_level(t_ast *root, int level, int curr_lvl)
{
    if (root == NULL) {
        return;
    }
    if (level == 1) {
        printf("LEVEL: %d	TYPE: %s		CONTENT: %s\n", curr_lvl, root->type, root->content);
    } else if (level > 1) {
        print_level(root->left, level - 1, curr_lvl + 1);
		print_level(root->subshell_ast, level - 1, curr_lvl + 1);
        print_level(root->right, level - 1, curr_lvl + 1);
    }
}

void print_ast(t_ast *root) {
    int h = height(root);
    for (int i = 1; i <= h; i++) {
        print_level(root, i, 0);
    }
    printf("\n");
}

void	parser(t_token_list *token_list)
{
	t_ast	*root;

	root = add_ast_node(&token_list);
	if (!root)
		return ;
	print_ast(root);
	//printf("TYPE: %s		CONTENT: %s\n", root->type, root->content);
	//free_token_list(token_list);
}
