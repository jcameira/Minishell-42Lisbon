/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:30 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/24 02:23:51 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->check if there is anything in need of expansion, in particular for the presence
//of '$' to expand enviroment variables or '*' for wildcards
//->send the full ast with all expansions completed to the executor