/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 17:46:00 by elpastor          #+#    #+#             */
/*   Updated: 2023/01/09 17:50:37 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Proteger ft_strlen, ajouter GNL

// typedef enum s_type
// {
// 	north,
// 	south,
// 	west,
// 	east,
// }	t_type;
// ajouter dans cub.h

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

char	*get_path_texture(char *s, int *count)
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
	*count += i;
	return (path_texture);
}

int	get_texture(char *s, t_vars *vars, int texture_type)
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
	vars->textures[texture_type] = get_path_texture(&s[i], &i);
	return (i + 1);
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

int	get_color(char *s, t_vars *vars)
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
		vars->floor = rgb2img(red << 16 | green << 8 | blue);
	else
		vars->ceiling = rgb2img(red << 16 | green << 8 | blue);
	return (i + 1);
}

int	is_in_map(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '\n')
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
				&& s[i] != 'W' && s[i] != 'E' && s[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	get_color_and_texture(char *s, t_vars *vars)
{
	int	i;

	i = 0;

	while (s[i])
	{
		if (s[i] == 'N' && s[i + 1] == 'O')
			i += get_texture(&s[i], vars, 0);
		else if (s[i] == 'S' && s[i + 1] == 'O')
			i += get_texture(&s[i], vars, 1);
		else if (s[i] == 'W' && s[i + 1] == 'E')
			i += get_texture(&s[i], vars, 2);
		else if (s[i] == 'E' && s[i + 1] == 'A')
			i += get_texture(&s[i], vars, 3);
		else if (s[i] == 'F' || s[i] == 'C')
			i += get_color(&s[i], vars);
		else if (s[i] != '\n' && is_in_map(&s[i]))
			break;
		else
			i++;
	}
	return (i);
}

int	map_is_valid(char *s)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (s[i])
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
				&& s[i] != 'W' && s[i] != 'E' && s[i] != ' ' && s[i] != '\n')
			return (0);
		if (ret == 0 && (s[i] == 'N' || s[i] == 'S' || s[i] == 'W' || s[i] == 'E'))
			ret = 1;
		else if (ret == 1 && (s[i] == 'N' || s[i] == 'S' || s[i] == 'W' || s[i] == 'E'))
			return (0);
		i++;
	}
	return (1);
}

int	map_height(char *s)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] && is_in_map(&s[i]))
	{
		if (s[i] == '\n')
			count++;
		i++;
	}
	return (count);
}

int	get_map_length(char **map)
{
	int	i;
	int	max;
	
	i = 0;
	max = 0;
	while (map[i])
	{
		if ((int)ft_strlen(map[i]) > max)
			max = ft_strlen(map[i]);
		i++;
	}
	return (max);
}

void	get_map_line(char **new_map, char *map, int map_length)
{
	int	i;

	new_map[0] = (char *)malloc(sizeof(char) * (map_length + 1));
	if (!new_map[0])
		ft_print_error_exit("Error\nMalloc failed\n");
	i = 0;
	while (map[i])
	{
		if (map[i] == ' ')
			new_map[0][i] = '1';
		else
			new_map[0][i] = map[i];
		i++;
	}
	while (i < map_length)
	{
		new_map[0][i] = '1';
		i++;
	}
	new_map[0][i] = 0;
}

int	map_is_closed(char **map)
{
	int	i;
	int	j;

	i = -1;
	while (map[++i])
		if (map[i][0] != '1')
			return (0);
	j = -1;
	i--;
	while (map[i][++j])
		if (map[i][j] != '1')
			return (0);
	j--;
	i++;
	while (--i >= 0)
		if (map[i][j] != '1')
			return (0);
	i++;
	while (j >= 0)
	{
		if (map[i][j] != '1')
			return (0);
		j--;
	}
	return (1);
}

void	get_map(char *s, t_vars *vars)
{
	int		i;
	char	**map;

	if (!map_is_valid(s))
		ft_print_error_exit("Error\nFile '.cub' invalid, wrong char in map or 2 players in map\n");
	map = ft_split(s, '\n');
	i = 0;
	vars->map = (char **)malloc(sizeof(char *) * (map_height(s) + 1));
	if (!vars->map)
		ft_print_error_exit("Error\nMalloc failed\n");
	while (map[i])
	{
		get_map_line(&vars->map[i], map[i], get_map_length(map));
		i++;
	}
	printf("i = %d\n", i);
	vars->map[i] = 0;
	if (!map_is_closed(vars->map))
		ft_print_error_exit("Error\nFile '.cub' invalid, map is not closed\n");
}

void	parsing(char *fichier, t_vars *vars)
{
	char	*str;
	int		i;

	str = file_to_str(fichier);
	if (!str)
		ft_print_error_exit("Error\nFile '.cub' empty\n");
	i = get_color_and_texture(str, vars);
	get_map(&str[i], vars);
	free(str);
}
