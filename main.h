#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct mylist
{
	int num;
	char *str;
	struct mylist *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct infopass
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

typedef struct agru
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

void fn_com(info_t *);
void fr_com(info_t *);
int hsh(info_t *, char **);
int fn_but(info_t *);

char *dpl_char(char *, int, int);
char *fn_path(info_t *, char *, char *);
int is_com(info_t *, char *);

void _inputs(char *);
int loopabc(char **);
int _myputfd(char c, int fd);
int _myputsfd(char *str, int fd);
int _inputchar(char);

char *st_with(const char *, const char *);
char *_stringcat(char *, char *);
int _stringlen(char *);
int _stringcmp(char *, char *);

void _puts(char *);
int _putchar(char);
char *_stringcpy(char *, char *);
char *_stringdup(const char *);

char *_stringncat(char *, char *, int);
char *_stringchr(char *, char);
char *_stringncpy(char *, char *, int);

char **stringtow2(char *, char);
char **stringtow(char *, char *);

char *_memoryset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

int _isalpha(int);
int _atoi(char *);
int interact(info_t *);
int is_dlm(char, char *);
int bfree(void **);

int prt_d(int, int);
char *cvt_number(long int, int, int);
void rve_com(char *);
int _erroratoi(char *);
void prt_error(info_t *, char *);

int _ouralias(info_t *);
int _ourhtry(info_t *);

int _ourcd(info_t *);
int _ourhelp(info_t *);
int _ourexit(info_t *);

void set_inform(info_t *, char **);
void free_inform(info_t *, int);
ssize_t wrt_input(info_t *);
int _getline(info_t *, char **, size_t *);
ssize_t wrt_input(info_t *);
void clean_info(info_t *);
void sigintH(int);

int _ourunsetenv(info_t *);
int ppt_envirn_list(info_t *);
char *_getenviron(info_t *, const char *);
int _ourenv(info_t *);
int _oursetenv(info_t *);

int _unsetenvir(info_t *, char *);
int _setenviron(info_t *, char *, char *);
char **get_ern(info_t *);

int rd_history(info_t *info);
int bld_hist_list(info_t *info, char *buf, int linecount);
int renbr_history(info_t *info);
char *get_hry_file(info_t *info);
int wte_history(info_t *info);

size_t prt_list_string(const list_t *);
int dlt_nd_at_index(list_t **, unsigned int);
void free_lists(list_t **);
list_t *add_nd(list_t **, const char *, int);
list_t *add_nd_end(list_t **, const char *, int);

size_t prt_list(const list_t *);
list_t *nd_sts_with(list_t *, char *, char);
ssize_t get_nd_idx(list_t *, list_t *);
size_t list_length(const list_t *);
char **list_to_strgs(list_t *);

int rpc_alias(info_t *);
int rpc_vars(info_t *);
int rpc_string(char **, char *);
void chk_chn(info_t *, char *, size_t *, size_t, size_t);
int is_chn(info_t *, char *, size_t *);

#endif
