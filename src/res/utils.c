


int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (2048);
	}
	else
		return (0);
}


int     is_numeric(char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}
