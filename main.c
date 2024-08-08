/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpennisi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:05:31 by lpennisi          #+#    #+#             */
/*   Updated: 2024/08/03 12:39:36 by lpennisi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main()
{

    char *input;
    while (1)
    {
        input = readline("minishell$ ");
        printf("You entered: %s\n", input);
    }
    
    return 0;
}