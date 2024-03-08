/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayal-ras <ayal-ras@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:27:11 by ayal-ras          #+#    #+#             */
/*   Updated: 2024/03/08 19:14:26 by ayal-ras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int simple_cmd(char *cmd, t_data *data) {
    int builtin_index = check_builtin(cmd);
    if (builtin_index != -1)
        return builtin_command(cmd, data);
    return 1; // Not a builtin command
}

int check_builtin(char *str) {
    static char *builtins[] = {
        "echo",
        "cd",
        "pwd",
        "export",
        "unset",
        "env",
        "exit"
    };
    int i;
    for (i = 0; i < 7; i++) {
        if (strcmp(builtins[i], str) == 0)
            return i; // Return index if builtin found
    }
    return -1; // Not a builtin command
}