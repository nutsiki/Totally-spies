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
    static int j;

    i = -1;
	if (!(converge = malloc(sizeof(t_converge))))
		return (0);
	while (i++ < 20)
	{
		if (((char *)tetri->content)[i] == '#')
		{
			cc = (t_converge_count){converge, 0};
			if (flood_fill(tetri->content, &cc, i) == 4)
			{
			    converge->index = j++;
				tetri->content = converge;
				return (1);
			}
			return (0);
		}
	}
	return (0);
}
int ft_sqrt(int nb)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while(j < nb)
    {
        j = i * i;
        i++;
    }
    return (i - 1);
}
char	**init_map(int nb)
{
	int x;
	int y;
	char **map;

	x = 0;
	y = 0;
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
    newmap = init_map(cpt);
	while (x < (cpt) - 1)
	{
		y = 0;
		while (y <= cpt - 1)
		{
			if (map[x][y] != '\n') {
//				printf("map vaut %c et newmap vaut %c  en (%d,%d)\n", map[x][y], newmap[x][y], x, y );
				newmap[x][y] = map[x][y];
			}
//			printf("apres map vaut %c et newmap vaut %c  en (%d,%d)\n", map[x][y], newmap[x][y], x, y );
			y++;
		}
		x++;
	}
    return (newmap);
}
int put_piece(char **map, t_converge *tetri, int x, int y)
{
    t_coord diff;
    int i;
    int cpt;

	cpt = ft_strlen(*map);
    diff = (t_coord){0, 0};
    if (map[y][x] == '.')
    {
        diff.x = x - tetri->p[0].x;
        diff.y = y - tetri->p[0].y;
        i = 0;
        while (i < 4)
        {
//            printf("diff x : %d | tet px : %d | diff y : %d | tet py : %d\n", diff.x, tetri->p[i].x, diff.y, tetri->p[i].y);
//            printf("map x : %d | map y : %d\n", (diff.x + tetri->p[i].x), (diff.y + tetri->p[i].y));
            if ((tetri->p[i].x + diff.x) >= 0 && (tetri->p[i].y + diff.y) >= 0 && (tetri->p[i].x + diff.x) < (cpt - 1) && (tetri->p[i].y + diff.y) < (cpt - 1) && (map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x]) == '.')
            {
//                printf("je met un A %d\n", cpt);
                map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x] = 'A' + tetri->index;
            }
            else
            {
                while (i--)
                {
//                    printf("je retire un A %d\n", cpt);
                    map[tetri->p[i].y + diff.y][tetri->p[i].x + diff.x] = '.';
                }
                break;
            }
            i++;
        }
        if (i == 4)
            return (1);
    }
	return (0);
}

int remove_piece(t_converge *tetri, char **map)
{
    int x;
    int y;
    int cpt;

    cpt = ft_strlen(*map);
    y = 0;
    while(y < cpt - 1)
    {
        x = 0;
        while (x < cpt)
        {
            if(map[y][x] == 'A' + tetri->index)
                map[y][x] = '.';
            x++;
        }
        y++;
    }
}

int resolv(t_mailon *lst, char **map)
{
    static int piece;
    int x;
    int y;
    int cpt;

    cpt = ft_strlen(*map);
    y = 0;
    if (lst == NULL)
        return (1);
    while(y < cpt - 1)
    {
        x = 0;
        while (x < cpt)
        {
            if (put_piece(map, lst->content, x, y))
            {
                piece++;
//                printf("j ai mit la piece %d en %d / %d \n", piece, x, y);
                if (resolv(lst->next, map))
                {
                    return (1);
                }
                piece--;
                remove_piece(lst->content, map);
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
	map = init_map(ft_sqrt(ft_lstsize(lst) * 4));
    int l = 0;
    int m = 0;
    while (l < (ft_strlen(*map)) - 1)
    {
        m = 0;
        while (m <= (ft_strlen(*map)) - 1)
        {
            printf("%c",map[l][m]);
            m++;
        }
        l++;
    }
//	resolv(lst, map);
//	put_piece(map, lst->content, 0, 0);
//	lst = lst->next;
//	put_piece(map, lst->next->content, 1, 0);
//	put_piece(map, lst->next->next->content);
//    l = 0;
//    while (l < (ft_strlen(*map)) - 1)
//    {
//        m = 0;
//        while (m <= (ft_strlen(*map)) - 1)
//        {
//            printf("%c",map[l][m]);
//            m++;
//        }
//        l++;
//    }
    while (!resolv(lst,map))
    {
//        l = 0;
//        while (l < (ft_strlen(*map)) - 1)
//        {
//            m = 0;
//            while (m <= (ft_strlen(*map)) - 1)
//            {
//                printf("%c",map[l][m]);
//                m++;
//            }
//            l++;
//        }
//        printf("not resolv\n");
        map = more_space(map);
    }
    l = 0;
    while (l < (ft_strlen(*map)) - 1)
    {
        m = 0;
        while (m <= (ft_strlen(*map)) - 1)
        {
            printf("%c",map[l][m]);
            m++;
        }
        l++;
    }
    printf("yo\n");
    return (0);
}

