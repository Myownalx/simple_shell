#include "main.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clean_info(info);
		if (interact(info))
			_puts("$ ");
		_inputchar(BUF_FLUSH);
		r = wrt_input(info);
		if (r != -1)
		{
			set_inform(info, av);
			builtin_ret = fn_but(info);
			if (builtin_ret == -1)
				fn_com(info);
		}
		else if (interact(info))
			_putchar('\n');
		free_inform(info, 0);
	}
	wte_history(info);
	free_inform(info, 1);
	if (!interact(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * fn_but - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int fn_but(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _ourexit},
		{"env", _ourenv},
		{"help", _ourhelp},
		{"history", _ourhtry},
		{"setenv", _oursetenv},
		{"unsetenv", _ourunsetenv},
		{"cd", _ourcd},
		{"alias", _ouralias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_stringcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * fn_com - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fn_com(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_dlm(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = fn_path(info, _getenviron(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fr_com(info);
	}
	else
	{
		if ((interact(info) || _getenviron(info, "PATH=")
			|| info->argv[0][0] == '/') && is_com(info, info->argv[0]))
			fr_com(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			prt_error(info, "not found\n");
		}
	}
}

/**
 * fr_com - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fr_com(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_ern(info)) == -1)
		{
			free_inform(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				prt_error(info, "Permission denied\n");
		}
	}
}
