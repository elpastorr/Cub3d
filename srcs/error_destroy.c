/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_destroy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 23:57:40 by eleotard          #+#    #+#             */
/*   Updated: 2023/01/23 17:00:59 by elpastor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub.h"

void	ft_print_error_exit(char *error)
{
	ft_putstr_fd(error, 2);
	exit (-1);
}

void	destroy_all_imgs(t_vars *vars)
{
	if (vars->textures[north].ptr)
		mlx_destroy_image(vars->mlx, vars->textures[north].ptr);
	if (vars->textures[south].ptr)
		mlx_destroy_image(vars->mlx, vars->textures[south].ptr);
	if (vars->textures[west].ptr)
		mlx_destroy_image(vars->mlx, vars->textures[west].ptr);
	if (vars->textures[east].ptr)
		mlx_destroy_image(vars->mlx, vars->textures[east].ptr);
	if (vars->game_img.ptr)
		mlx_destroy_image(vars->mlx, vars->game_img.ptr);
}

void	ft_destroy_all(char **map, void *mlx, void *win, t_vars *vars)
{
	if (vars)
	{
		destroy_all_imgs(vars);
		destroy_all_inttabs(vars);
		free(vars->textures);
	}
	if (map)
		ft_destroy_map(map);
	if (win)
		mlx_destroy_window(mlx, win);
	if (mlx)
	{
		mlx_destroy_display(mlx);
		free(mlx);
	}
}

void	ft_destroy_all_message(char **map, void	*mlx, char *message)
{
	ft_putstr_fd(message, 2);
	ft_destroy_all(map, mlx, NULL, NULL);
}

void	destroy_all_msg(t_vars *vars, char *msg, char *to_free, char **map_free)
{
	if (to_free)
		free(to_free);
	if (map_free)
		ft_destroy_map(map_free);
	destroy_all_imgs(vars);
	if (vars->textures)
		free(vars->textures);
	ft_destroy_all_message(vars->map, vars->mlx, msg);
	exit (-1);
}
