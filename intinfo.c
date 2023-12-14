#include "main.h"

/**
 * clean_info - initializes info_t struct
 * @info: struct address
 */
void clean_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_inform - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_inform(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = stringtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _stringdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		rpc_alias(info);
		rpc_vars(info);
	}
}

/**
 * free_inform - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_inform(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_lists(&(info->env));
		if (info->history)
			free_lists(&(info->history));
		if (info->alias)
			free_lists(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
