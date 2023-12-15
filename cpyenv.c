#include "main.h"

/**
 * _unsetenvir - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int _unsetenvir(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = st_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = dlt_nd_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * get_ern - returns the string array copy of our environ
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_ern(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strgs(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _setenviron - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int  _setenviron(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(_stringlen(var) + _stringlen(value) + 2);
	if (!buf)
		return (1);
	_stringcpy(buf, var);
	_stringcat(buf, "=");
	_stringcat(buf, value);
	node = info->env;
	while (node)
	{
		p = st_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_nd_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
