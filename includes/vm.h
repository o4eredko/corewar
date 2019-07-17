/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yochered <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 11:34:26 by yochered          #+#    #+#             */
/*   Updated: 2019/03/27 18:08:35 by dzaporoz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "libft.h"
# include "visu.h"
# include "op.h"
# include <fcntl.h>

# define DIRECT 0
# define INDIRECT 1

typedef struct		s_process
{
	unsigned int	uniq_number;
	int				alive_cycle;
	int				parent_number;
	unsigned int	carry: 1;
	unsigned int	op_code;
	unsigned int	op_args_type[3];
	void			*op_args_pointers[3];
	int				position;
	int				reg[REG_NUMBER + 1];
	int				waiting_cycles;
}					t_process;

typedef struct		s_champ
{
	unsigned int	number;
	char			*name;
	char			*comment;
	unsigned int	exec_size;
	unsigned int	magic_header;
	unsigned char	*exec_code;
}					t_champ;

typedef struct		s_data
{
	t_list			*champs;
	int				champs_amount;
	unsigned int	pl_numbers[MAX_PLAYERS];
	unsigned int	next_p_num;
	int				dump_flag : 1;
	int				visual_flag : 1;
	int				a_flag : 1;
	int				n_flag;
	int				dump_cycles;
	unsigned int	max_process_num;
	int				last_alive_champ;
	int				checks_amount;
	unsigned char	*board;
	int				cycle;
	int				cycle_to_die;
	int				cycles_fr_lst_check;
	unsigned int	live_op_amount;
	t_vs			*vs;
	t_list			*processes;
}					t_data;

void				error_msg(char *str);
int					parse_int(char *str);
char				*get_champ_name(int fd);
char				*get_champ_comment(int fd);
unsigned int		get_champ_4_bytes(int fd);
unsigned char		*get_champ_exec(int fd, int code_size);
void				arg_valid(int argc, char **argv, t_data *data);
void				reserve_numbers(int argc, char **argv, t_data *data);
void				initialization(t_data *data);
void				do_turn(t_data *data);
void				introduce_champs(t_list *champs);
void				codage_proc(t_process *process, unsigned char codage);
int					get_absolute_cord(int cord, int relative_cord);
void				execute_operation(t_process *process, t_data *data);
int					write_args_pointers(t_data *data, t_process *process);
int					get_offset(t_process *process);
void				*get_t_ind_pointer(t_data *data,
	t_process *process, int arg_num);
int					get_type_size(t_process *process, int arg_type);
void				print_board(const void *addr, size_t size);
int					read_arg(t_process *process, int argnum,
	t_data *data, int dir_flag);

void				write_data(void *position, unsigned int value,
	t_data *data);
void				mark_data(t_data *data, int pos, int owner);
void				zjmp(t_process *process, t_data *data);
void				ldi_lldi(t_process *process, t_data *data);
void				sti(t_process *process, t_data *data);
void				fork_lfork(t_process *process, t_data *data);
void				aff(t_process *process, t_data *data);
void				live(t_process *process, t_data *data);
void				st(t_process *process, t_data *data);
void				ld_lld(t_process *process, t_data *data);
void				sub_add(t_process *process, t_data *data);
void				and_or_xor(t_process *process, t_data *data);

void				ft_lstdelcrt(t_list **list, t_list *to_delete);
void				insert_champ(t_data *data, t_champ *champ);
void				to_die_check(t_data *d);

#endif
