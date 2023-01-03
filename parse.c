/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 17:46:00 by elpastor          #+#    #+#             */
/*   Updated: 2023/01/03 19:09:09 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

int	find_char(char *s, char c)
{
	int	i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i] && s[i] != '\n')
		if (s[i] == c)
			return (1);
	return (0);
}

char	*ft_strjoinspe(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*dst;

	if (!s2)
		return (NULL);
	dst = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (s1[i])
		{
			dst[i] = s1[i];
			i++;
		}
		free(s1);
	}
	j = -1;
	while (s2[++j])
		dst[i + j] = s2[j];
	dst[i + j] = 0;
	return (dst);
}

//BESOIN DE GNL

char	*file_to_str(char *fichier)
{
	int		fd;
	char	*ret;
	char	*str;

	str = NULL;
	fd = open(fichier, O_DIRECTORY);
	if (fd != -1)
		ft_print_error_exit("Error\nFile '.cub' is a directory\n");
	fd = open(fichier, O_RDONLY);
	if (fd == -1)
		ft_print_error_exit("Error\nFile '.cub' invalid\n");\
	ret = get_next_line(fd);
	while (ret)
	{
		str = ft_strjoinspe(str, ret);
		free(ret);
		ret = get_next_line(fd);
	}
	close(fd);
	return (str);
}

void	get_texture(char *s, char **texture, t_vars *vars)
{
	if (*texture != NULL)
		ft_print_error_exit("Error\nFile '.cub' invalid, too much textures\n");
	if (!find_char(s, '.') && !find_char(s, '/'))
		ft_print_error_exit("Error\nFile '.cub' invalid, wrong texture format\n");

}

void	get_color_and_texture(char *s, t_vars *vars)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == 'N' && s[i + 1] == 'O')
			get_texture(&s[i], vars);
		else if (s[i] == 'S' && s[i + 1] == 'O')
			get_texture(&s[i], vars);
		else if (s[i] == 'W' && s[i + 1] == 'E')
			get_texture(&s[i], vars);
		else if (s[i] == 'E' && s[i + 1] == 'A')
			get_texture(&s[i], vars);
		else if (s[i] != ' ' && s[i] != 'N' && s[i] != 'S' && s[i] != 'W'
				&& s[i] != 'E' && s[i] != 'F' && s[i] != 'C')
		i++;
	}
}

void	parsing(char *fichier, t_vars *vars)
{
	char	*str;

	str = file_to_str(fichier);
	if (!str)
		ft_print_error_exit("Error\nFile '.cub' empty\n");
	get_color_and_texture(str, vars);
	
}

int	error_map(char *map)
{
	int	i;

	if (!map);
		return (1);
	i = 0;
	while (map[i])
	{
		
	}
	
}