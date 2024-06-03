/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkangas <pkangas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 12:15:43 by tsaari            #+#    #+#             */
/*   Updated: 2024/06/03 14:13:26 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <sys/stat.h>
# include <errno.h> // is this needed ? is errnocheck needed
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define ERR_STAT 1

typedef struct s_file
{
	char			*filename;
	int				is_amb;
	int				is_infile;
	int				is_append;
	int				no_filename;
	int				hd_pipe[2];
	int				quoted_heredoc;
	struct s_file	*next;
}	t_file;

typedef struct s_token
{
	char			*com;
	t_file			*files;
	int				filecount;
	char			**args;
	int				comcount;
	int				arg_count;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char	*input;
	t_token	*tokens;
	int		proc_count;
	int		prev_exit_status;
	int		std_fd[2];
	char	current_directory[256];
}	t_data;

typedef struct s_parse
{
	char			*str;
	int				isexpand;
	int				istrim;
	struct s_parse	*next;
}	t_parse;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				is_global;
	struct s_env	*next;
}	t_env;

// GLOBAL VAR

extern int	g_signal_marker;

//free and error
int			ft_free_data(t_data *data, int code);
int			ft_free_parse(t_parse *head, int exit_status);
int			write_amb_error(char *err_str); // is this in use ?

//init
void		init_data(t_data *data, int exit_status, char *parsing_cur_dir);
void		init_file(t_file *new);
void		init_token(t_token *new);
void		init_parse(t_parse *new);

//parsing
int			parsing(t_data *data, t_env *env_lst, int exit_status);
int			parse_expansions(t_data *data, t_env *env_lst);
char		**ft_pipex_split(char const *s, char c);
int			handle_no_file(char **tokenarr, int i, int exit_status);
int			ft_char_counter(char *str, char c);
int			handle_only_spaces(t_data *data);
int			ft_array_len(char **arr);
int			skip_file(char **tokenarr, int i);
char		*check_non_spaced_files(char *str);
int			explore_tokenarr(t_token *new, char **tokenarr);
int			parse_com_and_args(t_token *new, char **tokenarr, int exit_status);

//parse split quotes to nodes
int			handle_substrings(char *str, t_parse **head);

//expand node utils
int			ft_iter_and_exp_com(t_parse *lst, t_env *e_lst, t_token *cur);
int			ft_iter_and_exp_files(t_parse *head, t_env *e, t_data *d, int e_st);
char		*expand_str_com(char *str, t_env *env_lst, t_token *current);
char		*ft_lstiter_and_make_new_str(t_parse *lst);
t_parse		*new_node(char *str, int isexpand, int istrim);
void		ft_lstadd_back_parse(t_parse **lst, t_parse *new);
void		change_expand_status(t_parse *head);
void		change_amb_status(t_parse *head, t_file *file);
int			ft_lst_iter_remove_quotes(t_parse *lst);

//expand
char		*expand_str(char *str, t_env *env_lst);
char		*expand_str_file(char *str, t_env *env_lst, char quote);
char		*ft_strjoin_free(char *s1, char *s2);
int			expand_com(t_token *cur, t_env *e_lst, t_data *d, int e_status);
int			expand_args(t_token *cur, t_env *e_lst, t_data *d, int e_st);
int			expand_files(t_token *cur, t_env *e_lst, t_data *d, int e_st);

//parse_utils
int			add_files_to_token(t_token *new, char **tokenarr, int i);
t_file		*add_file(char *str, int is_append, int is_infile);
int			check_redir(char *str);
int			ft_iter_and_exp_arg(t_parse *head, t_env *e_lst, int i);
int			expand_prev_exit_code(t_parse *lst, t_data *data);
char		*trim_str(char *str);
int			check_no_filename(char **tokenarr, int i, int exit_status);

//nodes
void		ft_lstadd_back_ms(t_token **tokens, t_token *newnode);
void		ft_lstadd_file_ms(t_file **lst, t_file *new);

//_________________________________________________
//print   REMOVE THESE!!!!!!!!
void		ft_lstiter2_ms(t_file *files, void (*f)(t_file *));
void		ft_lstiter_ms(t_token *tokens, void (*f)(t_token *));
void		printnode(t_token *token);
void		printfile(t_file *node);
void		ft_lstiter_parse(t_parse *lst, void (*f)(t_parse *));
void		printparse(t_parse *node);

// PANU's FUNCTIONS

t_env		*save_env_list(char **environ);

int			ft_unset(t_env *env_lst, char *var_name);
int			ft_export(t_env *env_lst, char *new_env_var);
int			ft_empty_export(t_env *env_lst);
int			ft_pwd(t_data *data);
void		ft_env(t_env *env_lst);
int			ft_cd(t_data *data, t_env *env_lst, t_token *token);
void		ft_echo(int flag, char **args);
int			ft_exit(t_env *env_lst, t_data *data, char **args, int e_flag);

int			ft_redirect(t_data *data, int **fd_pipes, int index);
int			process_heredoc(t_data *data, t_env *env_lst, int exit_status);
int			write_expanded_hd(t_env *env_lst, char *hd_str, char *lim, int fd);
void		close_hd_pipes(t_data *data);

char		*get_var_name(char *environ_var);
char		*get_var_value(char *environ_var);
t_env		*check_if_var_exist(t_env *env_lst, char *var_name);

int			process_global_env_node(t_env *env_lst, char *new_env_var);
int			process_non_global_env_node(t_env *env_lst, char *new_env_var);
int			process_null_value_env_node(t_env *env_lst, char *new_env_var);

t_env		*get_global_env_node(char *environ_var);
t_env		*get_non_global_env_node(char *environ_var);
t_env		*get_null_value_env_node(char *environ_var);

void		delete_env_node(t_env *temp);
void		free_env_lst(t_env *env_lst);
char		**ft_free_doubleptr(char **ptr);
void		ft_free_int_doubleptr(int **ptr);
int			env_lstsize(t_env *lst);

char		*expand_env_var(t_env *env_lst, char *var_name);

int			write_error(char *cmd, char *specifier, char *err_str);
int			write_exp_error(char *cmd, char *specifier);
int			write_sys_error(char *err_str);
int			write_syntax_error(char *err_str);

char		**get_paths(t_env *env_lst);
char		**make_env_var_array(t_env *env_lst);

int			handle_command(t_data *data, t_env *env_lst, int index, int e_f);
int			execute_command(char *cmd, char **args, t_env *env_lst, t_data *d);
int			execute_built_in(t_env *env, t_data *d, t_token *c_n, int e_f);

int			check_for_built_in(char *cmd);
int			is_echo(char *cmd);
int			is_pwd(char *cmd);
int			is_env(char *cmd);

int			make_processes(t_data *data, t_env *env_lst, int *std_fd);
pid_t		*get_pids(int proc_count);
int			**get_pipes(int pipe_cnt);
void		close_all_pipes(t_data *data, int **fd_pipes, int pipe_cnt);
void		close_and_init_fd(int *fd);

int			open_infile(char *filename, t_file *file);
int			open_outfile(char *filename, t_file *file);
int			ft_just_create_file(char *file);
int			open_outfile_append(char *filename, t_file *file);
t_token		*get_cur_token(t_data *data, int index);

int			process_signal_main(void);
void		process_signal_commands(void);
void		sig_handler_hd(int signum);
int			alter_termios(int flag);

void		copy_cur_dir_to_data(t_data *data, char *cur_dir);

int			check_pipe_syntax(t_data *data, int i);
int			check_quot_syntax(t_data *data);

int			analyze_path(char *path, t_data *data);
int			check_valid_path(char *path);
void		get_parsing_cur_dir(char *parsing_cur_dir);

// from norminetting

void		free_all_from_process(char *cmd_path, char **e_a, char **e_v);
int			check_empty_input(char *cmd);
int			check_cmd_path(char *cmd);
void		copy_cur_dir_to_data(t_data *data, char *cur_dir);
void		add_path_to_data_cur_dir(t_data *data, char *path);
int			update_pwd_env_var(t_data *data, t_env *env_lst, char *cur_dir);
int			handle_oldpwd(t_env *env_lst, t_env *pwd);
int			is_relative_path(char *path);
int			free_paths_cd(char **paths, int paths_count);
int			add_slash_cd(char **paths);
t_token		*get_cur_token(t_data *data, int index);
int			count_outfiles(t_token *cur_token);
t_file		*find_outfile(t_token *cur_token);
int			dup_pipe_out(int **fd_pipes, int index);
int			dup_pipe_in(int **fd_pipes, int index);
int			test_whole_path(char **split_path, int *path_flag, int i);
int			check_parent_dir_permissions(char *path);
int			check_cur_dir_permissions(char **split_cur_dir);
int			check_valid_path(char *path);
int			check_for_outfile(t_token *cur_token);
int			check_for_infile(t_token *cur_token);
int			export_check_valid_input(char *new_env_var);
int			unset_check_valid_input(char *var_name);
int			get_args_size(char **args);
int			free_close_wait(pid_t *pids, int **fd_pipes, t_data *d, int e_st);
int			fork_exit(int **fd_pipes, int index, pid_t *pids, t_data *data);
int			is_builtin(t_data *data, t_env *env_lst, int *std_fd, int *e_stat);
int			check_env_var_change(t_data *data, t_env *env_lst, int *exit_s);
int			handle_heredoc(t_data *data, t_env *env, int *std_fd, int *e_st);
int			store_stdin_stdout(int *std_fd);
char		*update_cur_dir(char *temp_cur_dir, char *path);
int			check_path_backtrack(t_data *data, char *path);
int			free_hd_str(char *str);
void		close_std_fd(int *std_fd);
void		set_signals_to_dfl(void);


int			check_bad_redir_syntax(char **tokenarr, int i);
int			check_empty_rl_input(t_data *data);


#endif