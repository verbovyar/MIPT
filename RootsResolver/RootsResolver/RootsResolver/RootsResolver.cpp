#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

enum RootsCount { 
	INFINITE_ROOTS = -1, 
	ZERO = 0, 
	ONEROOT = 1, 
	TWOROOTS = 2
};

const char* GREETING = "Enter coefficients: a, b, c :";
const char* ROOTS_COUNT_MSG = "Roots count : %d\n";

void checkCorrect(int count, double* roots) 
{
	assert(roots != NULL);

	if (count == ZERO ||
		count == INFINITE_ROOTS ||
		count == ONEROOT && roots[0] == 1 ||
		count == ONEROOT && roots[0] == -1 ||
		count == TWOROOTS && roots[0] == 3 && roots[1] == 2)
	{
		printf("CORRECT TEST\n");
	}
	else
	{
		printf("WARNING! INCORRECT TEST %d\n", __LINE__);
	}
}

RootsCount solveLinearEquation(double b, double c, double* roots)
{
	assert(roots != NULL);

	RootsCount count = ZERO;

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

RootsCount solveSquareEquation(double a, double b, double c, double* roots)
{
	assert(roots != NULL);

	RootsCount count = ZERO;
	double discr = b * b - 4 * a * c;

	if (discr > 0)
	{
		count = TWOROOTS;
		double sqrtdis = sqrt(discr);
		roots[0] = (-b + sqrtdis) / (2 * a);
		roots[1] = (-b - sqrtdis) / (2 * a);
	}
	else if (discr == 0)
	{
		count = ONEROOT;
		roots[0] = -b / (2 * a);
	}
	else if (discr < 0)
	{
		count = ZERO;
	}

	return count;
}

void runUnitTests() // Test 
{
	double roots[2];
	double a;
	double b;
	double c;

	//
	b = 1;
	c = 1;
	RootsCount count = solveLinearEquation(b, c, roots); // count == 1  roots[0] == -1
	checkCorrect(count, roots);

	//
	b = 0;
	c = 0;
	count = solveLinearEquation(b, c, roots); // count == -1
	checkCorrect(count, roots);

	//
	b = 0;
	c = 1;
	count = solveLinearEquation(b, c, roots); // count == 0
	checkCorrect(count, roots);

	//
	a = 1;
	b = -5;
	c = 6;
	count = solveSquareEquation(a, b, c, roots); // count == 2 roots[0] == 3 roots[1] == 2
	checkCorrect(count, roots);

	//
	a = 1;
	b = -2;
	c = 1;
	count = solveSquareEquation(a, b, c, roots); // count == 1 roots[0] == 1
	checkCorrect(count, roots);

	//
	a = 1;
	b = 1;
	c = 1;
	count = solveSquareEquation(a, b, c, roots); // count == 0
	checkCorrect(count, roots);
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
	scanf("%lf %lf %lf", &a, &b, &c);
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));

	if (a == 0)
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
