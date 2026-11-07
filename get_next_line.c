/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anboujel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 15:33:52 by anboujel          #+#    #+#             */
/*   Updated: 2026/11/03 02:35:49 by anboujel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*save_more(char *stokage)
{
	int		i;
	int		j;
	char	*buffer;

	i = 0;
	while (stokage[i] && stokage[i] != '\n')
		i++;
	if (!stokage[i])
		return (free(stokage), NULL);
	buffer = malloc(sizeof(char) * (ft_strlen(stokage) - i));
	if (!buffer)
		return (free(stokage), NULL);
	i++;
	j = 0;
	while (stokage[i + j])
	{
		buffer[j] = stokage[i + j];
		j++;
	}
	buffer[j] = '\0';
	free(stokage);
	return (buffer);
}

char	*get_the_line(char *stokage)
{
	int		i;
	char	*line;
	int		lenght;

	lenght = 0;
	i = 0;
	if (!stokage || !stokage[0])
		return (NULL);
	while (stokage[i] && stokage[i] != '\n')
		i++;
	lenght = len_befor_new_line(stokage);
	line = malloc(lenght + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (stokage[i] && stokage[i] != '\n')
	{
		line[i] = stokage[i];
		i++;
	}
	if (stokage[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*read_buffer(char *stokage, int fd)
{
	char	*store_s;
	int		bite_ret;
	char	*buff;

	store_s = malloc(BUFFER_SIZE + 1);
	if (!store_s)
		return (free(stokage), NULL);
	bite_ret = 0;
	while (!check_newline(stokage))
	{
		bite_ret = read(fd, store_s, BUFFER_SIZE);
		if (bite_ret == -1)
			return (free(store_s), free(stokage), NULL);
		if (bite_ret == 0)
			break ;
		store_s[bite_ret] = '\0';
		buff = ft_strjoin(stokage, store_s);
		if (!buff)
			return (free(store_s), free(stokage), NULL);
		if (stokage)
			free(stokage);
		stokage = buff;
	}
	return (free(store_s), stokage);
}

char	*get_next_line(int fd)
{
	static char	*stokage;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (free(stokage), stokage = NULL, NULL);
	stokage = read_buffer(stokage, fd);
	if (stokage == NULL)
		return (NULL);
	line = get_the_line(stokage);
	if (!line)
	{
		free(stokage);
		stokage = NULL;
		return (NULL);
	}
	stokage = save_more(stokage);
	return (line);
}
// the buffer size = 20;
// the rirst call
//||||||||||||||||||||||||||||||||||||||||||| lest say here is the 20 bite
// anas boujellaba amamam jfhwfh \n hfeuihfufguwfh \n hwoqhhowoqhgpighhg \n;
//	| here the 20 bites ends ;

// so the we gona gopy the 20 bites that buffer = 20;