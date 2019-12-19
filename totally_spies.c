#include "totally_spies.h"

t_mailon	*ft_lstnew(void *content)
{
	t_mailon *n;

	if (!(n = (t_mailon*)malloc(sizeof(t_mailon))))
		return (NULL);
	n->content = content;
	n->next = NULL;
	return (n);
}

char	*ft_strdup(const char *s)
{
	char *ps;

	if (!(ps = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1))))
		return (NULL);
	ft_strcpy(ps, s);
	return (ps);
}



int ft_check_tetri(char *tetri)
{
	int i;
	int p;
	int h;

	i = 0;
	p = 12;
	h = 4;
	while (tetri[i])
	{
		if (tetri[i] == '.')
			p--;
		if (tetri[i] == '#')
			h--;
		if ((((i % 5)  == 4) && (tetri[i] != '\n'))
		|| p < 0 || h < 0 || (i == 19 && (h + p)))
			return (0);
		i++;
	}
	return (1);
}

int flood_fill(char *map, t_converge_count *tetri, int i)
{
	if (tetri->nb_hash == 4)
		return (0);
	map[i] = 'o';
	tetri->converge->p[tetri->nb_hash++] = (t_coord){i % 5, i / 5};
	if (i - 1 >= 0 && map[i-1] == '#')
		flood_fill(map, tetri, i-1);
	if (i + 1 < 20 && map[i+1] == '#')
		flood_fill(map, tetri, i+1);
	if (i - 5 >= 0 && map[i-5] == '#')
		flood_fill(map, tetri, i-5);
	if (i + 5 < 20 && map[i+5] == '#')
		flood_fill(map, tetri, i+5);
	return (tetri->nb_hash);
}

int ft_create_tetri(t_mailon *tetri)
{
	t_converge *converge;
	t_converge_count cc;
	int i;
	int hashtag;

	i = 0;
	hashtag = 0;
	if (!(converge = malloc(sizeof(t_converge))))
		return (0);
	while (i < 20)
	{
		if (((char *)tetri->content)[i] == '#')
		{
			cc = (t_converge_count){converge, 0};
			if (flood_fill(tetri->content, &cc, i) == 4)
			{
				tetri->content = converge;
				return (1);
			}
			return (0);
		}
		i++;
	}
	return (0);
}

char	**init_map(int nb)
{
	int x;
	int y;
	char **map;

	x = 0;
	y = 0;
//	printf("%d\n", nb);
	map = (char**)malloc(sizeof(char*)*nb);
	while(x < nb)
	{
		map[x] = (char*)malloc(sizeof(char)*nb);
		x++;
	}
	x = 0;
	while (x < nb)
	{
	    y = 0;
		while (y < nb)
		{
			map[x][y] = '.';
			y++;
		}
		map[x][y] = '\n';
//		printf("la ligne vaut %s\n", map[x]);
		x++;
	}
	return (map);
}

char    **more_space(char **map)
{
	char **newmap;
	int cpt;
	int x;
	int y;

	x = 0;
	y = 0;
	cpt = ft_strlen(*map);
	newmap = init_map(cpt + 1);
//	while (x < 4)
//	{
//		y = 0;
//		while (y <= 4)
//		{
//			printf("%c",map[x][y]);
//			y++;
//		}
//		x++;
//	}
//	x = 0;
//	while (x < 5)
//	{
//		y = 0;
//		while (y <= 5)
//		{
//			printf("%c",newmap[x][y]);
//			y++;
//		}
//		x++;
//	}
	printf("cpt vaut %d\n", cpt);
	while (x < (cpt-1))
	{
		y = 0;
		while (y < (cpt -1))
		{
			if (map[x][y] != '\n') {
				printf("map vaut %c et newmap vaut %c  en (%d,%d)\n", map[x][y], newmap[x][y], x, y );
				newmap[x][y] = map[x][y];
			}
			printf("apres map vaut %c et newmap vaut %c  en (%d,%d)\n", map[x][y], newmap[x][y], x, y );
			y++;
		}
		x++;
	}
	printf("end\n");
    return (newmap);
}
//int resolv(t_mailon *lst, char **map)
//{
//    int piece;
//
//    piece = 0;
//	if (piece == (ft_lstsize(lst) + 1))
//		return (1);
//	while(piece < )
//	{
//        while ()
//        {
//            if (placerlapiece())
//            {
//                if (resolv(++piece))
//                {
//                    return (1);
//                }
//                retirerpice();
//            }
//        }
//    }
//		return (0);
//}

int put_piece(char **map, t_converge *tetri)
{
    t_coord diff;
    int i;
    int x;
    int y;
    int cpt;

	cpt = ft_strlen(*map);
	x = 0;
	y = 0;
    diff = (t_coord){0, 0};
	while (y < cpt)
    {
		x = 0;
		while (x < cpt)
        {
			if (map[y][x] == '.')
            {
				diff.x = x - tetri->p[0].x;
				diff.y = y - tetri->p[0].y;
				i = 0;
				while (i < 4)
				{
//					printf("diff x : %d | tet px : %d | diff y : %d | tet py : %d\n", diff.x, tetri->p[i].x, diff.y, tetri->p[i].y);
//					printf("map x : %d | map y : %d\n", (diff.x + tetri->p[i].x), (diff.y + tetri->p[i].y));
					if ((map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x]) == '.')
					{
						map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x] = 'A';
					}
                	else
                	{
                		while (i--)
						{
							map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x] = '.';
						}
						break;
					}
                	i++;
				}
                if (i == 4)
					return (1);
            }
            x++;
        }
        y++;
    }
	return (0);
}

int		main(void)
{
	t_mailon *new;
	t_mailon *lst;
	char buf[SIZE_TETRI + 1];
	int size;
	int fd;
	char **map;

	lst = NULL;
	if ((fd = open("tetri.txt", O_RDONLY)) == -1)
		printf("erreur dans le fichier");
	while ((size = read(fd, buf, SIZE_TETRI)) > 0)
	{
		buf[size] = '\0';
		printf("buf -> \n%s\n", buf);
		if (size != SIZE_TETRI || !ft_check_tetri(buf))
			return (1);
		new = ft_lstnew(ft_strdup(buf));
		ft_lstadd_back(&lst, new);
		ft_create_tetri(new);
		for (int i = 0; i < 4; i++)
			printf("%d, %d\n", ((t_converge*)new->content)->p[i].x, ((t_converge*)new->content)->p[i].y);
		printf("\n");
		if (!read(fd, buf, 1))
			break ;
		if (buf[0] != '\n')
			return (1);
	}
	map = init_map(ft_lstsize(lst));
	put_piece(map, lst->content);
//	lst = lst->next;
	put_piece(map, lst->next->content);
	put_piece(map, lst->next->next->content);
	map = more_space(map);
//    while (!resolv(lst,map))
//    {
//        return (more_space(map));
//    }
	int x = 0;
	int y = 0;
	while (x < (ft_strlen(*map)) - 1)
	{
	    y = 0;
		while (y <= (ft_strlen(*map)) - 1)
		{
			printf("%c",map[x][y]);
			y++;
		}
		x++;
	}
    return (0);
}

