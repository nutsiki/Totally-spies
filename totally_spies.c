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

int	put_piece(t_mailon *tetri, t_mailon *map)
{

}

//int resolv()
//{
//	if (piece == nb_piece + 1)
//		return (1);
//	while()
//		while() {
//			if (placerlapiece()) {
//
//				if (resolv(++piece)) {
//					return (1);
//				}
//				retirerpice();
//			}
//		}
//	return (0);
//}

int		main(void)
{
	t_mailon *new;
	t_mailon *lst;
	char buf[SIZE_TETRI + 1];
	int size;
	int fd;

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
//		init_map();
//		while (!resolv()){
//			returnagrandirlamap();
//		}
	}
	return (0);
}

