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

int		main(void)
{
	t_mailon *new;
	t_mailon *last;
	char buf[BUFFER_SIZE + 1];
	int size;
	int fd;

	last = NULL;
	if ((fd = open("tetri.txt", O_RDONLY)) == -1)
		printf("erreur dans le fichier");
	while ((size = read(fd, buf, BUFFER_SIZE)) > 0) {
		buf[size] = '\0';
		new = ft_lstnew(ft_strdup(buf));
		ft_lstadd_back(&last, new);
	}
	while (last)
	{
		printf("%s", last->content);
		last = last->next;
	}
}

