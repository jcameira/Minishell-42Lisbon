#include <lexer.h>
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

void	print_message(int curr_lvl, int subshell_lvl, t_ast_token_type type,
		char *content)
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
	printf ("LEVEL: %d	SUBSHELL_LEVEL: %d	TYPE: %s		CONTENT: %s\n",
		curr_lvl, subshell_lvl, write_type, content);
}

void	print_level(t_ast *root, int level, int curr_lvl)
{
	if (root == NULL)
		return ;
	if (level == 1)
		print_message(curr_lvl, root->subshell_level, root->type,
			root->content);
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

void	print_cmd_table(t_command_table *command_table)
{
	t_redir_list	*tmp_redir;
	int				i;

	while (command_table)
	{
		printf("\nTYPE:	%d	SUBSHELL_LVL:	%d\n", command_table->type,
			command_table->subshell_level);
		i = -1;
		printf("\nCommand\n");
		if (command_table->simplecmd)
		{
			while (command_table->simplecmd->arg_arr[++i])
				printf("%s->", command_table->simplecmd->arg_arr[i]);
			printf("\n");
		}
		printf("\nRedirections\n");
		tmp_redir = command_table->redirs;
		while (tmp_redir)
		{
			if (tmp_redir->type == HERE_DOC)
				printf("'<<'\n%s\n", tmp_redir->here_doc_limiter);
			else if (tmp_redir->type == INFILE)
				printf("'<'\n%s\n", tmp_redir->file);
			else if (tmp_redir->type == APPEND)
				printf("'>>'\n%s\n", tmp_redir->file);
			else if (tmp_redir->type == OUTFILE)
				printf("'>'\n%s\n", tmp_redir->file);
			else
				printf("Empty redir node\n");
			tmp_redir = tmp_redir->next;
		}
		printf("\n");
		command_table = command_table->next;
	}
}

void	print_list(t_token_list *token_list)
{
	while (token_list)
	{
		if (token_list->token_type == AND)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_AND, token_list->content);
		else if (token_list->token_type == OR)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_OR, token_list->content);
		else if (token_list->token_type == PIPE)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_PIPE, token_list->content);
		else if (token_list->token_type == LESSER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_LESSER, token_list->content);
		else if (token_list->token_type == D_LESSER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == GREATER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_GREATER, token_list->content);
		else if (token_list->token_type == D_GREATER)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_D_GREATER, token_list->content);
		else if (token_list->token_type == L_PARENTESIS)
			printf("TOKEN TYPE = %s 	TOKEN DATA = %s\n", PRINT_L_PARENTESIS, token_list->content);
		else if (token_list->token_type == R_PARENTESIS)
			printf("TOKEN TYPE = %s 	TOKEN DATA = %s\n", PRINT_R_PARENTESIS, token_list->content);
		else if (token_list->token_type == WORD)
			printf("TOKEN TYPE = %s 		TOKEN DATA = %s\n", PRINT_WORD, token_list->content);
		else
			printf("TOKEN TYPE = %s 		TOKEN DATA = \n", PRINT_BAD_TOKEN);
		token_list = token_list->next;
	}
}


((cat << EOF > OUT && cat OUT > OUT2) || (cat << EOF > OUT3 && cat OUT3 > OUT4))