/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 12:15:43 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/30 15:13:03 by pkangas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h> // is it ok to use this?
# include <termios.h> // JUST A TEST
# include <sys/stat.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define ERR_INFILE "no such file or directory: "
# define ERR_OUTFILE "no such file or directory: "
# define ERR_ARG "Invalid amount of arguments"
# define ERR_PIPE "Pipe error"
# define ERR_FORK "Fork error"
# define ERR_DUP "Dup error"
# define ERR_MALLOC "Malloc error"
# define ERR_EXECVE "error execve"
# define ERR_ACCESS "Access error"
# define ERR_CMD "command not found: "
# define ERR_PER "permission denied: "

# define ERR_STAT 1
# define MALLOC_ERR 0
# define SPECIAL_TOKENS "<>()="

typedef struct	s_file
{
	char	*filename;
	int		is_infile;
	int		is_append;
	int		no_filename;
	struct s_file *next;
}	t_file;


typedef struct	s_token
{
	char	*com;
	t_file	*files;
	int		filecount;
	char	**args;
	struct	s_token *next;
} t_token;

typedef struct	s_data
{
	char	*input;
	t_token	*tokens;
	int		proc_count;
	int		prev_exit_status;
} t_data;



typedef struct s_env_lst
{
	char				*name;
	char				*value;
	int					is_global;
	struct s_env_lst	*next;
}				t_env_lst;


//free
void	ft_free_double(char **arr);
int		ft_free_data(t_data *data, int code);

//init
void	init_data(t_data *data, int exit_status);
void	init_file(t_file *new);
void	init_token(t_token *new);

//parsing
int		parsing(void);
char	**ft_split_minishell(const char *so);
void	split_redir_and_pipes(t_data *data);
char	**ft_pipex_split(char const *s, char c);
int		handle_no_file(char **tokenarr, int i, int exit_status);

//parse_utils
char	**make_args_arr(char **tokenarr, int j, int i);
int		add_files_to_token(t_token *new, char **tokenarr);
t_file	*add_file(char *str, int is_append, int is_infile);
int		check_redir(char *str);

//nodes
void	ft_lstadd_back_ms(t_token **tokens, t_token *newnode);
void	ft_lstadd_file_ms(t_file **lst, t_file *new);

//print
void	ft_lstiter2_ms(t_file *files, void (*f)(t_file *));
void	ft_lstiter_ms(t_token *tokens, void (*f)(t_token *));
void	printnode(t_token *token);
void	printfile(t_file *node);



// PANU's FUNCTIONS

t_env_lst	*save_env_list(char **environ);

void		ft_unset(t_env_lst *env_lst, char *arg);
int			ft_export(t_env_lst *env_lst, char *new_env_var);
int			ft_empty_export(t_env_lst *env_lst);
int			ft_pwd(void);
void		ft_env(t_env_lst *env_lst);
int			ft_cd(t_env_lst *env_lst, char *path);
void		ft_echo(int flag, char **args);

int			ft_redirect(t_data *data, int **fd_pipes, int index);
int			ft_heredoc(char *limiter);

char		*get_var_name(char *environ_var);
char		*get_var_value(char *environ_var);
t_env_lst	*check_if_var_exist(t_env_lst *env_lst, char *var_name);

int			process_global_env_node(t_env_lst *env_lst, char *new_env_var);
int			process_non_global_env_node(t_env_lst *env_lst, char *new_env_var);
int			process_null_value_env_node(t_env_lst *env_lst, char *new_env_var);

t_env_lst	*get_global_env_node(char *environ_var);
t_env_lst	*get_non_global_env_node(char *environ_var);
t_env_lst	*get_null_value_env_node(char *environ_var);

void		delete_env_node(t_env_lst *temp);
void		free_env_lst(t_env_lst *env_lst);
void		ft_free_doubleptr(char **ptr);
void		ft_free_int_doubleptr(int **ptr);
int			env_lstsize(t_env_lst *lst);

char		*expand_env_var(t_env_lst *env_lst, char *var_name);

int			write_error(char *cmd, char *specifier, char *err_str);
int			write_sys_error(char *err_str);

char		**get_paths(t_env_lst *env_lst);
char		**make_env_var_array(t_env_lst *env_lst);

int			handle_command(t_data *data, t_env_lst *env_lst, int index);
int			execute_command(char *cmd, char **args, t_env_lst *env_lst, t_data *data);
int			execute_built_in(t_env_lst *env_lst, t_data *data, char *cmd, char **args);

int			check_for_built_in(char *cmd);
int			is_echo(char *cmd);
int			is_pwd(char *cmd);
int			is_env(char *cmd);

int			make_processes(t_data *data, t_env_lst *env_lst);
pid_t		*get_pids(int proc_count);
int			**get_pipes(int pipe_cnt);
void		close_all_pipes(int **fd_pipes, int pipe_cnt);

int			open_infile(char *file);
int			open_outfile(char *file);
int			ft_just_create_file(char *file);
int			open_outfile_append(char *file);
t_token		*get_cur_token(t_data *data, int index);

void		process_signal_main(void);
void		process_signal_commands(void);

#endif