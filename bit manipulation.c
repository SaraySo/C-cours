#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the function "func1"
// number - an int giving by the user
// the function printing the number of '1' bits and '0' bits
void func1(int number)
{
	// count variables
	int count0 = 0, count1 = 0;
	// making new number that will be like the number the user give
	int temp_number = number;
	// going through all the bits
	while (temp_number > 0)
	{
		// If current bit is 1
		if (temp_number & 1)
		{
			count1++;
		}

		// If current bit is 0
		else
		{
			count0++;
		}
		// moving wright so we will nknow when to stop the while
		temp_number = temp_number >> 1;
	}
	printf("Count of 0s in %d is %d\n", number, count0);
	printf("Count of 1s in %d is %d", number, count1);
}
// the function "func2"
// number - an int giving by the user
// the function turning the order of bits and printing the number in decimal.
void func2(int number)
{
	// making new number
	int temp_number = 0;

	while (number > 0) // the number should be positive
	{
		temp_number = temp_number << 1; // moving the temp_number left
		temp_number = temp_number | (number & 1); // doing 'and' with the number and then doing 'or' with the temp_number
		number = number >> 1; // moving the number left

	}
	printf("reversed number = %d", temp_number);
}
// the function "func3"
// number - an int giving by the user
// the function printing the multipication of the number with 7.
void func3(int number)
{
	// making new number
	int temp_number = 0;

	temp_number = number << 3; // moving the temp_number left 3 time
	temp_number -= number; // subtructing the temp_number with the number 
	printf("%d * 7 = %d", number, temp_number);

}
//------------------------------------------------------------------------------------
//                          Main
//------------------------------------------------------------------------------------
int main()
{
	int question_number = 0;
	int number = 0;
	printf("Please enter a number of a question: ");
	scanf("%d", &question_number);
	printf("Enter an integer: ");
	scanf("%d", &number);
	switch (question_number)
	{
	case 1:
		func1(number);
		break;
	case 2:
		func2(number);
		break;
	case 3:
		func3(number);
		break;
	default:
		printf("Wrong number\nPlease enter a number between 1 - 3 \n");
		return 1;
	}
	return 0;
}