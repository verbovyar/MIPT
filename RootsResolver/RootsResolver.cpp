#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<assert.h>

const int ZERO = 0;
const int INFINITE_ROOTS = -1;
const int ONEROOT = 1;
const int TWOROOTS = 2;
const char* GREETING = "Enter coefficients: a, b, c :";
const char* ROOTS_COUNT_MSG = "Roots count : %d\n";

int solveLinearEquation(double b, double c, double* roots)
{
	int count = ZERO;

	if (b != 0)
	{
		count = ONEROOT;
		roots[0] = -c / b;
	}
	else
	{
		if (c == 0)
		{
			count = INFINITE_ROOTS;
		}
		else
		{
			count = ZERO;
		}
	}

	return count;
}

int solveSquareEquation(double a, double b, double c, double* roots)
{
	int count = ZERO;
	double discr = b * b - 4 * a * c;
	double sqrtdis = sqrt(discr);

	if (discr > ZERO)
	{
		count = TWOROOTS;
		roots[0] = (-b + sqrtdis) / 2 / a;
		roots[1] = (-b - sqrtdis) / 2 / a;
	}
	else if (discr == 0)
	{
		count = ONEROOT;
		roots[0] = -b / 2 / a;
	}
	else if (discr < 0)
	{
		count = ZERO;
	}

	return count;
}

void runUnitTests()
{
	double roots[2];

	int count = solveLinearEquation(1, 1, roots); // count == 1  roots[0] == -1
	if (count == 1 && roots[0] == -1)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}

	count = solveLinearEquation(0, 0, roots); // count == -1
	if (count == -1)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}

	count = solveLinearEquation(0, 1, roots); // count == 0
	if (count == 0)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}

	count = solveSquareEquation(1, -5, 6, roots); // count == 2 roots[0] == 3 roots[1] == 2
	if (count == 2 && roots[0] == 3 && roots[1] == 2)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}

	count = solveSquareEquation(1, -2, 1, roots); // count == 1 roots[0] == 1
	if (count == 1 && roots[0] == 1)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}

	count = solveSquareEquation(1, 1, 1, roots); // count == 0
	if (count == 0)
	{
		printf("correct\n");
	}
	else
	{
		printf("incorrect\n");
	}
}

int main()
{
	runUnitTests();

	double roots[2];
	double a = NAN;
	double b = NAN;
	double c = NAN;
	int count = ZERO;

	printf(GREETING);
	scanf("%f %f %f", &a, &b, &c);
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));

	if (a == ZERO)
	{
		count = solveLinearEquation(b, c, roots);
	}
	else
	{
		count = solveSquareEquation(a, b, c, roots);
	}

	printf(ROOTS_COUNT_MSG, count);
	for (int index = 0; index < count; ++index)
	{
		printf("%f ", roots[index]);
	}

	return 0;
}
