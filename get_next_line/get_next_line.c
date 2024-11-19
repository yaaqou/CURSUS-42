/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycharkou <ycharkou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 16:56:19 by ycharkou          #+#    #+#             */
/*   Updated: 2024/11/19 15:21:47 by ycharkou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

ssize_t	ft_strchr(const char *accumulation, int c)
{
	int	i;

	i = 0;
	if (!accumulation)
		return (0);
	while (accumulation[i])
	{
		if (accumulation[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

char	*read_and_accumulate(int fd, char *accumulation)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free (buffer), free(accumulation), accumulation = NULL, NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		accumulation = ft_strjoin(accumulation, buffer);
		if (ft_strchr(accumulation, '\n') != -1)
			break ;
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		return (free(accumulation), accumulation = NULL, NULL);
	return (free(buffer), buffer = NULL, accumulation);
}

char	*extract_line(char **accumulation)
{
	char	*line;
	int		nl_index;
	char	*new_accumulation;

	nl_index = ft_strchr(*accumulation, '\n');
	if (nl_index != -1)
	{
		line = ft_substr(*accumulation, 0, nl_index + 1);
		new_accumulation = ft_strdup(*accumulation + nl_index + 1);
		free(*accumulation);
		*accumulation = new_accumulation;
		if (!line || (*accumulation && !**accumulation))
			return (free(*accumulation), *accumulation = NULL, line);
	}
	else
	{
		line = ft_strdup(*accumulation);
		free(*accumulation);
		*accumulation = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*accumulation;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) == -1)
	{
		free(accumulation);
		accumulation = NULL;
		return (NULL);
	}
	accumulation = read_and_accumulate(fd, accumulation);
	if (!accumulation || !*accumulation)
	{
		free(accumulation);
		accumulation = NULL;
		return (NULL);
	}
	return (extract_line(&accumulation));
}
