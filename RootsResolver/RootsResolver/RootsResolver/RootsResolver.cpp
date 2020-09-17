#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const double PRECISION = 1e-5;

enum RootsCount {
	INFINITE = -1,
	ZERO = 0,
	ONE = 1,
	TWO = 2
};

bool isZero(double value)
{
	return (fabs(value) < PRECISION);
}

void checkCorrect(RootsCount actualCount, double* actualx1, double* actualx2, RootsCount expectedCount, double* expectedx1, double* expectedx2, int line)
{
	assert (actualx1 != NULL && actualx2 != NULL);
	assert (expectedx1 != NULL && expectedx2 != NULL);

	bool correct = actualCount == expectedCount;
	if (actualCount == ONE)
	{
		correct = *expectedx1 == *actualx1;
	}
	else if (actualCount == TWO)
	{
		correct = (*expectedx1 == *actualx1 && *expectedx2 == *actualx2);
	}

	if (correct)
	{
		printf ("CORRECT TEST\n");
	}
	else
	{
		printf ("WARNING! INCORRECT TEST %d\n", line);
	}
}

RootsCount solveLinearEquation(double b, double c, double* x1)
{
	assert (x1 != NULL);

	RootsCount count = ZERO;

	if (!isZero(b))
	{
		count = ONE;
		*x1 = -c / b;
	}
	else
	{
		if (isZero(c))
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

RootsCount solveSquareEquation(double a, double b, double c, double* x1, double* x2)
{
	assert (x1 != NULL && x2 != NULL);

	RootsCount count = ZERO;
	double discr = b * b - 4 * a * c;

	if (isZero(discr))
	{
		count = ONE;
		*x1 = -b / (2 * a);
	}
	else if (discr > 0)
	{
		count = TWO;
		double sqrtdis = sqrt(discr);
		*x1 = (-b + sqrtdis) / (2 * a);
		*x2 = (-b - sqrtdis) / (2 * a);
	}
	else if (discr < 0)
	{
		count = ZERO;
	}

	return count;
}

RootsCount solveEquation(double a, double b, double c, double* x1, double* x2)
{
	assert (isfinite(a));
	assert (isfinite(b));
	assert (isfinite(c));
	assert (x1 != NULL && x2 != NULL);

	RootsCount count = ZERO;
	
	if (isZero(a))
	{
		count = solveLinearEquation(b, c, x1);
	}
	else
	{
		count = solveSquareEquation(a, b, c, x1, x2);
	}

	return count;
}

void runUnitTests() // Test 
{
	double actualx1 = NULL;
	double actualx2 = NULL;
	double expectedx1 = NULL;
	double expectedx2 = NULL;
	RootsCount expectedCount = ZERO;
	double a = NULL;
	double b = NULL;
	double c = NULL;

	//
	b = 1;
	c = 1;
	RootsCount actualCount = solveLinearEquation(b, c, &actualx1);
	expectedCount = ONE;
	expectedx1 = -1;
	expectedx2 = NAN;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);

	//
	b = 0;
	c = 0;
	actualCount = solveLinearEquation(b, c, &actualx1);
	expectedCount = INFINITE;
	expectedx1 = NAN;
	expectedx2 = NAN;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);

	//
	b = 0;
	c = 1;
	actualCount = solveLinearEquation(b, c, &actualx1);
	expectedCount = ZERO;
	expectedx1 = NAN;
	expectedx2 = NAN;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);

	//
	a = 1;
	b = -5;
	c = 6;
	actualCount = solveSquareEquation(a, b, c, &actualx1, &actualx2);
	expectedCount = TWO;
	expectedx1 = 3;
	expectedx2 = 2;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);

	//
	a = 1;
	b = -2;
	c = 1;
	actualCount = solveSquareEquation(a, b, c, &actualx1, &actualx2);
	expectedCount = ONE;
	expectedx1 = 1;
	expectedx2 = NAN;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);

	//
	a = 1;
	b = 1;
	c = 1;
	actualCount = solveSquareEquation(a, b, c, &actualx1, &actualx2);
	expectedCount = ZERO;
	expectedx1 = NAN;
	expectedx2 = NAN;
	checkCorrect(actualCount, &actualx1, &actualx2, expectedCount, &expectedx1, &expectedx2, __LINE__);
}

int main()
{
	runUnitTests();

	double a = NAN;
	double b = NAN;
	double c = NAN;

	printf("Enter coefficients of equation: a, b, c :");
	scanf("%lf %lf %lf", &a, &b, &c);
	
	double x1 = NAN;
	double x2 = NAN;
	RootsCount count = solveEquation(a, b, c, &x1, &x2);

	if (count == TWO)
	{
		printf("Roots: %lf %lf", x1, x2);
	}
	else if (count == ONE)
	{
		printf("Roots: %lf", x1);
	}
	else
	{
		printf("count of roots: %d", count);
	}

	return 0;
}
