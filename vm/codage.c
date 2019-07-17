/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codage.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmyslyvy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 23:35:21 by mmyslyvy          #+#    #+#             */
/*   Updated: 2019/03/25 23:35:54 by mmyslyvy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"

void	codage_proc(t_process *process, unsigned char codage)
{
	unsigned char	args[3];
	int				i;

	process->op_args_type[0] = 0;
	process->op_args_type[1] = 0;
	process->op_args_type[2] = 0;
	if (g_op_tab[process->op_code].octal && (i = -1) == -1)
	{
		args[0] = (codage & (unsigned char)0b11000000) >> 6;
		args[1] = (codage & (unsigned char)0b00110000) >> 4;
		args[2] = (codage & (unsigned char)0b00001100) >> 2;
		while (++i < g_op_tab[process->op_code].nb_arg)
		{
			if (args[i] == 0b10)
				process->op_args_type[i] = T_DIR;
			else if (args[i] == 0b01)
				process->op_args_type[i] = T_REG;
			else if (args[i] == 0b11)
				process->op_args_type[i] = T_IND;
			else
				process->op_args_type[i] = 0;
		}
	}
	else
		process->op_args_type[0] = T_DIR;
}

int		get_type_size(t_process *process, int arg_type)
{
	if (arg_type == T_DIR)
		return (g_op_tab[process->op_code].label);
	else if (arg_type == T_IND)
		return (2);
	else if (arg_type == T_REG)
		return (1);
	return (0);
}

int		get_offset(t_process *process)
{
	int	offset;
	int i;

	i = 0;
	offset = 1 + g_op_tab[process->op_code].octal;
	while (i < g_op_tab[process->op_code].nb_arg)
	{
		if (process->op_args_type[i] == T_DIR)
			offset += g_op_tab[process->op_code].label;
		else
			offset += get_type_size(process, process->op_args_type[i]);
		i++;
	}
	return (offset);
}

void	*get_t_ind_pointer(t_data *data, t_process *process, int arg_num)
{
	short			new_pos;
	unsigned char	*address;

	address = process->op_args_pointers[arg_num];
	new_pos = *address;
	address++;
	if (address - data->board > MEM_SIZE)
		address = data->board;
	new_pos = (new_pos << 8) | *address;
	if (process->op_code >= 13 && process->op_code <= 15)
		new_pos %= MEM_SIZE;
	else
		new_pos %= IDX_MOD;
	return (&data->board[get_absolute_cord(process->position, new_pos)]);
}

int		write_args_pointers(t_data *data, t_process *process)
{
	int	offset;
	int pos;
	int i;

	offset = (g_op_tab[process->op_code].octal) ? 2 : 1;
	i = 0;
	while (i < g_op_tab[process->op_code].nb_arg)
	{
		if (process->op_args_type[i] & g_op_tab[process->op_code].args[i])
		{
			pos = get_absolute_cord(process->position, offset);
			process->op_args_pointers[i] = &(data->board[pos]);
			offset = pos - process->position +
					get_type_size(process, process->op_args_type[i]);
		}
		else
			return (0);
		i++;
	}
	return (1);
}
