/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumatsui <yumatsui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:21:59 by yumatsui          #+#    #+#             */
/*   Updated: 2024/04/27 21:18:08 by yumatsui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_strlen(char *s);
char	*ft_strchr(char *s, int c);
int		find_newline(char *s);
char	*ft_strjoin_nextline(char *s1, char *s2);
char	*free_return(char *s1);

char	*ft_prepare_for_next(char *buff, char *nextline, int index)
{
	int		j;

	if (buff[index] == '\n')
		index++;
	if (buff[index] == '\0')
	{
		nextline[0] = '\0';
		return (NULL);
	}
	j = 0;
	while (buff[index] != '\0')
	{
		nextline[j] = buff[index];
		j++;
		index++;
	}
	nextline[j] = '\0';
	return (nextline);
}

char	*ft_get_currentline(char *buff, char *nextline)
{
	char	*currentline;
	int		i;

	if (!buff)
		return (NULL);
	i = 0;
	while (buff[i] != '\n' && buff[i] != '\0')
		i++;
	currentline = (char *)malloc(sizeof(char) * (i + 2));
	if (!currentline)
		return (free_return(buff));
	i = -1;
	while (buff[++i] != '\n' && buff[i] != '\0')
		currentline[i] = buff[i];
	if (buff[i] == '\n')
		currentline[i] = buff[i];
	else
		currentline[i] = '\0';
	currentline[i + 1] = '\0';
	nextline = ft_prepare_for_next(buff, nextline, i);
	free(buff);
	return (currentline);
}

char	*ft_buff_to_currentline(int fd, char *currentline, char *nextline)
{
	char	*buff;
	int		read_byte;

	currentline = ft_strjoin_nextline(currentline, nextline);
	buff = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (NULL);
	read_byte = 1;
	while (!find_newline(currentline) && read_byte != 0)
	{
		read_byte = read(fd, buff, BUFFER_SIZE);
		if (read_byte == -1)
		{
			free(buff);
			return (free_return (currentline));
		}
		buff[read_byte] = '\0';
		currentline = ft_strjoin_nextline(currentline, buff);
	}
	free(buff);
	return (currentline);
}

char	*get_next_line(int fd)
{
	char		*currentline;
	static char	nextline[BUFFER_SIZE + 1] = "";

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	currentline = (char *)malloc(1);
	if (!currentline)
		return (NULL);
	currentline[0] = '\0';
	currentline = ft_buff_to_currentline(fd, currentline, nextline);
	if (!currentline)
		return (NULL);
	currentline = ft_get_currentline(currentline, nextline);
	if (currentline == NULL)
		return (NULL);
	return (currentline);
}

int main()
{
    int fd;
    int i;
    char *pri;
    // (void) ac;
    fd = open("test", O_RDONLY);
    i = 0;
    while (i < 7)
    {
        pri = get_next_line(fd);
        printf("output = %s", pri);
		if (!pri)
			break ;
	    free(pri);
        i++;
    }
    close(fd);
    return (0);
}