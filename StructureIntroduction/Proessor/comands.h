#define TOPDEF double temp1 = top(my_cpu.stck);	\
			pop(my_cpu.stck);					\
			double temp2 = top(my_cpu.stck);	\
			pop(my_cpu.stck)

#define POPDEF double temp1 = top(my_cpu.stck);	\
			pop(my_cpu.stck)

#define OFSET ++my_cpu.OFS;\
			my_cpu.OFS = *(int*)(my_cpu.comand_buffer + my_cpu.OFS) - 2;

DEF_CMD(PUSH, 1, 1, {
	++my_cpu.OFS;

	if (my_cpu.comand_buffer[my_cpu.OFS] == 1)
	{
		push(my_cpu.stck, my_cpu.rax);
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 2)
	{
		push(my_cpu.stck, my_cpu.rbx);
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 3)
	{
		push(my_cpu.stck, my_cpu.rcx); 
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 4)
	{
		push(my_cpu.stck, my_cpu.rdx);
	}
	else
	{
		++my_cpu.OFS;
		push(my_cpu.stck, *(double*)(my_cpu.comand_buffer + my_cpu.OFS));
		my_cpu.OFS += sizeof(double) - 1;
    }
})

DEF_CMD(POP, 2, 1, {
	my_cpu.OFS++;
	if (my_cpu.comand_buffer[my_cpu.OFS] == 1)
	{
		my_cpu.rax = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 2)
	{
		my_cpu.rbx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 3)
	{
		my_cpu.rcx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
	else if (my_cpu.comand_buffer[my_cpu.OFS] == 4)
	{
		my_cpu.rdx = top(my_cpu.stck);
		pop(my_cpu.stck);
	}
})

DEF_CMD(SUM, 3, 0, {
	TOPDEF;

	push(my_cpu.stck, temp1 + temp2);
})

DEF_CMD(DIV, 4, 0, {
	TOPDEF;

	if (temp1 == 0)
	{
		printf("DIV ON ZERO!!!\n");

		return;
	}
	push(my_cpu.stck, temp2 / temp1);
})

DEF_CMD(MUL, 5, 0, {
	TOPDEF;

	push(my_cpu.stck, temp1 * temp2);
})

DEF_CMD(COS, 6, 0, {
	POPDEF;

	push(my_cpu.stck, cos(temp1));
})

DEF_CMD(SIN, 7, 0, {
	POPDEF;

	push(my_cpu.stck, sin(temp1));
})

DEF_CMD(SQRT, 8, 0, {
	POPDEF;

	push(my_cpu.stck, sqrt(temp1));
})

DEF_CMD(OUT, 9, 0, { 
	double result = top(my_cpu.stck);
	pop(my_cpu.stck);
	printf("result:[%lf]\n", result);
})

DEF_CMD(HLT, 10, 0, { 
	my_cpu.isWorking = false; 
})

DEF_CMD(DEC, 11, 0, {
	TOPDEF;

	push(my_cpu.stck, temp2 - temp1);
})

DEF_CMD(JMP, 12, 1, {
	OFSET;
})

DEF_CMD(JBE, 13, 1, {
	TOPDEF;

	if (temp2 <= temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(JB, 14, 1, {
	TOPDEF;

	if (temp2 < temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(JAE, 15, 1, {
	TOPDEF;

	if (temp2 >= temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(JA, 16, 1, {
	TOPDEF;

	if (temp2 > temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(JE, 17, 1, {
	TOPDEF;

	if (temp2 == temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(JNE, 18, 1, {
	TOPDEF;

	if (temp2 != temp1)
	{
		OFSET;
	}
	else
	{
		my_cpu.OFS += sizeof(int);
	}
})

DEF_CMD(CALL, 19, 1, {
	push(my_cpu.call_stack, my_cpu.OFS + sizeof(int));

	OFSET;
})

DEF_CMD(RET, 20, 0, {
	my_cpu.OFS = top(my_cpu.call_stack);
	pop(my_cpu.call_stack);
})

DEF_CMD(IN, 21, 0, {
	double temp_numb = 0;
	scanf("%lf", &temp_numb);
	push(my_cpu.stck, temp_numb);
})
