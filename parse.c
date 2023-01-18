/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 17:46:00 by elpastor          #+#    #+#             */
/*   Updated: 2023/01/17 18:52:18 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Proteger ft_strlen (if (!s) return (0)), ajouter GNL

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

void	path2img(char *path, t_img *texture, t_vars *vars)
{
	texture->ptr = mlx_xpm_file_to_image(vars->mlx, path, &(texture->px_length), &(texture->px_height));
	if (texture->ptr == NULL)
		ft_print_error_exit("Error\nMlx File_to_image failed\n");
	texture->addr = mlx_get_data_addr(texture->ptr, &texture->bits_per_pixel, &texture->line_length, &texture->endian);
}

int	get_texture(char *s, t_vars *vars, int texture_type)
{
	int		i;

	if (vars->textures[texture_type].ptr != NULL)
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
	path2img(get_path_texture(&s[i], &i), &vars->textures[texture_type], vars);
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

	if (!check_color(s) || (s[0] == 'F' && vars->floor != -1)
			|| (s[0] == 'C' && vars->ceiling != -1))
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
	if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0
			|| blue > 255)
		ft_print_error_exit("Error\nFile '.cub' invalid color values\n");
	if (s[0] == 'F')
		vars->floor = (red << 16 | green << 8 | blue);
	else
		vars->ceiling = (red << 16 | green << 8 | blue);
	return (i + 1);
}

int	is_in_map(char *s)
{
	int	i;
	int	in_map;

	i = 0;
	in_map = 0;
	if (!s)
		return (0);
	if (s[i] && s[i] == '\n' && (s[i + 1] == '0' || s[i + 1] == '1'
			|| s[i + 1] == 'N' || s[i + 1] == 'S' || s[i + 1] == 'W'
			|| s[i + 1] == 'E' || s[i + 1] == ' '))
		return (1);
	while (s[i] && s[i] != '\n')
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
				&& s[i] != 'W' && s[i] != 'E' && s[i] != ' ')
			return (0);
		else
			in_map = 1;
		i++;
	}
	return (in_map);
}

int	is_in_map_wbn(char *s)
{
	int	i;
	int	in_map;

	i = 0;
	in_map = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != '0' && s[i] != '1' && s[i] != 'N' && s[i] != 'S'
				&& s[i] != 'W' && s[i] != 'E' && s[i] != ' ' && s[i] != '\n')
			return (0);
		else
			in_map = 1;
		i++;
	}
	return (in_map);
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
	if (!s[i])
		return (0);
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
		if (ret == 0 && (s[i] == 'N' || s[i] == 'S'
				|| s[i] == 'W' || s[i] == 'E'))
			ret = 1;
		else if (ret == 1 && (s[i] == 'N' || s[i] == 'S'
				|| s[i] == 'W' || s[i] == 'E'))
			return (0);
		i++;
	}
	if (ret == 0)
		return (0);
	return (1);
}

int	get_map_height(char *s)
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

int	map_height(char **map)
{
	int	i;

	i = 0;
	while (map[i] && is_in_map_wbn(map[i]))
		i++;
	return (i);
}

int	map_length(char **map)
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

int	splited_map_is_closed(char **map, int j)
{
	int	i;

	while (map[j])
	{
		i = 0;
		while (map[j][i])
		{
			if (map[j][i] == '0' || map[j][i] == 'N' || map[j][i] == 'S'
					|| map[j][i] == 'W' || map[j][i] == 'E')
			{
				if (j == 0 || i == 0)
					return (0);
				if ((int)ft_strlen(map[j - 1]) <= i || map[j - 1][i] == ' ')
					return (0);
				if ((int)ft_strlen(map[j + 1]) <= i || map[j + 1][i] == ' ')
					return (0);
				if (map[j][i - 1] == ' ' || !map[j][i + 1]
						|| map[j][i + 1] == ' ')
					return (0);
			}
			i++;
		}
		j++;
	}
	return (1);
}

void	get_map(char *s, t_vars *vars)
{
	int		i;
	char	**map;

	if (!map_is_valid(s))
		ft_print_error_exit("Error\nFile '.cub' invalid map\n");
	map = ft_split(s, '\n');
	if (!splited_map_is_closed(map, 0))
		ft_print_error_exit("Error\nFile '.cub' invalid map not closed\n");
	i = 0;
	vars->map = (char **)malloc(sizeof(char *) * (get_map_height(s) + 1));
	if (!vars->map)
		ft_print_error_exit("Error\nMalloc failed\n");
	while (map[i])
	{
		get_map_line(&vars->map[i], map[i], map_length(map));
		i++;
	}
	vars->map[i] = 0;
	if (i < get_map_height(s))
		ft_print_error_exit("Error\nFile '.cub' invalid, map has empty line\n");
	if (map_length(vars->map) < 3 || map_height(vars->map) < 3)
		ft_print_error_exit("Error\nFile '.cub' invalid, map is too tiny\n");
}

void	verify_colors_and_texture(t_vars *vars)
{
	int	i;

	i = 0;
	if (vars->floor == -1 || vars->ceiling == -1)
		ft_print_error_exit("Error\nFile '.cub' invalid, color not defined\n");
	while (i < 4)
	{
		if (vars->textures[i].ptr == NULL)
			ft_print_error_exit("Error\nFile '.cub' invalid, texture not defined\n");
		i++;
	}
}

void	parsing(char *fichier, t_vars *vars)
{
	char	*str;
	int		i;

	vars->textures = (t_img *)malloc(sizeof(t_img) * 4);
	if (!vars->textures)
		ft_print_error_exit("Error\nMalloc failed\n");
	vars->floor = -1;
	vars->ceiling = -1;
	i = 0;
	while (i < 4)
		vars->textures[i++].ptr = NULL;
	str = file_to_str(fichier);
	if (!str)
		ft_print_error_exit("Error\nFile '.cub' empty\n");
	i = get_color_and_texture(str, vars);
	verify_colors_and_texture(vars);
	get_map(&str[i], vars);
	free(str);
}