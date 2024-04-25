/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 18:46:55 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/25 12:40:12 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Builtin functions to be made
//->echo (need to be able to handle -n option)
//->cd (only with relative or absolute path, should be easy enough to add only
//cd, cd .. and cd - but totally not mandatory, just easy enough)
//->pwd
//->export
//->unset
//->env
//->exit

//Was thinking of a way to do this in a function array that is itself a callable
//function that "turns itself" into the correct builtin to be executed