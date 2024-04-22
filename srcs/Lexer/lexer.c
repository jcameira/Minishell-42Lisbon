/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:05 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/22 18:20:58 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//tokens to be searched for in order of priority
//()
//&&
//||
//|
//<
//<<
//>
//>>

//create an ast with these tokens until we reach the leafs then pass to parsing