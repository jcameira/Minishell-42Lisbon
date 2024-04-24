/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcameira <jcameira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:33:19 by jcameira          #+#    #+#             */
/*   Updated: 2024/04/24 02:22:01 by jcameira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//->parse each simple command in the leafs of the tree creating an array of its arguments
//->initialize each full command struct by adding any needed redirections
//->check for any possible errors coming from bad command lines
//->send the complete ast to the expander