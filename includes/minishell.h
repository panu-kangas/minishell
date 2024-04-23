/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsaari <tsaari@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 12:15:43 by tsaari            #+#    #+#             */
/*   Updated: 2024/04/23 15:36:12 by tsaari           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

# define ERR_INFILE "no such file or directory: "
# define ERR_OUTFILE "no such file or directory: "
# define ERR_ARG "Invalid  amount of arguments"
# define ERR_PIPE "Pipe error"
# define ERR_FORK "Fork error"
# define ERR_DUP "Dup error"
# define ERR_MALLOC "Malloc error"
# define ERR_EXECVE "error execve"
# define ERR_ACCESS "Access error"
# define ERR_CMD "command not found: "
# define ERR_PER "permission denied: "

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
	int		pipecount;
} t_data;

//free
void	ft_free_double(char **arr);
int		ft_free_data(t_data *data, int code);

//init
void	init_data(t_data *data);
void	init_file(t_file *new);
void	init_token(t_token *new);

//parsing
int		parsing(void);
char	**ft_split_minishell(const char *so);
void	split_redir_and_pipes(t_data *data);
char	**ft_pipex_split(char const *s, char c);
void	handle_no_file(char **tokenarr, int i);

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

#endif