#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

enum RootsCount { 
	INFINITE = -1, 
	ZERO = 0, 
	ONE = 1, 
	TWO = 2
};

void checkCorrect(RootsCount actualCount, double* actualRoots, RootsCount expectedCount, double* expectedRoots)
{
	assert(actualRoots != NULL);
	assert(expectedRoots != NULL);

	bool correct = actualCount == expectedCount;
	int index = 0;
	while (correct && index < (int)actualCount && index < (int)expectedCount)
	{
		correct = actualRoots[index] == expectedRoots[index];
		++index;
	}

	if (correct)
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
		count = ONE;
		roots[0] = -c / b;
	}
	else
	{
		if (c == 0)
		{
			count = INFINITE;
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
		count = TWO;
		double sqrtdis = sqrt(discr);
		roots[0] = (-b + sqrtdis) / (2 * a);
		roots[1] = (-b - sqrtdis) / (2 * a);
	}
	else if (discr == 0)
	{
		count = ONE;
		roots[0] = -b / (2 * a);
	}
	else if (discr < 0)
	{
		count = ZERO;
	}

	return count;
}

RootsCount solveEquation(double a, double b, double c, double* roots)
{
	RootsCount count = ZERO;

	if (a == 0)
	{
		count = solveLinearEquation(b, c, roots);
	}
	else
	{
		count = solveSquareEquation(a, b, c, roots);
	}

	return count;
}

void runUnitTests() // Test 
{
	double actualRoots[2];
	double expectedRoots[2];
	RootsCount expectedCount;
	double a;
	double b;
	double c;

	//
	b = 1;
	c = 1;
	RootsCount actualCount = solveLinearEquation(b, c, actualRoots);
	expectedCount = ONE;
	expectedRoots[0] = -1;
	expectedRoots[1] = NULL;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);

	//
	b = 0;
	c = 0;
	actualCount = solveLinearEquation(b, c, actualRoots);
	expectedCount = INFINITE;
	expectedRoots[0] = NULL;
	expectedRoots[1] = NULL;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);

	//
	b = 0;
	c = 1;
	actualCount = solveLinearEquation(b, c, actualRoots);
	expectedCount = ZERO;
	expectedRoots[0] = NULL;
	expectedRoots[1] = NULL;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);

	//
	a = 1;
	b = -5;
	c = 6;
	actualCount = solveSquareEquation(a, b, c, actualRoots);
	expectedCount = TWO;
	expectedRoots[0] = 3;
	expectedRoots[1] = 2;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);

	//
	a = 1;
	b = -2;
	c = 1;
	actualCount = solveSquareEquation(a, b, c, actualRoots);
	expectedCount = ONE;
	expectedRoots[0] = 1;
	expectedRoots[1] = NULL;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);

	//
	a = 1;
	b = 1;
	c = 1;
	actualCount = solveSquareEquation(a, b, c, actualRoots);
	expectedCount = ZERO;
	expectedRoots[0] = NULL;
	expectedRoots[1] = NULL;
	checkCorrect(actualCount, actualRoots, expectedCount, expectedRoots);
}

int main()
{
	runUnitTests();

	double a = NAN;
	double b = NAN;
	double c = NAN;

	printf("Enter coefficients: a, b, c :");
	scanf("%lf %lf %lf", &a, &b, &c);
	assert(isfinite(a));
	assert(isfinite(b));
	assert(isfinite(c));
	
	double roots[2];
	RootsCount count = solveEquation(a, b, c, roots);

	printf("Roots count : %d\n", count);
	for (int index = 0; index < count; ++index)
	{
		printf("%f ", roots[index]);
	}

	return 0;
}
