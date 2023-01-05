/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 17:46:00 by elpastor          #+#    #+#             */
/*   Updated: 2023/01/05 20:29:15 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

Proteger ft_strlen, ajouter GNL

typedef enum s_type
{
	north,
	south,
	west,
	east,
}	t_type;
ajouter dans cub.h

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
		ft_print_error_exit("Error\nFile '.cub' invalid\n");
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

char	*get_path_texture(char *s)
{
	int		i;
	int		j;
	char	*path_texture;

	i = 0;
	while (s[i] && s[i] != '\n')
		i++;
	path_texture = (char *)malloc(sizeof(char) * (i + 1));
	if (!path_texture)
		ft_print_error_exit("Error\nMalloc failed\n");
	j = 0;
	while (j < i)
	{
		path_texture[j] = s[j];
		j++;
	}
	path_texture[j] = 0;
	return (path_texture);
}

void	get_texture(char *s, t_vars *vars, int texture_type)
{
	int		i;

	if (vars->textures[texture_type] != NULL)
		ft_print_error_exit("Error\nFile '.cub' invalid, too much textures\n");
	if (!find_char(s, '.') && !find_char(s, '/'))
		ft_print_error_exit("Error\nFile '.cub' invalid, wrong texture format\n");
	i = 2;
	while (s[i] && s[i] != '.' && s[i] != '\n')
	{
		if (s[i] != ' ')
			ft_print_error_exit("Error\nFile '.cub' invalid, wrong texture format\n");
		i++;
	}
	if (s[i] == '\n')
		ft_print_error_exit("Error\nFile '.cub' invalid, no texture\n");
	vars->textures[texture_type] = get_path_texture(&s[i]);
}


int		check_color(char *s)
{
	int	i;

	i = 1;
	if (s[i++] != ' ')
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (s[i++] != ',')
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (s[i++] != ',')
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (s[i] != '\n')
		return (0);
	return (1);
}

void	get_color(char *s, t_vars *vars)
{
	int	i;
	int	red;
	int	green;
	int	blue;

	if (!check_color(s))
		ft_print_error_exit("Error\nFile '.cub' invalid color syntax\n");
	i = 1;
	red = 0;
	green = 0;
	blue = 0;
	while (ft_isdigit(s[++i]))
		red = red * 10 + (s[i] - '0');
	while (ft_isdigit(s[++i]))
		green = green * 10 + (s[i] - '0');
	while (ft_isdigit(s[++i]))
		blue = blue * 10 + (s[i] - '0');
	if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255)
		ft_print_error_exit("Error\nFile '.cub' invalid color values\n");
	if (s[0] == 'F')
		vars->floor = red << 16 | green << 8 | blue;
	else
		vars->ceiling = red << 16 | green << 8 | blue;
}

void	get_color_and_texture(char *s, t_vars *vars)
{
	int	i;

	i = 0;

	while (s[i])
	{
		if (s[i] == 'N' && s[i + 1] == 'O')
			get_texture(&s[i], vars, 0);
		else if (s[i] == 'S' && s[i + 1] == 'O')
			get_texture(&s[i], vars, 1);
		else if (s[i] == 'W' && s[i + 1] == 'E')
			get_texture(&s[i], vars, 2);
		else if (s[i] == 'E' && s[i + 1] == 'A')
			get_texture(&s[i], vars, 3);
		else if (s[i] == 'F' || s[i] == 'C')
			get_color(&s[i], vars);
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
	//MAP
}

// int	error_map(char *map)
// {
// 	int	i;

// 	if (!map);
// 		return (1);
// 	i = 0;
// 	while (map[i])
// 	{
		
// 	}
	
// }