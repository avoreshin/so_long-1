/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ghosts.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/04 16:59:34 by aperez-b          #+#    #+#             */
/*   Updated: 2021/08/13 18:59:00 by aperez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/game.h"
#include <mlx.h>

t_list	*ft_chooseghcolor(t_game *g, int i, int dir)
{
	t_list	*anim;
	char	*s;
	int		j;

	j = 0;
	anim = NULL;
	s = NULL;
	s = ft_substr("sprites/Ghosts/R/", 0, 17);
	if (i > 0)
		ft_memset(&s[15], COLORS[i % 7], 1);
	if (dir == N)
		anim = ft_load_north(g, s, j);
	if (dir == S)
		anim = ft_load_south(g, s, j);
	if (dir == E)
		anim = ft_load_east(g, s, j);
	if (dir == W)
		anim = ft_load_west(g, s, j);
	free(s);
	return (anim);
}

void	ft_load_ghosts(t_game *g)
{
	t_player	*ghost;
	int			i;
	int			size;

	i = 0;
	ghost = g->gh;
	while (ghost)
	{
		ghost->sprites.up = ft_chooseghcolor(g, i, N);
		ghost->sprites.up_bak = ghost->sprites.up;
		ghost->sprites.down = ft_chooseghcolor(g, i, S);
		ghost->sprites.down_bak = ghost->sprites.down;
		ghost->sprites.right = ft_chooseghcolor(g, i, E);
		ghost->sprites.right_bak = ghost->sprites.right;
		ghost->sprites.left = ft_chooseghcolor(g, i, W);
		ghost->sprites.left_bak = ghost->sprites.left;
		ghost->sprites.panic1 = mlx_xpm_file_to_image(g->id, \
			"sprites/Ghosts/Panic/ghost_panic1.xpm", &size, &size);
		ghost->sprites.panic2 = mlx_xpm_file_to_image(g->id, \
			"sprites/Ghosts/Panic/ghost_panic2.xpm", &size, &size);
		ghost->sprites.black = mlx_xpm_file_to_image(g->id, \
			"sprites/Ghosts/black.xpm", &size, &size);
		ghost = ghost->next;
		i++;
	}
}

void	ft_put_ghosts(t_game *g)
{
	t_player	*ghost;

	ghost = g->gh;
	while (ghost)
	{
		if (ghost->dir == N && !g->panic_mode)
			ft_anim_north(g, ghost);
		if (ghost->dir == S && !g->panic_mode)
			ft_anim_south(g, ghost);
		if ((ghost->dir == E || ghost->dir == ST) && !g->panic_mode)
			ft_anim_east(g, ghost);
		if (ghost->dir == W && !g->panic_mode)
			ft_anim_west(g, ghost);
		if (g->panic_mode && g->n_frames % (5 * ANIM_RATE) < (3 * ANIM_RATE / 2))
			mlx_put_image_to_window(g->id, g->w_id, ghost->sprites.panic1, \
				ghost->win_pos.x, ghost->win_pos.y);
		else if (g->panic_mode)
			mlx_put_image_to_window(g->id, g->w_id, ghost->sprites.panic2, \
				ghost->win_pos.x, ghost->win_pos.y);
		ghost = ghost->next;
	}
}

void	ft_update_ghosts(t_game *g, t_player **pl)
{
	t_player	*ghost;
	t_player	*closest;
	int			dir;

	ghost = g->gh;
	while (ghost && !ghost->moving)
	{
		closest = ft_getnearestpac(g, ghost);
		dir = ft_choose_dir(g, ghost, closest);
		ghost->moving = 1;
		ft_move_ghost(dir, g, ghost, pl);
		ghost = ghost->next;
	}
}

void	ft_move_ghost(int d, t_game *g, t_player *gh, t_player **pl)
{
	ft_memset(&g->map[gh->pos.y][gh->pos.x], \
		'0', g->map[gh->pos.y][gh->pos.x] == 'G');
	if (d == N)
		gh->pos.y--;
	if (d == S)
		gh->pos.y++;
	if (d == E)
		gh->pos.x++;
	if (d == W)
		gh->pos.x--;
	gh->dir = d;
	if (g->map[gh->pos.y][gh->pos.x] == 'P')
		g->pac_dying = 1;
}
