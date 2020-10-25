DEF_CMD(PUSH, 0, 1, {
	++i;
	if (my_cpu.comand_buffer[i] == 1)
	{
		push(my_cpu.stck, my_cpu.rax);
	}
	else if (my_cpu.comand_buffer[i] == 2)
	{
		push(my_cpu.stck, my_cpu.rbx);
	}
	else if (my_cpu.comand_buffer[i] == 3)
	{
		push(my_cpu.stck, my_cpu.rcx);
	}
	else if (my_cpu.comand_buffer[i] == 4)
	{
		push(my_cpu.stck, my_cpu.rdx);
	}
	else
	{
		++i;
		push(my_cpu.stck, *(double*)(my_cpu.comand_buffer + i));
		i += sizeof(double);
		i--;
    }
	})

DEF_CMD(POP, 1, 1, {
	i++;
	if (my_cpu.comand_buffer[i] == 1)
	{
		my_cpu.rax = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[i] == 2)
	{
		my_cpu.rbx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[i] == 3)
	{
		my_cpu.rcx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[i] == 4)
	{
		my_cpu.rdx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
})

DEF_CMD(SUM, 2, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	double temp2 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, temp1 + temp2);
})

DEF_CMD(DEL, 3, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	double temp2 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, temp1 / temp2);
 })

DEF_CMD(MUL, 4, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	double temp2 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, temp1 * temp2);
	})

DEF_CMD(COS, 5, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, cos(temp1));
})

DEF_CMD(SIN, 6, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, sin(temp1));
})

DEF_CMD(SQRT, 7, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, sqrt(temp1));
})

DEF_CMD(OUT, 8, 0, { 
	FILE* file_discriptor = fopen("result.txt", "w");
	double result = top(my_cpu.stck);
	fprintf(file_discriptor, "result:%lf", result);
	fclose(file_discriptor); 
})

DEF_CMD(HLT, 9, 0, { my_cpu.isWorking = false; })

DEF_CMD(MIN, 10, 0, {
	double temp1 = top(my_cpu.stck);
	pop(my_cpu.stck);
	double temp2 = top(my_cpu.stck);
	pop(my_cpu.stck);
	push(my_cpu.stck, temp1 - temp2);
})