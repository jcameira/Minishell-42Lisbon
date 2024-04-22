/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 12:26:37 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/22 17:34:23 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	if (argc != 1)
		return (error_message(WRONG_ARG_N), 1);
	(void) argv;
	// copy enviroment variables into minishell 
	// increment shell level (if no enviroment variables then set to 1)
	// if no enviroment variables set PWD, else update PWD
	// start minishell loop
	return (0);
}