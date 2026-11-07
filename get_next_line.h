/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anboujel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 15:31:40 by anboujel          #+#    #+#             */
/*   Updated: 2026/11/02 14:41:15 by anboujel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <stdlib.h>
# include <unistd.h>

int		check_newline(char *lst);
char	*ft_strjoin(char *s1, char *s2);
int		len_befor_new_line(char *str);
size_t	ft_strlen(char *s);
char	*get_next_line(int fd);
char	*read_buffer(char *stokage, int fd);
char	*save_more(char *stokage);
char	*get_the_line(char *stokage);

#endif
